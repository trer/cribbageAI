#pragma once
#include "greedyplayer.h"



greedyplayer::greedyplayer() : player() {
    name = "greedyplayer";
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
             hand_cards, num_cards, action(&hand_cards[i]))) {
                score = score_played_card(tmp_cards_played, counter+1, &hand_cards[i], sum_cards);
            } else {
                score = -1;
            }
        if (score > best_score) {
            best_score = score;
            best_card_index = i;
        }
    }
    if (best_card_index == -1) {
        throw std::runtime_error("greedyplayer: No card is the best card!");
    }
    
    return best_card_index;
}


int* greedyplayer::get_best_two_cards(card* hand_cards, int num_cards, bool is_dealer) {
    /*
     * If we get a SEH error here, it is likely because this is called during the play-phase
    */
    double best_score = -1.0;
    card tmp_crib[3];
    card* tmp_cut = new card();
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
            if (is_dealer) {
                    score += score_cards(tmp_crib, 2, true, tmp_cut);
                } else {
                    score -= score_cards(tmp_crib, 2, true, tmp_cut);
                }
                score += score_cards(tmp_hand_cards, 4, false, tmp_cut);
            // for (int k = 0; k < MAX_NUM_DECKCARDS; k++) {
            //     tmp_cut = deck_cards + k;
                
            //     if (card_in_hand(hand_cards, num_cards, tmp_cut)) {
            //         continue;
            //     } 
            //     tmp_hand_cards[4] = *tmp_cut;
            //     tmp_crib[2] = card(*tmp_cut);
            //     if (is_dealer) {
            //         score += score_cards(tmp_crib, 3, true, tmp_cut);
            //     } else {
            //         score -= score_cards(tmp_crib, 3, true, tmp_cut);
            //     }
            //     score += score_cards(tmp_hand_cards, 5, false, tmp_cut);

            // }
            // score = score / MAX_NUM_DECKCARDS;
            if (score > best_score) {
                best_cards_indexes[0] = i;
                best_cards_indexes[1] = j;
                best_score = score;
            }
            
        }
        
    }
    delete tmp_cut;
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
    // player_hand->remove_2card(best_cards_indexes[0], best_cards_indexes[1]);

    return action(&c1, &c2);    
}

action greedyplayer::play_a_card(card* cards_played, int num_cards_played, int sum_played_cards) {
    int index = get_best_card(player_hand->get_cards(), player_hand->get_num_cards(), cards_played, num_cards_played, sum_played_cards);
    c1 = *player_hand->get_card(index);
    // player_hand->remove_card(index);
    return action(&c1);
}


action greedyplayer::poll_player(bool discard_phase, hand* p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer) {
    /*
     * Plays greedily
    */
    set_hand(p_hand);
    if (discard_phase) {
        return discard_two_cards(is_dealer);
    }
    if (!find_legal_moves(sum_cards)) {
        return action();
    } else {
        return play_a_card(cards_played, num_cards_played, sum_cards);
    }
    
}

greedypolicy::greedypolicy() : greedyplayer(), policy() {
    policy::name = "greedyplayer_policy";
}

bool greedypolicy::part_of_policy(std::string key) {
    return false;
}

std::vector<double> greedypolicy::action_probabilities(std::string key) {
    
    std::stringstream in_str(key);
    std::string card_str;
    std::string outer_str;
    std::vector<std::vector<int>> card_ranks_lst = std::vector<std::vector<int>>();
    std::vector<std::vector<std::string>> hand_lst = std::vector<std::vector<std::string>>();
    while(std::getline(in_str, outer_str, '|')) {
        std::stringstream current_stream(outer_str);
        std::vector<int> card_ranks = std::vector<int>();
        std::vector<std::string> strlst = std::vector<std::string>();
        while(std::getline(current_stream, card_str, '_')) {
            strlst.push_back(card_str);
        }
        for(std::string ele : strlst) {
            card_ranks.push_back(std::stoi(ele));        
        }
        card_ranks_lst.push_back(card_ranks);
        hand_lst.push_back(strlst);
    }
    if(card_ranks_lst.size() == 1) {
        card_ranks_lst.push_back(std::vector<int>());
    }
    card hand_cards[4];
    card played_cards[8];
    for (int i = 0; i < card_ranks_lst[0].size(); i++) {
        hand_cards[i] = card(card_ranks_lst[0][i], 'N'+i);
    }
    for (int i = 0; i < card_ranks_lst[1].size(); i++) {
        played_cards[i] = card(card_ranks_lst[1][i], 'R'); //using non standard suits is fine since suit values are not counted during playing phase
    }
    hand player_hand = hand(nullptr, hand_cards, card_ranks_lst[0].size());
    int sum_cards_played = 0;
    for (int i=0; i<card_ranks_lst[1].size(); i++) {
        sum_cards_played += played_cards[i].get_value(false);
    }
    return poll_player_and_return_probabilities(false, &player_hand, played_cards, card_ranks_lst[1].size(), sum_cards_played);
    
}

std::vector<double> greedypolicy::action_probabilities(std::string info_state_key, int num_available_actions) {
    return action_probabilities(info_state_key);
}

std::vector<double> greedypolicy::action_probabilities(simulator::cribbage* game) {
    if (!game->is_discard_done() || game->is_playphase_done()) {
        throw std::runtime_error("greedyplayer: greedypolicy, game is not in the correct phase");
    }
    // hand h = *game->get_player_hand(game->get_current_player());
    // std::sort(h.get_cards(), h.get_cards() + h.get_num_cards());
    std::vector<double> ac = poll_player_and_return_probabilities(!game->is_discard_done(), game->get_player_hand(game->get_current_player()), game->get_cards_played(), game->get_num_cards_played(), game->get_sum_cards_played());
    return ac;
}


std::vector<double> greedypolicy::poll_player_and_return_probabilities(bool discard_phase, hand* player_hand, card* cards_played, int num_cards_played, int sum_cards_played) {
    
    double best_score = -1.0;
    double num_best_score = 0.0;
    std::vector<double> scores = std::vector<double>();

    card tmp_cards_played[8];
    card tmp_card_to_play;
    int counter = 0;

    int num_cards = player_hand->get_num_cards();
    card* hand_cards = player_hand->get_cards();

    while (counter < num_cards_played) {
        tmp_cards_played[counter] = cards_played[counter];
        counter++;
    }
    for (int i = 0; i < num_cards; i++) {
        scores.push_back(0.0);
        tmp_cards_played[counter] = hand_cards[i];
        if (check_valid_move(false, cards_played, num_cards_played, sum_cards_played, nullptr, 0,
            hand_cards, num_cards, action(&hand_cards[i]))) {
            scores[i] = score_played_card(tmp_cards_played, counter+1, &hand_cards[i], num_cards_played) + 1; //Adding 1 to all scores to avoid dividing by 0
        } else {
            scores[i] = -1;
        }
        
        if (scores[i] == best_score) {
            num_best_score += 1;
        }
        if (scores[i] > best_score) {
            best_score = scores[i];
            num_best_score = 1;
        }
    }


    for (int i=0; i<scores.size(); i++) {
        if (scores[i] != best_score) {
            scores[i] = 0.0;
        } else {
            scores[i] /= num_best_score;
        }
    }
    
    return scores;
}

