#include "./cribbage.h"
#include "cribbage.h"

using namespace std;




cribbage::cribbage(int in_seed) : gen(in_seed), distrib(1, 2){
    seed = in_seed;

    player1_ready = false;
    player2_ready = false;
    init();
}


cribbage::cribbage(int in_seed, player *player1_in, player *player2_in): gen(in_seed), distrib(1, 2) {
    seed = in_seed; 

    set_player(player1_in, 1);
    set_player(player2_in, 2);
    
    init();
}

void cribbage::init() {
    game_deck = deck(distrib(gen));
    
    wins[0] = 0;
    wins[1] = 0;
    reset();
}

void cribbage::reset() {
        

    player1_score = 0;
    player2_score = 0;

    dealer_score = &player1_score;
    pone_score = &player2_score;
    
    current_dealer = distrib(gen)%2; //use seed to set dealer with some randoming
    
}


void cribbage::set_player(player* player, int num) {
    if (num == 1) {
        player1 = player;
        player1_ready = true;
    }
    if (num == 2) {
        player2 = player;
        player2_ready = true;
    }
}


void cribbage::swap_dealer() {

    if (current_dealer == 1) {
        dealer = player1;
        pone = player2;
        dealer_score = &player1_score;
        pone_score = &player2_score;
        current_dealer = 2;
    } else {
        dealer = player2;
        pone = player1;
        dealer_score = &player2_score;
        pone_score = &player1_score;
        current_dealer = 1;
    }
}

int cribbage::check_win() {
    if (player1_score >= 121) {
        return 1;
    }
    if (player2_score >= 121) {
        return 2;
    }
    return 0;
}

int cribbage::start_game() {
    swap_dealer();
    while(!round()) {
        swap_dealer();
    }
    return check_win();
}

int *cribbage::start_games(int num_games) {
    wins[0] = 0;
    wins[1] = 0;
    int winner = 0;
    for(int i=0; i < num_games; ++i) {
        winner = start_game();
        wins[winner-1]++;
        reset();
    }
    return wins;
}

int cribbage::play_phase() {
    card cards_played[8];
    int num_cards_played = 0;
    int sum_cards = 0;
    bool pone_go = false;
    bool dealer_go = false;
    bool pone_to_play = true;


    while (pone->get_hand()->get_num_cards() || dealer->get_hand()->get_num_cards()) {
        //get action from acting player
        action a;
        if (pone_to_play) {
            a = pone->poll_player(false, cards_played, num_cards_played, sum_cards, dealer->get_hand()->get_num_cards(), *pone_score, *dealer_score);
        } else {
            a = dealer->poll_player(false, cards_played, num_cards_played, sum_cards, pone->get_hand()->get_num_cards(), *dealer_score, *pone_score); 
        }
        
        //if action is not go, then play the card and score the acting player
        if (a.card1 != 0) {
            cards_played[num_cards_played] = *a.card1;
            num_cards_played++;
            sum_cards = sum_cards + a.card1->get_value(false);
            if (pone_to_play) {
                *pone_score = *pone_score + score_played_card(cards_played, num_cards_played, a.card1, sum_cards);
            } else {
                *dealer_score = *dealer_score + score_played_card(cards_played, num_cards_played, a.card1, sum_cards);
            }
        } else {
            //otherwise check if the other player has already called go
            if (pone_to_play) {
                if (dealer_go) {
                    *pone_score = *pone_score + 1;
                }
                pone_go = true;
            } else {
                if (pone_go) {
                    *dealer_score = *dealer_score + 1;
                }
                dealer_go = true;
                
            }
        }
        //check if any player has won
        if (check_win()) {
            return check_win();
        }
        //If both dealer and pone has called go, we start on a new deck
        if (dealer_go && pone_go) {
            num_cards_played = 0;
            sum_cards = 0;
            pone_go = false;
            dealer_go = false;
        }
        //swap current player
        pone_to_play = !pone_to_play;
    }
    return 0;
}

int cribbage::matching() {
    int s = score_cards(pone);
    *pone_score = *pone_score + s;
    if (check_win()) {
        return check_win();
    }
    *dealer_score = *dealer_score + score_cards(dealer);
    if (check_win()) {
        return check_win();
    }
    return 0;
}


int cribbage::round() {
    game_deck.shuffle();
    hand dealer_hand = hand(&game_deck);
    hand pone_hand = hand(&game_deck);
    dealer_hand.draw(6);
    pone_hand.draw(6);

    dealer->set_hand(&dealer_hand);
    pone->set_hand(&pone_hand);

    action discard1 = dealer->poll_player(true, 0, 0, 0, 6, *dealer_score, *pone_score);
    action discard2 = pone->poll_player(true, 0, 0, 0, 6, *pone_score, *dealer_score);
    card crib_cards[4];
    crib_cards[0] = *discard1.card1;
    crib_cards[1] = *discard1.card2;
    crib_cards[2] = *discard2.card1;
    crib_cards[3] = *discard2.card2;

    hand crib = hand(&game_deck, crib_cards, 4);

    //give the cut to all hands
    dealer_hand.draw_cut();
    pone_hand.draw_cut();
    crib.draw_cut();

    *dealer_score = *dealer_score + score_cut(game_deck.cut());
    if (check_win()) {
        return check_win();
    }

    if (play_phase()) {
        return check_win();
    }

    pone_hand.set_num_cards(5);
    dealer_hand.set_num_cards(5);

    if(matching()) {
        return check_win();
    }
    
    return 0;
}
