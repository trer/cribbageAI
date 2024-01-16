#pragma once
#include "greedyplayer.h"



greedyplayer::greedyplayer() {
}

bool greedyplayer::card_in_hand(card* hand_cards, int num_cards, card* card_to_check) {
    for(int i=0; i<num_cards; i++) {
        if(compare_cards(&hand_cards[i], card_to_check)) {
            return true;
        }
    }
    return false;
}

int greedyplayer::get_best_card(card* hand_cards, int num_cards, card* cards_played, int num_cards_played, int sum_cards) {
    double best_score = -1.0;
    int best_card_index = -1;
    int score = 0;
    card tmp_cards_played[8];
    card tmp_card_to_play;
    int counter = 0;
    while (counter < num_cards_played) {
        tmp_cards_played[counter] = cards_played[counter];
        counter++;
    }
    for (int i = 0; i < num_cards; i++) {
        tmp_cards_played[counter] = hand_cards[i];
        if (check_valid_move(false, cards_played, num_cards_played, sum_cards, cards_in_crib, 2,
             hand_cards, num_cards, action(&hand_cards[i]))) 
            {
                score = score_played_card(tmp_cards_played, counter+1, &hand_cards[i], sum_cards);
            } else {
                score = -1;
            }
        if (score > best_score) {
            best_score = score;
            best_card_index = i;
        }
    }
    
    return best_card_index;
}


int* greedyplayer::get_best_two_cards(card* hand_cards, int num_cards, bool is_dealer) {
    double best_score = -1.0;
    card tmp_crib[3];
    card* tmp_cut;
    card tmp_hand_cards[6];
    card* deck_cards = player_deck.get_cards();
    for (int i = 0; i < num_cards; i++) {
        for (int j = i+1; j < num_cards; j++) {
            tmp_crib[0] = card(hand_cards[i]);
            tmp_crib[1] = card(hand_cards[j]);
            //create a "hand" of cards removing the two in the crib
            int offset = 0;
            for (int l = 0; l<num_cards; l++) {
                card c_tmp = hand_cards[l];
                if (!compare_cards(&c_tmp, &tmp_crib[0]) && !compare_cards(&c_tmp, &tmp_crib[1])) {
                    tmp_hand_cards[l-offset] = card(c_tmp);
                } else {
                    offset++;
                }
            }
            
            int score = 0;
            for (int k = 0; k < 52; k++) {
                tmp_cut = deck_cards + k;
                
                if (card_in_hand(hand_cards, num_cards, tmp_cut)) {
                    continue;
                } 
                tmp_hand_cards[4] = *tmp_cut;
                tmp_crib[2] = card(*tmp_cut);
                if (is_dealer) {
                    score += score_cards(tmp_crib, 3, true, tmp_cut);
                } else {
                    score -= score_cards(tmp_crib, 3, true, tmp_cut);
                }
                score += score_cards(tmp_hand_cards, 5, false, tmp_cut);

            }
            score = score / 52;
            if (score > best_score) {
                best_cards_indexes[0] = i;
                best_cards_indexes[1] = j;
                best_score = score;
            }
            
        }
        
    }
    return best_cards_indexes;
}

action greedyplayer::discard_two_cards(bool is_dealer) {
    /*
    * Iterate through all possible discards and weight them against all possible cutcards.
    * Choose the one that gives the highest average score
   */
    get_best_two_cards(player_hand->get_cards(), player_hand->get_num_cards(), is_dealer);
    c1 = *player_hand->get_card(best_cards_indexes[0]);
    c2 = *player_hand->get_card(best_cards_indexes[1]);
    player_hand->remove_2card(best_cards_indexes[0], best_cards_indexes[1]);

    return action(&c1, &c2);    
}

action greedyplayer::play_a_card(card* cards_played, int num_cards_played, int sum_played_cards) {
    int index = get_best_card(player_hand->get_cards(), player_hand->get_num_cards(), cards_played, num_cards_played, sum_played_cards);
    c1 = *player_hand->get_card(index);
    player_hand->remove_card(index);
    return action(&c1);
}


action greedyplayer::poll_player(bool discard_phase, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer) {
    /*
     * Plays greedily
    */
    if (discard_phase) {
        return discard_two_cards(is_dealer);
    }
    if (!update_legal_moves(sum_cards)) {
        return action();
    } else {
        return play_a_card(cards_played, num_cards_played, sum_cards);
    }
    
}