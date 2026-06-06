#pragma once
#include "./player.h"
#include <iostream>
#include <string>

using namespace std;

#define CRIBBAGE_SUM_CEIL 31

player::player() {
    for (int i=0; i<6; i++) {
        legal_moves[i] = 0;
    }
}

bool player::find_legal_moves(int sum_cards_played) {
    bool is_legal_move = false;
    int num_cards = player_hand->get_num_cards();
    card* cards = player_hand->get_cards();

    for (int i=0; i<num_cards; i++) {
        if (cards[i].get_value(false) + sum_cards_played <= CRIBBAGE_SUM_CEIL) {
            legal_moves[i] = 1;
            is_legal_move = true;
        } else {
            legal_moves[i] = 0;
        }
    }
    return is_legal_move;
}

void player::set_hand(hand *new_hand) {
    player_hand = new_hand;
}

hand* player::get_hand() {
    return player_hand;
}

std::string player::get_name() {
    return name;
}

randomplayer::randomplayer() {
    name = "randomplayer";
}

action randomplayer::discard_two_random_cards() {
    /*
     * Discards the two last cards in the hand.
     * As draw is random, this results in random discards.
    */
    int pos1 = player_hand->get_num_cards()-1;
    int pos2 = pos1 - 1;


    c1 = *player_hand->get_card(pos1);
    c2 = *player_hand->get_card(pos2);

    // player_hand->remove_2card(pos1, pos2);

    return action(&c1, &c2);
}

action randomplayer::play_random_card() {
    /*
     * Plays first card which is legal.
     * As the hand is randomly drawn this will result in playing random cards.
     * Might have a slight bias towards choosing higher cards first in the second round.
    */
    int num_cards = player_hand->get_num_cards();
    for (int i=0; i<=num_cards; i++) {
        if (legal_moves[i]) {
            c1 = *player_hand->get_card(i);
            // player_hand->remove_card(i);
            return action(&c1);
        }
    }
    return action();
}

action randomplayer::poll_player(bool discard_phase, hand *p_hand, card *cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer) {
    /*
     * Plays more or less randomly
    */
    set_hand(p_hand);
    if (discard_phase) {
        return discard_two_random_cards();
    }
    if (!find_legal_moves(sum_cards)) {
        return action();
    } else {
        return play_random_card();
    }
    
}

action realplayer::discard_two_cards() {
    int pos1 = 0;
    int pos2 = 0;
    cout << "Give index of first card you want to discard";
    cin >> pos1;
    cout << "Give index of second card you want to discard";
    cin >> pos2;
    c1 = *player_hand->get_card(pos1);
    c2 = *player_hand->get_card(pos2);
    // player_hand->remove_2card(pos1, pos2);

    return action(&c1, &c2);
}

action realplayer::play_a_card(int sum_cards_played) {
    if (find_legal_moves(sum_cards_played)) {
        int pos;
        cout << "Give index of the card you want to play";
        cin >> pos;
        c1 = *player_hand->get_card(pos);
        // player_hand->remove_card(pos);

        return action(&c1);
    }
    return action();
}

realplayer::realplayer() {
    name = "realplayer";
}

action realplayer::poll_player(bool discard_phase, hand *p_hand, card *cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer)
{
    set_hand(p_hand);
    cout << "Your score: " << score_self << " ";
    cout << "Opponent score: " << score_opp << " ";
    cout << "Discad Phase: " << discard_phase << endl;
    cout << "cards played: " << endl;
    for (int i = 0; i < num_cards_played; i++) {
        cards_played[i].print_card();
    }
    cout << "Your cards are: " << endl;
    for (int i = 0; i < player_hand->get_num_cards(); i++) {
        player_hand->get_card(i)->print_card();
    }

    if (discard_phase) {
        return discard_two_cards();
    } else {
        return play_a_card(sum_cards);
    }

}

mockplayer::mockplayer() {
    /*
     * A player that gets a predetermined order it should play its card in. Used for testing of the cribbage class
    */
    name = "mockplayer";
    count = 0;
    for(int i=5; i>=0; i--) {
        order[i] = i;
    }
}

