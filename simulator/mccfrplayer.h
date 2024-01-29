#pragma once
#include <algorithm> 
#include <array>
#include <chrono>
#include <cmath>
#include <forward_list>
#include <iterator>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "player.h"
#include "utils.h"
#include "cribbage.h"
#include "cheatingutcplayer.h"



class policy {
    private:
        std::unordered_map<std::string, std::vector<double>> infostates;

    public:
        policy();
        policy(std::unordered_map<std::string, std::vector<double>*> cfr_infostates, int index);
        policy(std::unordered_map<std::string, std::vector<double>> in_infostates);

        std::vector<double> action_probabilities(simulator::cribbage* state, int player);
};

class mccfrplayer {

    private:
        std::unordered_map<std::string, std::vector<double>*> infostates;
        int num_players;
        double expl = 0.06;
        simulator::cribbage *game;
        std::mt19937 *gen;

        
        const int REGRET_INDEX = 0;
        const int AVG_POLICY_INDEX = 1;


        void init();
        void add_regret(std::string some_sort_of_key, int idx, double regret);
        void add_avg_strategy(std::string some_sort_of_key, int idx, double increment);
        std::vector<double>* lookup_infostate_info(std::string some_sort_of_key, int num_available_actions);
        double episode(simulator::cribbage *state, int update_player, double player_reach, double opp_reach, double chance_reach);
        double* regret_matching(double* p, std::vector<double>& regrets, int num_legal_actions);
        double baseline_corrected_child_value(double child_value, int action_index, int sampled_action_index, double sample_probability);

        void full_update_average(node* state, double* reach_probabilities);
        double update_regrets(node* state, int player);

    public:
        mccfrplayer();
        mccfrplayer(simulator::cribbage* in_game, double in_expl, std::mt19937* in_gen);
        
        void set_game(simulator::cribbage* in_game);
        void iteration();
        policy* average_policy();

        std::unordered_map<std::string, std::vector<double>*>* get_infostates();
};