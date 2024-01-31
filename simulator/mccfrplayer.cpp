#include "mccfrplayer.h"



policy::policy() {

}

policy::policy(std::unordered_map<std::string, std::vector<double>*> in_states, int index) {

    for (auto it : in_states) {
        
        // int length = it.second->size();
        
        infostates[it.first] = std::vector<double>(it.second[index]);
        // for (int i = 0; i < length; i++) {
        //     infostates[it.first][i] = it.second[index][i];
        // }
        
    }
    
}

policy::policy(std::unordered_map<std::string, std::vector<double>> in_infostates) {
    infostates = in_infostates;
}

std::vector<double> policy::action_probabilities(simulator::cribbage* state, int player) {
    std::string info_state_key = state->get_informationstate_string(player);
    int num_available_actions = state->get_num_available_actions();

    //if the infostate is not present in the strategy return a random move
    if (infostates.find(info_state_key) == infostates.end()) {
        infostates[info_state_key] = std::vector<double>();
        std::generate_n(std::back_inserter(infostates[info_state_key]), num_available_actions, [] {return 1.0;});
        for (int i = 0; i < num_available_actions; i++) {
            infostates[info_state_key][i] = 1.0/num_available_actions;
        }
        
    }
    
    return infostates[info_state_key];
}

void policy::serialize(std::string filepath) {
    std::ofstream file;
    file.open(filepath, std::ios::binary | std::ios::out);
    if (file.is_open()) {
        size_t length = infostates.size();
        size_t str_length;
        size_t vec_length;
        file.write(reinterpret_cast<char*>(&length), sizeof(length));
        for(auto it : infostates) {
            str_length = it.first.length();
            file.write(reinterpret_cast<char*>(&str_length), sizeof(str_length));
            file.write( it.first.c_str(), str_length);
            vec_length = it.second.size();
            file.write(reinterpret_cast<char*>(&vec_length), sizeof(vec_length));
            file.write(reinterpret_cast<char*>(&it.second[0]), vec_length * sizeof(double));
        }
        
        file.close();
    }
}

void policy::deserialize(std::string filepath) {
    std::ifstream file;
    file.open(filepath, std::ios::binary | std::ios::out);
    if (file.is_open()) {
        size_t length;
        
        

        file.read(reinterpret_cast<char*>(&length), sizeof length);

        for (int i = 0; i < length; i++) {
            size_t str_length;
            size_t vec_length;
            char* tmp_key;
            std::string tmp_key2;
            file.read(reinterpret_cast<char*>(&str_length), sizeof(str_length));
            tmp_key = new char[str_length+1];
            file.read(reinterpret_cast<char*>(&tmp_key[0]), str_length);
            tmp_key[str_length] = '\0';
            tmp_key2 = tmp_key;
            file.read(reinterpret_cast<char*>(&vec_length), sizeof(vec_length));
            infostates[tmp_key2] = std::vector<double>(vec_length);
            file.read(reinterpret_cast<char*>( &infostates[tmp_key2][0]), vec_length * sizeof(double));
        }
        

        // infostates["4_2_|"] = std::vector<double>(6);
        // infostates["4_1_|"] = std::vector<double>(6);

        // file.read(reinterpret_cast<char*>( &infostates["4_2_|"][0]), sizeof infostates["4_2_|"]);
        // infostates["4_2_|"].shrink_to_fit();

        // file.read(reinterpret_cast<char*>( &infostates["4_1_|"][0]), sizeof infostates["4_1_|"]);
        // infostates["4_2_|"].shrink_to_fit();

        file.close();
    }
}
 

mccfrplayer::mccfrplayer() {
    init();
}

mccfrplayer::mccfrplayer(simulator::cribbage* in_game, double in_expl, std::mt19937* in_gen) {
    expl = in_expl;
    gen = in_gen;
    set_game(in_game);
    init();
}


void mccfrplayer::init() {
    num_players = 2; // game_get_num_players
}

void mccfrplayer::set_game(simulator::cribbage* in_game) {
    game = in_game;
}

void mccfrplayer::add_regret(std::string some_sort_of_key, int action_index, double regret) {
    infostates[some_sort_of_key][REGRET_INDEX][action_index] += regret;
}

void mccfrplayer::add_avg_strategy(std::string some_sort_of_key, int action_index, double increment) {
    infostates[some_sort_of_key][AVG_POLICY_INDEX][action_index] += increment;
}

std::vector<double>* mccfrplayer::lookup_infostate_info(std::string some_sort_of_key, int num_available_actions) {
    //if key not found
    if (infostates.find(some_sort_of_key) == infostates.end()) {
        // create index
        // std::vector<double>* ss = new std::vector<double>[2]();
        // ss = new std::vector<double>[2]();
        // std::vector<double> s = std::vector<double>();
        infostates[some_sort_of_key] = new std::vector<double>[2]();
        // infostates[some_sort_of_key][0] = *(new std::vector<double>());
        // infostates[some_sort_of_key][1] = new std::vector<double>();
        
        std::generate_n(std::back_inserter(infostates[some_sort_of_key][0]), num_available_actions, [] {return 1.0/1e6;});
        std::generate_n(std::back_inserter(infostates[some_sort_of_key][1]), num_available_actions, [] {return 1.0/1e6;});
        // for (int i = 0; i < num_available_actions; i++)
        // {
        //     infostates[some_sort_of_key][0][i] = 1.0 / 1e6;
        //     infostates[some_sort_of_key][1][i] = 1.0 / 1e6;
        // }
        
    } 

    return infostates[some_sort_of_key];
}