action mockplayer::poll_player(bool discard_phase, hand *p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer) {
    action a;
    card* hand_cards = p_hand->get_cards();


    if (discard_phase) {
        count = 0;
        reset(p_hand);
        a = action(&card_order[count], &card_order[count+1]);
        // player_hand->remove_2card(order[count], order[count+1]);
        count++;
    } else {
        a = action(&card_order[count]);
    }

    count++;
    if (count >= 6) {
        count = 0;
        reset(p_hand);
    }
    return a;
}

void mockplayer::reset(hand* p_hand) {
    player_hand = p_hand;
    count = 0;
    for(int i=0; i<6; i++) {
        card_order[i] = *player_hand->get_card(order[i]);
    }
}

policyplayer::policyplayer(int in_seed) : gen(in_seed){
    name = "policyplayer";
}

policyplayer::policyplayer(policy* in_policy, int in_seed) : gen(in_seed) {
    precomputed_policy = *in_policy;
}

policyplayer::policyplayer(std::string filepath, int in_seed) : gen(in_seed) {
    load_policy(filepath);
}

void policyplayer::set_seed(int in_seed) {
    gen.seed(in_seed);
}

bool policyplayer::load_policy(std::string filepath) {
    std::cout << "Loading policy from: " << filepath << std::endl;
    policy_loaded = precomputed_policy.deserialize(filepath);
    return policy_loaded;
}

int policyplayer::size_of_policy() {
    return precomputed_policy.size();
}

std::string policyplayer::get_informationstate_string(bool discard_phase, hand* p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer) {
    /*
     * Returns a string representing the current information-state.
     * This will need to be updated as more information abstractions are used.
     * Currently only supports default abstraction
    */
    int n = p_hand->get_num_cards();
    int* card_ranks = new int[n];
    for (int i = 0; i < n; i++) {
        card_ranks[i] = p_hand->get_card(i)->get_value(true);
    }
    sort(card_ranks, card_ranks + n);

    std::string result = "";
    for (int i = 0; i < n; i++){
        result.append(to_string(card_ranks[i]) + "_");
    }
    
    result.append("|");

    for (int i = 0; i < num_cards_played; i++) {
        result.append(to_string(cards_played[i].get_value(true)) + "_");
    }
    
    delete card_ranks;
    return result;
}

action policyplayer::poll_player(bool discard_phase, hand* p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer) {
    std::string key = get_informationstate_string(discard_phase, p_hand, cards_played, num_cards_played, sum_cards, opponent_num_cards, score_self,  score_opp, is_dealer);
    //if the infostate is not present in the strategy return a random move
    num_available_actions = update_legal_moves(available_actions, available_actions_indexes, p_hand->get_cards(), p_hand->get_num_cards(), sum_cards, !discard_phase);

    if (!precomputed_policy.part_of_policy(key)) {
        // return r.poll_player(discard_phase, p_hand, cards_played, num_cards_played, sum_cards, opponent_num_cards, score_self, score_opp, is_dealer);

    }
    
    std::vector<double> ac = precomputed_policy.action_probabilities(key, num_available_actions);

    std::discrete_distribution<int> d(ac.begin(), ac.end());
    int sampled_action_index = d(gen); // replace with mersene twister
    
    if (discard_phase) {
        c1 = *p_hand->get_card(available_actions_indexes[available_actions[sampled_action_index][0]]);
        c2 = *p_hand->get_card(available_actions_indexes[available_actions[sampled_action_index][1]]);
        return action(&c1, &c2);
    } else {
        c1 = *p_hand->get_card(available_actions_indexes[available_actions[sampled_action_index][0]]);
        return action(&c1);
    }

    return action();
}


highlowpolicyplayer::highlowpolicyplayer(int in_seed) : policyplayer(in_seed) {
    name = "highlowpolicyplayer";
}

highlowpolicyplayer::highlowpolicyplayer(policy* in_policy, int in_seed) : policyplayer(in_policy, in_seed) {

}
highlowpolicyplayer::highlowpolicyplayer(std::string filepath, int inseed) : policyplayer(filepath, inseed) {

}

