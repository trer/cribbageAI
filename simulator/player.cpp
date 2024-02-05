#pragma once
#include "./player.h"
#include <iostream>
#include <string>

using namespace std;

#define CRIBBAGE_SUM_CEIL 31

player::player() {
    *player_hand; //the player hand
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

randomplayer::randomplayer() {

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

realplayer::realplayer()
{
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
    count = 0;
    order;
}

action mockplayer::poll_player(bool discard_phase, hand *p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer) {
    action a;
    switch (count)
    {
    case 0:
        a = action(player_hand->get_card(order[count]), player_hand->get_card(order[count+1]));
        // player_hand->remove_2card(order[count], order[count+1]);
        count++;
        break;
    case -1:
        a = action();
    default:
        a = action(player_hand->get_card(order[count]));
        // player_hand->remove_card(order[count]);
        break;
    }

    count++;
    if (count >= 6) {
        count = 0;
    }
    return a;
}

policyplayer::policyplayer() : gen(rand()){

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

    if (!precomputed_policy.part_of_policy(key)) {
        return r.poll_player(discard_phase, p_hand, cards_played, num_cards_played, sum_cards, opponent_num_cards, score_self, score_opp, is_dealer);
    }
    
    std::vector<double> ac = precomputed_policy.action_probabilities(key);

    std::discrete_distribution<int> d(ac.begin(), ac.end());
    int sampled_action_index = d(gen); // replace with mersene twister
    num_available_actions = update_legal_moves(available_actions, available_actions_indexes, p_hand->get_cards(), p_hand->get_num_cards(), sum_cards, true);
    
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
