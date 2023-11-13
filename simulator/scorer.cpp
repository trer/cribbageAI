#include "./hand.h"
#include "./card.h"
#include "./card.h"
#include "./player.h"
#include <cmath>

using namespace std;


int score_set_15(card* cards, int num_cards, int current_value, int pos) {
    int val;
    int score = 0;
    for (int i=pos; i<num_cards; i++) {
        val = cards[i].get_value(false) + current_value;
        if (val < 15) {
            if (pos+1 < num_cards) {
                score = score + score_set_15(cards, num_cards, val, i+1);
            }
            continue;
        }
        if (val == 15) {
            score = score + 2; 
        }
        
        
    }
    return score;
}

int score_cut(card* cut_card) {
    //If cut is a jack dealer scores 2 for his heels in the cut.
    if (cut_card->get_value(true) == 11) {
        return 2;
    }
    return 0;
}

int score_cards(card* cards, int num_cards, bool is_crib, card* cut_card) {
    int total_score = 0;
    int ranks[13] = {0};
    int suits[4] = {0};
    int values_10_and_over = 0;
    for (int i=0; i<num_cards; i++) {
        ranks[cards[i].get_value(true)-1] ++;
        suits[cards[i].get_suit(true)] ++; //suits are not indexes, prolly change suit internal value
    }
    for (int i=10; i<13; i++) {
        values_10_and_over = values_10_and_over + ranks[i];
    }

    //2, 3, 4 of a kind
    for (int v : ranks) {
        if (v == 4) {
            total_score = total_score + 12;
            continue;
        }
        if (v == 3) {
            total_score = total_score + 6;
            continue;
        }
        if (v == 2) {
            total_score = total_score + 2;
            continue;
        }
    }

    //runs
    int run_length = 0;
    int num_runs = 1;
    for (int i = 0; i < 13; i++) {
        if (ranks[i]) {
            // if the next value in the series exists, extend the run
            run_length ++;
            num_runs = num_runs*ranks[i];
        } else {
            // calculate score for the run if it is longer than 3 and reset score
            if (run_length >= 3) {
                total_score = total_score + num_runs*run_length;
            }
            run_length = 0;
            num_runs = 1;
            // could probably break here as it shouldn't be possible to have to seperate runs (5 card hand)
        }
    }
    

    //flush
    int same_suit = 0;
    for (int i=0; i<4; i++) {
        same_suit = max(same_suit, suits[i]);
    }
    if (same_suit == 5) {
        total_score = total_score + 5;
    } else if (!is_crib && same_suit == 4) {
        total_score = total_score + 4;
    }
    
    //nobs and his heels
    if (ranks[10]) {
        //if cut_card is Jack this is scored before the play phase and not in the matching phase
        int cut_suit = cut_card->get_suit(true);
        for (int i=0; i<num_cards-1; i++) {
            // If card is both a Jack and the same suit as cut card
            if (cards[i].get_value(true) == 11 && cards[i].get_suit(true) == cut_suit) {
                total_score = total_score + 1; // His nob
            }
        }
    }

    int score_15 = 0;
    //sets of 15 only for what?
    for (int i = 0; i < num_cards; i++) {
        score_15 = score_15 + score_set_15(cards, num_cards, cards[i].get_value(false), i+1);
    }
    total_score = total_score + score_15;


    return total_score;
}

int score_cards(hand * hand, bool is_crib) {
    card* cards = hand->get_cards();
    card* cut_card = cards+hand->get_num_cards()-1;
    return score_cards(hand->get_cards(), hand->get_num_cards(), is_crib, cut_card);
}


int score_cards(player *player) {
    return score_cards(player->get_hand(), false);
}


int find_run_of(int* ranks, int played_rank, int len_needed) {
    int min_card = max(played_rank - len_needed, 0);
    for(int i=min_card; i<11; i++) {
        if (ranks[i] == 1) {
            int run_len = 1;
            for(int j=i+1; j<13; j++) {
                if (ranks[j] == 1) {
                    run_len++;
                } else {
                    if (run_len == len_needed) {
                        return run_len;
                    }
                    return 0;
                }
            }
        }
    }
    return 0;
}


int score_played_card(card* card_history, int num_cards_played, card* card_played, int sum_cards) {
    
    int ranks[13] = {0};
    int total_score = 0;
    for (int i=0; i<num_cards_played; i++) {
        ranks[card_history[i].get_value(true)-1]++;
    }


    //15, 31 seperate
    if (sum_cards == 15) {
        total_score = total_score + 2;
    } else if (sum_cards == 31) {
        total_score = total_score + 1; //playing last card is scored seperatly, at least for now.
    }
    

    //2, 3, 4 of a kind, seperate
    int rank_of_last_card = card_history[num_cards_played-1].get_value(true);
    int score_to_add = 2;
    for (int i=num_cards_played-2; i>=0; --i) {
        if (rank_of_last_card == card_history[i].get_value(true)){
            total_score = total_score + score_to_add;
            score_to_add = score_to_add + 2;
        } else {
            break;
        }
    }
    
    //runs seperate (hard?)
    for (int i=num_cards_played; i>=3; i--) {
        //check if a run consists of the i last cards
        if (find_run_of(ranks, card_history[num_cards_played-1].get_value(true), i)) {
            total_score = total_score + i;
            break;
        }
        
        ranks[card_history[num_cards_played-i].get_value(true)-1]--; //remove the last card
    }

    return total_score;
}