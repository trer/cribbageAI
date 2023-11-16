#pragma once
#include "./cribbage.h"


using namespace std;




cribbage::cribbage(int in_seed, int first_dealer, deck* in_deck) : gen(in_seed), distrib(1, 2){
    seed = in_seed;

    player1_ready = false;
    player2_ready = false;
    init(first_dealer, in_deck);
}


cribbage::cribbage(int in_seed, player *player1_in, player *player2_in, int first_dealer, deck* in_deck): gen(in_seed), distrib(1, 2) {
    seed = in_seed; 

    set_player(player1_in, 1);
    set_player(player2_in, 2);
    
    init(first_dealer, in_deck);
}

void cribbage::init(int first_dealer, deck* in_deck) {
    base_deck = deck(distrib(gen));
    if (in_deck == nullptr) {
        game_deck = &base_deck;
    } else {
        game_deck = in_deck;
    }
    
    wins[0] = 0;
    wins[1] = 0;
    reset(first_dealer);
}

void cribbage::set_deck(deck *in_deck) {
    /*
     * Supports insertion of specialized decks
    */
   game_deck = in_deck;
}

void cribbage::reset(int first_dealer) {
        

    player1_score = 0;
    player2_score = 0;

    dealer_score = &player1_score;
    pone_score = &player2_score;

    current_dealer = first_dealer; //use seed to set dealer with some randoming
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

void cribbage::set_current_player(bool pone_to_play) {

    if (pone_to_play) {
        current_player = pone;
        current_opp = dealer;
        current_player_score = pone_score;
        current_opp_score = dealer_score;
    } else {
        current_player = dealer;
        current_opp = pone;
        current_player_score = dealer_score;
        current_opp_score = pone_score;
    }
}

int cribbage::check_win() {
    /*
     * Checks if one of the players has reached 121 points.
     * Not able to tell which player reached first, so has to be called each time a player gets more points.
    */
    if (player1_score >= 121) {
        return 1;
    }
    if (player2_score >= 121) {
        return 2;
    }
    return 0;
}

int cribbage::start_game() {
    int w = 0;
    while(!w) { //While round doesn't end in a win keep playing
        w = round();
    }
    return w; // return the winner
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

int cribbage::get_player1_score()
{
    return player1_score;
}

int cribbage::get_player2_score()
{
    return player2_score;
}


int cribbage::play_phase() {
    card cards_played[8];
    int num_cards_played = 0;
    int sum_cards = 0;
    bool pone_go = false;
    bool dealer_go = false;
    bool pone_to_play = true;


    while (pone->get_hand()->get_num_cards() || dealer->get_hand()->get_num_cards()) {
        
        set_current_player(pone_to_play); //update current acting player

        //get action from acting player
        action a;
        if (exsists_legal_move(current_player->get_hand()->get_cards(), current_player->get_hand()->get_num_cards(), sum_cards)) {
            a = current_player->poll_player(false, cards_played, num_cards_played, sum_cards, current_opp->get_hand()->get_num_cards(), *current_player_score, *current_opp_score);
            //Checking is move is legal
            if(!check_valid_move(false, cards_played, num_cards_played, sum_cards, crib.get_cards(), current_player->get_hand()->get_cards(), current_player->get_hand()->get_num_cards(), a)) {
                cout << "Current player tried to do an illigal move" << endl;
                if (current_player == player1) {
                    cout << "Player1 is current player" << endl;
                    return -1;
                }
                if (current_player == player2) {
                    cout << "Player2 is current_player" << endl;
                    return -2;
                }
                return -3;
            }
        } else {
            //if pone has no legal move he has to call go
            a = action();
        }

        //if action is not go, then play the card and score the acting player
        if (a.card1 != 0) {
            cards_played[num_cards_played] = *a.card1;
            num_cards_played++;
            sum_cards = sum_cards + a.card1->get_value(false);
            *current_player_score = *current_player_score + score_played_card(cards_played, num_cards_played, a.card1, sum_cards);
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
    //Last player to play a card gets a point (pone is inverted)
    if(pone_to_play) {
        *dealer_score = *dealer_score + 1;
    } else {
        *pone_score = *pone_score + 1;
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
    *dealer_score = *dealer_score + score_cards(&crib, true);

    if (check_win()) {
        return check_win();
    }
    return 0;
}


int cribbage::round() {
    swap_dealer();
    game_deck->shuffle();
    hand dealer_hand = hand(game_deck);
    hand pone_hand = hand(game_deck);
    dealer_hand.draw(6);
    pone_hand.draw(6);

    // //create mirrors
    // hand dealer_hand_mirror = dealer_hand;
    // hand pone_hand_mirror = pone_hand;
    // current_player_hand_mirror = &pone_hand_mirror; 
    // current_opp_hand_mirror = &dealer_hand_mirror;

    dealer->set_hand(&dealer_hand);
    pone->set_hand(&pone_hand);

    action discard1 = dealer->poll_player(true, 0, 0, 0, 6, *dealer_score, *pone_score);
    action discard2 = pone->poll_player(true, 0, 0, 0, 6, *pone_score, *dealer_score);
    if(!check_valid_move(true, nullptr, 0, 0, nullptr, dealer_hand.get_cards(), dealer_hand.get_num_cards(), discard1)) {
        cout << "Player 1 tried to do an illigal move" << endl;
        return -1;
    }
    if(!check_valid_move(true, nullptr, 0, 0, nullptr, pone_hand.get_cards(), pone_hand.get_num_cards(), discard2)) {
        cout << "Player 2 tried to do an illigal move" << endl;
        return -2;
    }
    card crib_cards[4];
    crib_cards[0] = *discard1.card1;
    crib_cards[1] = *discard1.card2;
    crib_cards[2] = *discard2.card1;
    crib_cards[3] = *discard2.card2;

    crib = hand(game_deck, crib_cards, 4);

    //give the cut to all hands
    dealer_hand.draw_cut();
    pone_hand.draw_cut();
    crib.draw_cut();

    *dealer_score = *dealer_score + score_cut(game_deck->cut());
    if (check_win()) {
        return check_win();
    }

    int play_phase_winner = play_phase(); //supports error handling
    if (play_phase_winner) {
        return play_phase_winner;
    }

    //setting all hand sizes to 5 before matching
    pone_hand.set_num_cards(5);
    dealer_hand.set_num_cards(5);
    crib.set_num_cards(5);

    if(matching()) {
        return check_win();
    }
    
    return 0;
}