action highlowpolicyplayer::poll_player(bool discard_phase, hand* p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer) {
    std::string key = get_informationstate_string(discard_phase, p_hand, cards_played, num_cards_played, sum_cards, opponent_num_cards, score_self,  score_opp, is_dealer);
    //if the infostate is not present in the strategy return a random move
    num_available_actions = update_legal_moves(available_actions, available_actions_indexes, p_hand->get_cards(), p_hand->get_num_cards(), sum_cards, !discard_phase);

    if (!precomputed_policy.part_of_policy(key)) {
        // return r.poll_player(discard_phase, p_hand, cards_played, num_cards_played, sum_cards, opponent_num_cards, score_self, score_opp, is_dealer);

    }
    
    std::vector<double> ac = precomputed_policy.action_probabilities(key, std::min(2, num_available_actions));

    std::discrete_distribution<int> d(ac.begin(), ac.end());
    int sampled_action_index = d(gen); // replace with mersene twister
    if (sampled_action_index == 1) {
        sampled_action_index = num_available_actions - 1;
    }
    
    if (discard_phase) {
        c1 = *p_hand->get_card(available_actions_indexes[available_actions[sampled_action_index][0]]);
        c2 = *p_hand->get_card(available_actions_indexes[available_actions[sampled_action_index][1]]);
        return action(&c1, &c2);
    } else {
        c1 = *p_hand->get_card(available_actions_indexes[available_actions[sampled_action_index][0]]);
        return action(&c1);
    }

    return action();
}



permutationpolicyplayer::permutationpolicyplayer(int in_seed) : policyplayer(in_seed) {
    name = "permutationpolicyplayer";
}

permutationpolicyplayer::permutationpolicyplayer(policy* in_policy, int in_seed) : policyplayer(in_policy, in_seed) {

}

permutationpolicyplayer::permutationpolicyplayer(std::string filepath, int inseed) : policyplayer(filepath, inseed) {

}


std::string permutationpolicyplayer::get_informationstate_string(bool discard_phase, hand* p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer) {
    /*
     * Returns a string representing the current information-state.
     * This will need to be updated as more information abstractions are used.
    */
    int n = p_hand->get_num_cards();
    int* card_ranks = new int[n];
    for (int i = 0; i < n; i++) {
        card_ranks[i] = p_hand->get_card(i)->get_value(true);
    }
    std::sort(card_ranks, card_ranks + n);

    std::string result = "";
    for (int i = 0; i < n; i++){
        result.append(std::to_string(card_ranks[i]) + "_");
    }

    delete[] card_ranks;
    return result;
}


action permutationpolicyplayer::poll_player(bool discard_phase, hand* p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer) {
    
    if (actions_set) {
        c1 = player_actions[action_counter];
        action a = action(&c1);
        //If the chosen move is no longer valid choose a random move.
        if (!check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, nullptr, 0, p_hand->get_cards(), p_hand->get_num_cards(), a)) {
            return r.poll_player(discard_phase, p_hand, cards_played, num_cards_played, sum_cards, opponent_num_cards, score_self, score_opp, is_dealer);
        }
        
        action_counter++;
        if (action_counter == 4) {
            actions_set = false;
            action_counter = 0;
        }
        return a;
    }
    
    std::string key = get_informationstate_string(discard_phase, p_hand, cards_played, num_cards_played, sum_cards, opponent_num_cards, score_self,  score_opp, is_dealer);
    //if the infostate is not present in the strategy return a random move
    
    if (!precomputed_policy.part_of_policy(key)) {
        return r.poll_player(discard_phase, p_hand, cards_played, num_cards_played, sum_cards, opponent_num_cards, score_self, score_opp, is_dealer);
    }
    
    std::vector<double> ac = precomputed_policy.action_probabilities(key);

    std::discrete_distribution<int> disrib(ac.begin(), ac.end());
    int sampled_action_index = disrib(gen); // replace with mersene twister
    
    int a;
    int b;
    int c;
    int d;
    map_24_to_4(&a, &b, &c, &d, sampled_action_index);
    actions_set = true;

    num_available_actions = update_legal_moves(available_actions, available_actions_indexes, p_hand->get_cards(), p_hand->get_num_cards(), sum_cards, true);
    
    player_actions[0] = *p_hand->get_card(available_actions_indexes[available_actions[a][0]]);
    player_actions[1] = *p_hand->get_card(available_actions_indexes[available_actions[b][0]]);
    player_actions[2] = *p_hand->get_card(available_actions_indexes[available_actions[c][0]]);
    player_actions[3] = *p_hand->get_card(available_actions_indexes[available_actions[d][0]]);
    action_counter = 0;

    //now that actions are set we can poll our selfs again
    return poll_player(discard_phase, p_hand, cards_played, num_cards_played, sum_cards, opponent_num_cards, score_self, score_opp, is_dealer);
}
