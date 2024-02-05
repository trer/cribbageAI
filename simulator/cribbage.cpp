#pragma once
#include "cribbage.h"

#define SCORE_TO_WIN 121
#define NUM_CARDS_IN_HAND 6

using namespace std;

namespace simulator {


cribbage::cribbage(int in_seed, int first_dealer, deck* in_deck) : gen(in_seed), distrib(1, 2){
    /*
     * Constructor without players, game might break if they are not set due to some short-cuts in programming
     * Player_ready set to false, but not necessarily checked. BEWARE!
    */
    seed = in_seed;

    player1_ready = false;
    player2_ready = false;
    init(first_dealer, in_deck);
}

cribbage::cribbage() : gen(0), distrib(1, 2) {
    srand((unsigned)time(0));
    int r = rand(); 
    seed = r;
    gen.seed(seed);
    player1_ready = false;
    player2_ready = false;
    init(0, nullptr);
}

cribbage::cribbage(int in_seed, player *player1_in, player *player2_in, int first_dealer, deck* in_deck): gen(in_seed), distrib(1, 2) {
    /*
     * Main constructor, if possible use this!
     * Sets both players
     * Both first_dealer and in_deck can be set automatically by this constructor
    */
    seed = in_seed; 

    set_player(player1_in, 1);
    set_player(player2_in, 2);
    
    init(first_dealer, in_deck);
}

cribbage::cribbage(const cribbage &source) {
    /*
     * Creates a copy from existing source
    */
    copy_self(source);
}

const cribbage& cribbage::operator= (const cribbage& source) {
    /*
     * Override of the = operator. Copies all relevant fields and arrays. Also keeps same internal state of the random number generator.
    */
    if (this != &source) {
        cout << "COPYING FOR ASSIGNMENT, expensive, might want to look into why it is copied" << endl;
        copy_self(source);
    }
    return *this;
}


void cribbage::copy_self(const cribbage &source) {
    /*
     * Copies all relevant field from source to this.
     * Should work for all stages of a round
    */
    gen = source.gen;
    distrib = source.distrib;
    
    seed = source.seed;

    base_deck = deck(source.base_deck);
    game_deck = &base_deck;

    if (source.crib.get_num_cards() > 0) {
        crib = hand(source.crib); 
        //If cards have been put into the crib we have also revealed the cut card.
        cut_card = base_deck.cut(); 
    }

    // Does this sometime lead to problems (calls lead to same player instance, meaning that statefull players will cause the games to go differently)
    // but we might not need to set the players at all
    player1_ready = source.player1_ready;
    if (source.player1_ready) {
        player1 = source.player1;
    }
    player2_ready = source.player2_ready;
    if (source.player2_ready) {
        player2 = source.player2;
    }
    
    
    if (source.round_setup_once) {
        round_setup_once = source.round_setup_once;
        player1_score = source.player1_score;
        player2_score = source.player2_score;

        next_dealer = source.next_dealer;

        dealer_hand = hand(source.dealer_hand);
        pone_hand = hand(source.pone_hand);
        set_dealer_and_pone(3-next_dealer); //If next dealer is 2 then current is 1. Math checks out
        dealer->set_hand(&dealer_hand);
        pone->set_hand(&pone_hand);
        
        dealer_discard_done = source.dealer_discard_done;
        pone_discard_done = source.pone_discard_done;
        dealer_discard = source.dealer_discard;
        pone_discards = source.pone_discards;

        num_cards_played = source.num_cards_played;
        num_cards_played_since_new_stack = source.num_cards_played_since_new_stack;
        sum_cards = source.sum_cards;


        for (int i = 0; i < num_cards_played; i++) {
            cards_played[i] = source.cards_played[i];
        }
        for (int i = 0; i < num_cards_played_since_new_stack; i++) {
            cards_played_since_new_stack[i] = source.cards_played_since_new_stack[i];
        }

        pone_go = source.pone_go;
        dealer_go = source.dealer_go;
        pone_to_play = source.pone_to_play;

        set_current_player();
    } else {
        reset(source.first_dealer);
    }
}

void cribbage::init(int first_dealer, deck* in_deck) {
    /*
     * Initializes deck, and prepare for a round.
    */
    base_deck = deck(gen());
    if (in_deck == nullptr) {
        game_deck = &base_deck;
    } else {
        game_deck = in_deck;
    }
    
    reset(first_dealer);
}

void cribbage::set_deck(deck *in_deck) {
    /*
     * Supports insertion of specialized decks
    */
   game_deck = in_deck;
}

void cribbage::reset(int f_dealer) {
    /*
     * Reset scores
    */
    if (f_dealer != -1) {
        first_dealer = f_dealer;
    } else {
        first_dealer = next_dealer;
    }
    player1_score = 0;
    player2_score = 0;


    switch (first_dealer)
    {
    case 0:
        first_dealer = distrib(gen);
        break;
    case 1:
        first_dealer = 1;
        break;
    case 2:
        first_dealer = 2;
        break;
    default:
        cout << "game should break, otherwise investigate soon!" << endl;
        break;
    }

    //This is also set in swap dealer, so might not need to set here!
    //Or if I am unlucky it might need to be set backwards here. Keep for a little while.
    // if (first_dealer == 1) {
    //     dealer_score = &player1_score;
    //     pone_score = &player2_score;
    // } else {
    //     dealer_score = &player2_score;
    //     pone_score = &player1_score;
    // }

    next_dealer = first_dealer; //other player will be first dealer next turn
    
}


void cribbage::set_player(player* player, int num) {
    /*
     * Sets playerpointer
    */
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
    /*
     * Switches dealer and all acompanying variables.
     * if player 1 is dealer => player 1 become pone.
     * if player 2 is dealer => player 2 become pone.
     * if player 1 is pone => player 1 become dealer.
     * if player 2 is pone => player 2 become dealer.
    */

    set_dealer_and_pone(next_dealer);
    if (next_dealer == 1) {
        next_dealer = 2;
    } else {
        next_dealer = 1;
    }
}

void cribbage::set_dealer_and_pone(int current_dealer) {
    if (current_dealer == 1) {
        dealer = player1;
        pone = player2;
        dealer_score = &player1_score;
        pone_score = &player2_score;
    } else {
        dealer = player2;
        pone = player1;
        dealer_score = &player2_score;
        pone_score = &player1_score;
    }
}

void cribbage::set_current_player() {
    /*
     * Sets all current player variables.
     * It also updates legal moves for the current players!
     * This must be called between each move!
     * Calling this once too many doesn't change anything
     * Calling it once too few generally breaks the program :)
    */
    if (pone_to_play) {
        current_player = pone;
        current_opp = dealer;
        current_hand = &pone_hand;
        current_opp_hand = &dealer_hand;
        current_player_score = pone_score;
        current_opp_score = dealer_score;
        num_available_actions = update_legal_moves(available_actions, available_actions_indexes, pone_hand.get_cards(), pone_hand.get_num_cards(), sum_cards, is_discard_done());
    } else {
        current_player = dealer;
        current_opp = pone;
        current_hand = &dealer_hand;
        current_opp_hand = &pone_hand;
        current_player_score = dealer_score;
        current_opp_score = pone_score;
        num_available_actions = update_legal_moves(available_actions, available_actions_indexes, dealer_hand.get_cards(), dealer_hand.get_num_cards(), sum_cards, is_discard_done());
    }
}

int cribbage::check_win() {
    /*
     * Checks if one of the players has reached 121 points.
     * Not able to tell which player reached first, so has to be called each time a player gets more points.
    */
    if (player1_score >= SCORE_TO_WIN) {
        return 1;
    }
    if (player2_score >= SCORE_TO_WIN) {
        return 2;
    }
    return 0;
}

int cribbage::start_game() {
    /*
     * plays rounds until a player wins. Automatically polls players.
    */
    int w = 0;
    while(!w) { //While round doesn't end in a win keep playing
        w = round();
    }
    return w; // return the winner
}

int cribbage::start_games(int num_games) {
    /*
     * Runs num_games number of games.
    */
    int wins = 0;
    int winner = 0;
    for(int i=0; i < num_games; ++i) {
        winner = start_game();
        switch (winner) {
        case 1:
            wins++;
            break;
        case 2:
            // Do nothing if player 2 wins
            break;
        default:
            // This is likely because one of the players did an illigal move.
            cout << "non of the players won a game, not good!" << winner << endl;
            break;
        }
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

int cribbage::get_current_player() {
    /*
     * returns 1 if it is player 1's turn.
     * returns 2 if it is player 2's turn.
     * This requires current player to always be set.
    */

    //Current player is always set, and is therefor safer to use!
    // if (!pone_discard_done) {
    //     if(pone == player1) {
    //         return 1;
    //     } else {
    //         return 2;
    //     }
    // }
    // if (!dealer_discard_done) {
    //     if(dealer == player1) {
    //         return 1;
    //     } else {
    //         return 2;
    //     }
    // }

    if (current_player == player1) {
        return 1;
    }
    if (current_player == player2) {
        return 2;
    }
    return 0;
}

int cribbage::get_current_pone() {
    if (pone == player1) {
        return 1;
    }
    if (pone == player2) {
        return 2;
    }
    return 0;
}

bool cribbage::player_has_called_go(int player) {
    if (player == 1) {
        if (player1 == pone) {
            return pone_go;
        } else {
            return dealer_go;
        }
    } else if (player == 2) {
        if (player2 == pone) {
            return pone_go;
        } else {
            return dealer_go;
        }
    }
    return false;
}


hand *cribbage::get_player_hand(int player) {
    /*
     * Best if this can be avoided being used, but I don't think that is possible.
     * returns the hand of a player (not a good security choice, but will only be me running the code).
     * -1 returns the crib
    */
    if(player == 1) {
        if (player1 == dealer) {
            return &dealer_hand;
        } else {
            return &pone_hand;
        }
    } else if (player == 2) {
        if (player2 == dealer) {
            return &dealer_hand;
        } else {
            return &pone_hand;
        }
    } else if (player == -1) {
        return &crib;
    }
    return nullptr;
}

int cribbage::get_player_hand_size(int player) {
    /*
     * Returns hand size of a player
     * -1 returns the size of the crib
    */
    return get_player_hand(player)->get_num_cards();
}

int cribbage::get_num_cards_played() {
    return num_cards_played_since_new_stack;
}

card* cribbage::get_cards_played() {
    return cards_played_since_new_stack;
}

int cribbage::get_sum_cards_played() {
    /*
     * Returns the sum of the cards currently played
    */
    return sum_cards;
}

void cribbage::set_discard(action a) {
    /*
     * If you know which player's turn it is to discard you don't have to know wheter they are a dealer or not.
    */
    if (current_player == dealer) {
        set_discard(a, 'd');
    } else {
        set_discard(a, 'p');
    }
}

void cribbage::set_discard(action a, char player) {
    /*
     * Sets discard of player (pone or dealer)
     * Calls set_current player to update leagal moves
     * If both players are finnished discarding this could call handle discard, but it currently does not.
    */
    if (player == 'd') {
        dealer_discard = a;
        dealer_discard_done = true;
        pone_to_play = true; //at the end of discard it should always be pone to play
        if (!pone_discard_done) {
            set_current_player();
        }
    } else if (player == 'p') {
        pone_discards = a;
        pone_discard_done = true;
        if (!dealer_discard_done) { //if the dealer had not set its discards update_current player
            pone_to_play = false;
            set_current_player();
        }
    }
    
    if (pone_discard_done && dealer_discard_done) {
        handle_discards();
    }
}

void cribbage::set_discard(action a, int player) {
    /*
     * Sets discard for a spesific player.
    */
    if (player == 1) {
        if (player1 == dealer) {
            set_discard(a, 'd');
        } else {
           set_discard(a, 'p');
        }
    } else if (player == 2) {
        if (player2 == dealer) {
            set_discard(a, 'd');
        } else {
            set_discard(a, 'p');
        }
    }
}

void cribbage::set_play_action(action a) {
    /*
     * Sets the play action of the current player
    */
    acting_player_action = a;
}

void cribbage::set_action_for_player(int p_int) {
    /*
     * polls the given player for a move and sets it to the player action.
    */

    if (get_current_player() != p_int && is_discard_done()) {
        cout << "Trying to set action for a player whos turn it is not!" << endl;
        return;
    }

    if (!is_discard_done()) {
        bool is_pone;

        //find out if we should poll pone or dealer
        //eg.if player1 == dealer && p_int == 1 --> poll dealer 
        if (p_int == 1) {
            if (player1 == pone) {
                is_pone = true;
            } else {
                is_pone = false;
            }
        } else if (p_int == 2) {
            if (player2 == pone) {
                is_pone = true;
            } else {
                is_pone = false;
            }
        }
        if (is_pone) {
            set_discard(pone->poll_player(!is_discard_done(), &pone_hand, cards_played_since_new_stack, num_cards_played_since_new_stack, sum_cards, pone->get_hand()->get_num_cards(), *pone_score, *dealer_score, false), 'p');
        } else {
            set_discard(dealer->poll_player(!is_discard_done(), &dealer_hand, cards_played_since_new_stack, num_cards_played_since_new_stack, sum_cards, dealer->get_hand()->get_num_cards(), *dealer_score, *pone_score, true), 'd');
        }

    } else {
        //If it is not discard phase just poll current player
        set_play_action(current_player->poll_player(!is_discard_done(), current_hand, cards_played_since_new_stack, num_cards_played_since_new_stack, sum_cards, get_player_hand_size(3-p_int), *current_player_score, *current_opp_score, !pone_to_play));
    }

    
}

void cribbage::setup_play_phase() {
    /*
     * Sets up the play-phase.
     * Currently this is moved to setup_round, so this might not be needed
    */
    //pass
    //TODO: remove
}

int cribbage::resolve_action() {
    /*
     * Assumes that dealer_discard and pone_discard is set or that acting_player_action is set, depending on the phase.
     * Places the card(s) in play and removes them from the respective players hand.
     * Does currently not check if it is a legal move. (This is not needed for apply action from list, but could be useful for set_action + resolve action elsewhere).
    */

    //if action is not go, then play the card and score the acting player
    if (acting_player_action.card1 != 0) {
        cards_played[num_cards_played] = *acting_player_action.card1;
        cards_played_since_new_stack[num_cards_played_since_new_stack] = *acting_player_action.card1;
        num_cards_played++;
        num_cards_played_since_new_stack++;
        sum_cards = sum_cards + acting_player_action.card1->get_value(false);
        *current_player_score = *current_player_score + score_played_card(cards_played_since_new_stack, num_cards_played_since_new_stack, acting_player_action.card1, sum_cards);
        //once we are done with the card we can remove it
        current_hand->remove_card(acting_player_action.card1);
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
    //If both dealer and pone has called go, we start on a new pile
    if (dealer_go && pone_go) {
        num_cards_played_since_new_stack = 0;
        sum_cards = 0;
        pone_go = false;
        dealer_go = false;
    }
    //swap current player
    pone_to_play = !pone_to_play;
    set_current_player();
    return check_win();
}

void cribbage::last_card_played() {
    /*
     * Last player to play a card gets a point.
     * Last player to call go gets to call this
     * This gets called after resolve action therefore PONE IS INVERTED BY DESIGN!
     * Might need to be inverted again. Don't know anymore
    */
    if(pone_to_play) {
        *dealer_score = *dealer_score + 1;
    } else {
        *pone_score = *pone_score + 1;
    }
}

int cribbage::play_phase() {
    /*
     * runs the play_phase automatically, might want to remove this as it sorta crashes with the rest of the program.
    */
    setup_play_phase();
    while (pone_hand.get_num_cards() || dealer_hand.get_num_cards()) {
        
        set_current_player(); //update current acting player

        if (exsists_legal_move(current_hand->get_cards(), current_hand->get_num_cards(), sum_cards)) {
            acting_player_action = current_player->poll_player(false, current_hand, cards_played_since_new_stack, num_cards_played_since_new_stack, sum_cards, current_opp_hand->get_num_cards(), *current_player_score, *current_opp_score, !pone_to_play);
            //Checking is move is legal
            if(!check_valid_move(false, cards_played, num_cards_played, sum_cards, crib.get_cards(), crib.get_num_cards(), current_hand->get_cards(), current_hand->get_num_cards(), acting_player_action)) {
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
            acting_player_action = action();
        }

        
        //check if any player has won
        if (resolve_action()) {
            return check_win();
        }
        
    }
    last_card_played();
    if (check_win()) {
        return check_win();
    }
    return 0;
}


void cribbage::matching_setup() {
    /*
     * Setting all hand sizes to 5 before matching.
     * Cards are not deleted from the array in the hand, meaning that this is eqivalent to picking them back up again.
    */
    pone_hand.set_num_cards(5);
    dealer_hand.set_num_cards(5);
    crib.set_num_cards(5);
}

int cribbage::matching_score_pone() {
    /*
     * Scores matching for only the pone.
    */
    int s = score_cards(pone);
    *pone_score = *pone_score + s;

    return check_win();
}

int cribbage::matching_score_dealer() {
    /*
     * Score matching for just the dealer.
    */
    *dealer_score = *dealer_score + score_cards(dealer);
    *dealer_score = *dealer_score + score_cards(&crib, true);
    return check_win();
}

int cribbage::matching() {
    /*
     * Setsup matching phase
     * Scores points for pone, and then for dealer.
     * Sets round done to true.
    */
    matching_setup();

    if (matching_score_pone()) {
        return check_win();
    }

    if(matching_score_dealer()) {
        return check_win();
    }
    round_done = true;
    return 0;
}

void cribbage::setup_round() {
    /*
     * Sets up the round, swaps dealer to initialize all variables.
     * Sets all variables to what they should be.
     * Also setup while we are at it
    */
    
    swap_dealer();
    game_deck->shuffle();
    round_done = false;
    
    dealer_hand = hand(game_deck);
    pone_hand = hand(game_deck);
    dealer_hand.draw(6);
    pone_hand.draw(6);
    dealer->set_hand(&dealer_hand);
    pone->set_hand(&pone_hand);
    dealer_discard_done = false;
    pone_discard_done = false;

    //play_phase
    num_cards_played = 0;
    num_cards_played_since_new_stack = 0;
    sum_cards = 0;
    pone_go = false;
    dealer_go = false;
    pone_to_play = true;
    round_setup_once = true;

    set_current_player(); //set current player variables and update legal moves.
}

void cribbage::skip_to_play_phase(player* discard_policy) {
    /*
     * discard_policy should preferably not be a statefull policy/player.
     * Does the discard phase automatically with a default policy
     * if discard policy is nullptr it will use its internal players to run the discard phase.
     * Crashes if those players aren't set.
     * Weird things might happen if a player wins because jack is revealed after play-phase, but I think it is fine as it is not possible to win on the first move for the pone anyway.
    */

    if (discard_policy == nullptr) {
        //if no policy is given fall back to players.
        
        //if players aren't set create random players for them.
        if (!player1_ready || !player2_ready) {
            if (!player1_ready) {
                player1 = new randomplayer();
                player1_ready = true;
            }
            if (!player2_ready) {
                player2 = new randomplayer();
                player2_ready = true;
            }
            if (next_dealer == 1) {
                //if next dealer is 1 then current must be 1
                set_dealer_and_pone(2);
            } else {
                set_dealer_and_pone(1);
            }
        }
        set_discard(pone->poll_player(true, &pone_hand, 0, 0, 0, 6, *pone_score, *dealer_score, false), 'p');
        set_discard(dealer->poll_player(true, &dealer_hand, 0, 0, 0, 6, *dealer_score, *pone_score, true), 'd');

    } else {
        set_discard(discard_policy->poll_player(true, &pone_hand, 0, 0, 0, 6, *pone_score, *dealer_score, false), 'p');
        set_discard(discard_policy->poll_player(true, &dealer_hand, 0, 0, 0, 6, *dealer_score, *pone_score, true), 'd');
    }

    set_current_player();
}


int cribbage::handle_discards() {
    /*
     * Checks that both discards are legal moves.
     * puts the cards in the crib
     * then removes the cards from the players hands.
     * Then it reveals the cut card
     * And updates the legal moves for the pone.
    */

    if(!check_valid_move(true, nullptr, 0, 0, nullptr, 0, dealer_hand.get_cards(), dealer_hand.get_num_cards(), dealer_discard)) {
        cout << "dealer tried to do an illigal move" << endl;
        return -1;
    }
    if(!check_valid_move(true, nullptr, 0, 0, nullptr, 0, pone_hand.get_cards(), pone_hand.get_num_cards(), pone_discards)) {
        cout << "pone tried to do an illigal move" << endl;
        return -2;
    }
    crib_cards[4];
    crib_cards[0] = *dealer_discard.card1;
    crib_cards[1] = *dealer_discard.card2;
    crib_cards[2] = *pone_discards.card1;
    crib_cards[3] = *pone_discards.card2;

    crib = hand(game_deck, crib_cards, 4);
    //once we are done with everything else we can remove the cards
    dealer_hand.remove_2card(dealer_discard.card1, dealer_discard.card2);
    pone_hand.remove_2card(pone_discards.card1, pone_discards.card2);
    

    //give the cut to all hands
    dealer_hand.draw_cut();
    pone_hand.draw_cut();
    crib.draw_cut();

    *dealer_score = *dealer_score + score_cut(game_deck->cut());
    set_current_player();
    if (check_win()) {
        return check_win();
    }
    return 0;
}

int cribbage::round() {
    /*
     * Automatically plays a round of cribbage.
    */
    setup_round();

    pone_discards = pone->poll_player(true, &pone_hand, 0, 0, 0, 6, *pone_score, *dealer_score, false);
    dealer_discard = dealer->poll_player(true, &dealer_hand, 0, 0, 0, 6, *dealer_score, *pone_score, true);
    int discard_winner = handle_discards();
    if (discard_winner) {
        return discard_winner;
    }

    int play_phase_winner = play_phase(); //supports error handling
    if (play_phase_winner) {
        return play_phase_winner;
    }


    if(matching()) {
        return check_win();
    }
    
    return 0;
}

bool cribbage::is_discard_done() {
    /* 
     * Returns true if both players has discarded cards
     * 
    */
    return dealer_discard_done && pone_discard_done;
}

bool cribbage::is_playphase_done() {
    /*
     * Returns true if both players hands are empty, or if the game is over.
    */
    return (dealer_hand.get_num_cards() == 0 && pone_hand.get_num_cards() == 0) || check_win();
}

bool cribbage::is_round_done() {
    /* 
     * When checkning if the round is done we also have to
     * account for the fact that the game might be over.
     */
    return round_done || check_win();
}

int cribbage::get_num_available_actions() {
    /*
     * Returns the number of legal actions for the current player
    */
    return num_available_actions;
}

int* cribbage::get_available_action_indexes() {
    /*
     * Returns the available indexes for an action.
     * This one conserns me. Should the available indexes just be [0, num_available_actions)?
     * LOOK INTO THIS
    */
    return available_actions_indexes;
}

int** cribbage::get_available_actions() {
    /*
     * Returns the available indexes for an action.
     * This one conserns me. Should the available indexes just be [0, num_available_actions)?
     * LOOK INTO THIS
    */
    return available_actions;
}

int cribbage::apply_action_from_list(int action_index) {
    /*
     * Applies an action from available_actions. 
     * -1 means calling go (this is done automatically)
     * 0 plays the lowest card in hand
     * 1 playes the second lowest card in hand (und so weiter...)
     * Part of the tree-search algorithms, together with with setup_round() and matching()
     *  
    */
    int win = 0;
    if (action_index < -1 || action_index >= num_available_actions) {
        //If chosen action is not a legal action return -99 and change no internal states.
        //This could cause recursion
        return -99;
    }
    if (!is_discard_done()) {
        set_discard(action(current_hand->get_card(available_actions_indexes[available_actions[action_index][0]]), current_hand->get_card(available_actions_indexes[available_actions[action_index][1]])));

    } else {
        if (action_index == -1) {
            set_play_action(action());
        } else {
            set_play_action(action(current_hand->get_card(available_actions_indexes[available_actions[action_index][0]])));
        }
        resolve_action();
        if (get_player_hand_size(1) == 0 && get_player_hand_size(2) == 0) {
            last_card_played();
        }
        win = check_win(); //before a new action is taken we have to check if the game is over.
        if (win) {
            return win;
        }
        // Currently doing that in is_round_done();
    }
    //setup next turn
    set_current_player();
    //if a player has no available moves he must call go (I want to do this automatically)
    if (get_num_available_actions() == 0 && !is_playphase_done()) {
        win = apply_action_from_list(-1);
    }
    return win;
}

int cribbage::get_point_diff(int player) {
    /*
     * Returns the point differential between the input player and the other player.
    */
    if (player == 1) {
        return player1_score - player2_score;
    } else if (player == 2) {
        return player2_score - player1_score;    
    }
    cout << "ERROR wrong player input. Expected 1 or 2. Got: " << player << endl;
    return 0;
}



void cribbage::randomize(int player){
    /*
     * Randomize all the information that the player doesn't know.
     * Eg. Randomize opponent player hand, and what is placed in the crib, 
     * but not player cards, and played cards
    */
   // TODO: implement
   // Points of focus
   // Player hands has to be updated?) 
   // Old Copy should not change as we do these changes
   // Not all gamestates are as likely (eg. the opponent is more likely to keep certain cards over others)
}


std::string cribbage::get_informationstate_string(int player) {
    /*
     * Returns a string representing the current information-state.
     * This will need to be updated as more information abstractions are used.
    */
    int n = get_player_hand_size(player);
    int* card_ranks = new int[n];
    for (int i = 0; i < n; i++) {
        card_ranks[i] = get_player_hand(player)->get_card(i)->get_value(true);
    }
    sort(card_ranks, card_ranks + n);

    std::string result = "";
    for (int i = 0; i < n; i++){
        result.append(to_string(card_ranks[i]) + "_");
    }
    
    result.append("|");

    for (int i = 0; i < num_cards_played_since_new_stack; i++) {
        result.append(to_string(cards_played_since_new_stack[i].get_value(true)) + "_");
    }

    delete card_ranks;    
    return result;
}

std::string cribbage::get_informationstate_string(int player) {
    /*
     * Returns a string representing the current information-state.
     * This will need to be updated as more information abstractions are used.
    */
    int n = get_player_hand_size(player);
    int* card_ranks = new int[n];
    for (int i = 0; i < n; i++) {
        card_ranks[i] = get_player_hand(player)->get_card(i)->get_value(true);
    }
    sort(card_ranks, card_ranks + n);

    std::string result = "";
    for (int i = 0; i < n; i++){
        result.append(to_string(card_ranks[i]) + "_");
    }
    
    delete card_ranks;    
    return result;
}

}