double mccfrplayer::episode(simulator::cribbage *state, int update_player, double player_reach, double opp_reach, double chance_reach, int start_point_diff) {
    if (state->is_round_done()) {
        return state->get_point_diff(update_player) - start_point_diff; //simulator expects player 1 or 2
    }

    // if (state.is_chance_node()) {
    //    pass
    // }
    // I want to skip passed chance nodes to save time.
    // As long as the cut card is not modelled this should be fine
    // But I will have to calculate what the chance_reach is at the start of the round

    int current_player = state->get_current_player();
    std::string info_state_key = state->get_informationstate_string(current_player);
    int* legal_actions = state->get_available_actions();
    int num_available_actions = state->get_num_available_actions();

    std::vector<double>* infostate_info = lookup_infostate_info(info_state_key, num_available_actions);

    double* policy = new double[num_available_actions];
    regret_matching(policy, infostate_info[REGRET_INDEX], num_available_actions);

    double* sample_strategy = new double[num_available_actions];
    if (current_player == update_player) {
        for (int i = 0; i < num_available_actions; i++) {
            sample_strategy[i] = expl * 1/num_available_actions + (1-expl) * policy[i];
        }
    } else {
        for (int i = 0; i < num_available_actions; i++) {
            sample_strategy[i] = policy[i];
        }
    }

    std::discrete_distribution<int> d(sample_strategy, sample_strategy + num_available_actions);

    int sampled_action_index = d(*gen); // replace with mersene twister
    state->apply_action_from_list(sampled_action_index);
    
    double new_player_reach;
    double new_opp_reach;
    if (current_player == update_player) {
        new_player_reach = player_reach * policy[sampled_action_index];
        new_opp_reach = opp_reach;
    } else {
        new_player_reach = player_reach;
        new_opp_reach = opp_reach * policy[sampled_action_index];
    }
    double new_chance_reach = sample_strategy[sampled_action_index];

    double child_value = episode(state, update_player, new_player_reach, new_opp_reach, new_chance_reach, start_point_diff);

    double* child_values = new double[num_available_actions];
    for (int i = 0; i < num_available_actions; i++) {
        child_values[i] = baseline_corrected_child_value(child_value, i, sampled_action_index, sample_strategy[i]);
    }
    double value_estimate = 0;
    for (int i = 0; i < num_available_actions; i++) {
        value_estimate += policy[i] * child_values[i];
    }

    if (current_player == update_player) {
        double cf_value = value_estimate * opp_reach / chance_reach;

        for (int i = 0; i < num_available_actions; i++) {
            double cf_action_value  = child_values[i] * opp_reach / chance_reach;
            add_regret(info_state_key, i, cf_action_value - cf_value);
        }
        for (int i = 0; i < num_available_actions; i++) {
            double increment = player_reach * policy[i] / chance_reach;
            add_avg_strategy(info_state_key, i, increment);
        }
        
    }
    delete child_values;
    delete sample_strategy;
    delete policy;


    

    return value_estimate;
    

}   

double* mccfrplayer::regret_matching(double* positive_regret, std::vector<double>& regrets, int num_legal_actions) {
    double sum_pos_regret = 0;
    for (int i = 0; i < num_legal_actions; i++) {
        positive_regret[i] = std::max(regrets[i], 0.0);
        sum_pos_regret += std::max(regrets[i], 0.0);
    }
    

    if (sum_pos_regret <= 0.0) {
        for (int i = 0; i < num_legal_actions; i++)
        {
            positive_regret[i] = 1/num_legal_actions;
        }
    } else {
        for (int i = 0; i < num_legal_actions; i++) {
            positive_regret[i] /= sum_pos_regret;
        }
    }
    return positive_regret;
}

double mccfrplayer::baseline_corrected_child_value(double child_value, int action_index, int sampled_action_index, double sample_probability) {
    double baseline = 0; // get baseline here if I ever create one

    if (action_index == sampled_action_index) {
        return baseline + (child_value - baseline) / sample_probability;
    } else {
        return baseline;
    }
}

void mccfrplayer::full_update_average(node* state, double* reach_probabilities) {
    // pass
}

double mccfrplayer::update_regrets(node* state, int player) {
    return 0.0;
}


void mccfrplayer::iteration() {
    for (int update_player = 1; update_player < num_players+1; update_player++) {
        int negate = -(2*(update_player-1)-1); // if player 1 = 1, if player 2 = -1
        game->reset();
        game->setup_round();
        game->skip_to_play_phase();
        episode(game, update_player, 1.0, 1.0, 1.0, negate * (game->get_player1_score() - game->get_player2_score())); //update chance_reach to be correct (maybe it is fine as long as the chance-reach-weight is the same)
    }
}


policy* mccfrplayer::average_policy() {
    std::cout << infostates.size() << std::endl;;
    return new policy(infostates, AVG_POLICY_INDEX);
}

std::unordered_map<std::string, std::vector<double>*>* mccfrplayer::get_infostates() {
    return &infostates;
}