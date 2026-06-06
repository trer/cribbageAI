#pragma once
#include <algorithm>
#include <fstream>
#include <forward_list>
#include <iostream>
#include <string>
#include <unordered_map>

#include "action.h"
#include "card.h"
#include "globparameters.h"

bool part_of_hand(card* hand_cards, int num_hand_cards, card* card_to_check);
bool check_valid_move(bool discard_phase, card *cards_played, int num_cards_played,
                    int sum_cards, card* crib, int num_cards_in_crib, card* player_hand, int num_cards_in_player_hand, 
                    action player_action);
bool compare_cards(card* c1, card* c2);
bool exsists_legal_move(card* hand_cards, int num_hand_cards, int sum_cards_played);
int update_legal_moves(int** available_actions, int* available_actions_indexes, card* hand_cards, int num_hand_cards, int sum_cards_played, bool discard_done);
int update_legal_moves_24_choose_1(int** available_actions, int* available_actions_indexes, card* hand_cards, int num_hand_cards, int sum_cards_played, bool discard_done);
void map_24_to_4(int* a, int* b, int* c, int* d, int index);

int* get_array(int i);
int* get_array(int i, int j);


class policy {
    private:

    public:
        std::string name = "nameless policy";
        std::unordered_map<std::string, std::vector<double>> infostates = std::unordered_map<std::string, std::vector<double>>();

        
        policy();
        policy(std::unordered_map<std::string, std::vector<double>*> cfr_infostates, int index);
        policy(std::unordered_map<std::string, std::vector<double>> in_infostates);

        bool part_of_policy(std::string key);
        int size();
        virtual std::vector<double> action_probabilities(std::string key);
        virtual std::vector<double> action_probabilities(std::string info_state_key, int num_available_actions);
        

        bool serialize(std::string filepath);
        bool deserialize(std::string filepath);

        virtual ~policy() {};
};

class randompolicy : public policy {
    public:
        
        randompolicy();
        virtual std::vector<double> action_probabilities(std::string key) override;
        virtual std::vector<double> action_probabilities(std::string info_state_key, int num_available_actions) override;

        ~randompolicy() {};
};

class determenisticpolicy : public policy {
    private:
        int play_index;
    public:
        determenisticpolicy(int in_play_index=0);
        virtual std::vector<double> action_probabilities(std::string key) override;
        virtual std::vector<double> action_probabilities(std::string info_state_key, int num_available_actions) override;

        ~determenisticpolicy() {};
};
