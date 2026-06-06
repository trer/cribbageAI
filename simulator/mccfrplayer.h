#pragma once
#include <algorithm> 
#include <array>
#include <chrono>
#include <cmath>
#include <forward_list>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "abstraction.h"
#include "utils.h"
#include "cribbage.h"


class mccfrplayer {

    private:
        std::unordered_map<std::string, std::vector<double>*> infostates = std::unordered_map<std::string, std::vector<double>*>();
        std::unordered_map<std::string, std::vector<double>*> infostates_opp = std::unordered_map<std::string, std::vector<double>*>();
        std::unordered_map<std::string, std::vector<double>*>* curr_infostates = &infostates;

        int num_players;
        double expl = 0.06;
        abstraction *game;
        std::mt19937 *gen;

        policy* opponent;
        abstraction* opp_abstraction;
        
        bool AVERAGE_TYPE_FULL = true; // true = "FULL", false = "SIMPLE";
        const int REGRET_INDEX = 0;
        const int AVG_POLICY_INDEX = 1;


        void init();
        void add_regret(std::string some_sort_of_key, int idx, double regret, int player=1);
        void add_avg_strategy(std::string some_sort_of_key, int idx, double increment, int player=1);
        std::vector<double>* lookup_infostate_info(std::string some_sort_of_key, int num_available_actions, int player=1);
        double episode(abstraction *state, int update_player, double player_reach, double opp_reach, double chance_reach, int start_point_diff);
        double episode_different_abstractions(abstraction *state, int update_player, double player_reach, double opp_reach, double chance_reach, int start_point_diff);
        double* regret_matching(double* p, std::vector<double>& regrets, int num_legal_actions);
        double baseline_corrected_child_value(double child_value, int action_index, int sampled_action_index, double sample_probability);

        void full_update_average(abstraction* state, double* reach_probabilities);
        double update_regrets(abstraction* state, int player);

    public:
        bool opp_set = false;
        bool run_with_locked_opp = false;

        mccfrplayer();
        mccfrplayer(abstraction* in_game, double in_expl, std::mt19937* in_gen);
        
        void set_game(abstraction* in_game);
        void set_opp_policy(policy* opponent_policy, abstraction* opp_abstraction, bool locked_opp=true);
        void delete_opp_policy();
        void iteration();
        void iteration_external();
        policy* average_policy();
        policy* average_policy_opp();

        std::unordered_map<std::string, std::vector<double>*>* get_infostates();

        double measure_exploitability(policy* policy_to_be_measured, abstraction* ab, double util_max_min, double p, double delta, double M);
        double amount_of_avg_regret(int num_iter);
        policy* get_br(policy* policy_to_beat, abstraction* opp_ab, br_abstraction* br_abstraction);
        double dfs(policy* policy_to_beat, abstraction* state, br_abstraction* opp_ab, int player, double opp_reach);
        // void add_to_infoset(std::string key, int index, double opp_reach);
};  