#pragma once
#include <algorithm>
#include <fstream>
#include <forward_list>
#include <iostream>
#include <string>
#include <unordered_map>

#include "action.h"
#include "card.h"

bool part_of_hand(card* hand_cards, int num_hand_cards, card* card_to_check);
bool check_valid_move(bool discard_phase, card *cards_played, int num_cards_played,
                    int sum_cards, card* crib, int num_cards_in_crib, card* player_hand, int num_cards_in_player_hand, 
                    action player_action);
bool compare_cards(card* c1, card* c2);
bool exsists_legal_move(card* hand_cards, int num_hand_cards, int sum_cards_played);
int update_legal_moves(int** available_actions, int* available_actions_indexes, card* hand_cards, int num_hand_cards, int sum_cards_played, bool discard_done);

int* get_array(int i);
int* get_array(int i, int j);


class policy {
    private:

    public:
    
        std::unordered_map<std::string, std::vector<double>> infostates;

        
        policy();
        policy(std::unordered_map<std::string, std::vector<double>*> cfr_infostates, int index);
        policy(std::unordered_map<std::string, std::vector<double>> in_infostates);

        bool part_of_policy(std::string key);
        std::vector<double> action_probabilities(std::string key);
        std::vector<double> action_probabilities(std::string info_state_key, int num_available_actions);
        

        bool serialize(std::string filepath);
        bool deserialize(std::string filepath);
};

