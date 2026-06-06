#include "mccfrplayer.h"

 

mccfrplayer::mccfrplayer() {
    init();
}

mccfrplayer::mccfrplayer(abstraction* in_game, double in_expl, std::mt19937* in_gen) {
    expl = in_expl;
    gen = in_gen;
    set_game(in_game);
    init();
}


void mccfrplayer::init() {
    num_players = 2; // game_get_num_players
}

void mccfrplayer::set_game(abstraction* in_game) {
    game = in_game;
}

void mccfrplayer::add_regret(std::string some_sort_of_key, int action_index, double regret, int player) {
    if (player == 1) {
        infostates[some_sort_of_key][REGRET_INDEX][action_index] += regret;
    } else {
        infostates_opp[some_sort_of_key][REGRET_INDEX][action_index] += regret;
    }
}

void mccfrplayer::add_avg_strategy(std::string some_sort_of_key, int action_index, double increment, int player) {
    if (increment < 0) {
        throw std::runtime_error("Trying to add negative increment");
    }
    if (player == 1) {
        infostates[some_sort_of_key][AVG_POLICY_INDEX][action_index] += increment;
    } else {
        infostates_opp[some_sort_of_key][AVG_POLICY_INDEX][action_index] += increment;
    }
}

std::vector<double>* mccfrplayer::lookup_infostate_info(std::string some_sort_of_key, int num_available_actions, int player) {
    //if key not found
    if (player == 1) {
        curr_infostates = &infostates;
    } else {
        curr_infostates = &infostates_opp;
    }
    if (curr_infostates->find(some_sort_of_key) == curr_infostates->end()) {
        (*curr_infostates)[some_sort_of_key] = new std::vector<double>[2]();
        
        std::generate_n(std::back_inserter((*curr_infostates)[some_sort_of_key][0]), num_available_actions, [] {return 1.0/1e6;});
        std::generate_n(std::back_inserter((*curr_infostates)[some_sort_of_key][1]), num_available_actions, [] {return 1.0/1e6;});
        
    } 

    return (*curr_infostates)[some_sort_of_key];
}

double mccfrplayer::episode(abstraction *state, int update_player, double player_reach, double opp_reach, double chance_reach, int start_point_diff) {
    if (state->is_terminal()) {
        return state->get_point_diff(update_player) - start_point_diff; //simulator expects player 1 or 2
    }

    if (state->is_chance_node()) {
        std::vector<double> probs = state->chance_outcomes();
        std::discrete_distribution<int> d(probs.begin(), probs.end());
        int sampled_action_index = d(*gen);
        state->apply_action_from_list(sampled_action_index);
        return episode(state, update_player, player_reach, opp_reach * probs[sampled_action_index], chance_reach * probs[sampled_action_index], start_point_diff);
    }



    int current_player = state->get_current_player();
    std::string info_state_key;
    int num_available_actions;
    double* policy;

    if (run_with_locked_opp && current_player != update_player) {
        info_state_key = opp_abstraction->get_information_abstraction()->get_informationstate_string(state->get_game(), current_player);
        num_available_actions = opp_abstraction->get_action_abstraction()->get_num_available_actions(state->get_game());
        std::vector<double> opp_policy = opponent->action_probabilities(info_state_key, num_available_actions);
        num_available_actions = opp_policy.size();
        policy = new double[num_available_actions];
        for (int i=0; i<opp_policy.size(); i++) {
            policy[i] = opp_policy[i];
        }
    } else {
        info_state_key = state->get_informationstate_string(current_player);
        num_available_actions = state->get_num_available_actions();
        
        if (num_available_actions == 0) {
            std::cout << "Sumthing wrong, no actions available" << std::endl; 
        }

        std::vector<double>* infostate_info = lookup_infostate_info(info_state_key, num_available_actions);
        policy = new double[num_available_actions];
        
        regret_matching(policy, infostate_info[REGRET_INDEX], num_available_actions);
    }
    
    
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

    int sampled_action_index = d(*gen); 
    if (run_with_locked_opp && current_player != update_player) {
        opp_abstraction->get_action_abstraction()->apply_action_from_list(state->get_game(), sampled_action_index);
    } else {
        state->apply_action_from_list(sampled_action_index);
    }
    
    double new_player_reach;
    double new_opp_reach;
    if (current_player == update_player) {
        new_player_reach = player_reach * policy[sampled_action_index];
        new_opp_reach = opp_reach;
    } else {
        new_player_reach = player_reach;
        new_opp_reach = opp_reach * policy[sampled_action_index];
    }
    double new_chance_reach = chance_reach * sample_strategy[sampled_action_index];

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
            double cf_action_value = child_values[i] * opp_reach / chance_reach;
            add_regret(info_state_key, i, cf_action_value - cf_value);
        }
        for (int i = 0; i < num_available_actions; i++) {
            double increment = player_reach * policy[i] / chance_reach;
            add_avg_strategy(info_state_key, i, increment);
        }
        
    }
    delete[] child_values;
    delete[] sample_strategy;
    delete[] policy;
    

    return value_estimate;
}   


double mccfrplayer::episode_different_abstractions(abstraction *state, int update_player, double player_reach, double opp_reach, double chance_reach, int start_point_diff) {
    if (state->is_terminal()) {
        return state->get_point_diff(update_player) - start_point_diff; //simulator expects player 1 or 2
    }

    int current_player = state->get_current_player();

    if (current_player == 1 && state->is_chance_node()) {
        std::vector<double> probs = state->chance_outcomes();
        std::discrete_distribution<int> d(probs.begin(), probs.end());
        int sampled_action_index = d(*gen);
        state->apply_action_from_list(sampled_action_index);
        return episode_different_abstractions(state, update_player, player_reach, opp_reach * probs[sampled_action_index], chance_reach * probs[sampled_action_index], start_point_diff);
    }

    if (current_player == 2 && opp_abstraction->get_action_abstraction()->is_chance_node(state->get_game())) {
        std::vector<double> probs = opp_abstraction->get_action_abstraction()->chance_outcomes(state->get_game());
        std::discrete_distribution<int> d(probs.begin(), probs.end());
        int sampled_action_index = d(*gen);
        opp_abstraction->get_action_abstraction()->apply_action_from_list(state->get_game(), sampled_action_index);
        return episode_different_abstractions(state, update_player, player_reach, opp_reach * probs[sampled_action_index], chance_reach * probs[sampled_action_index], start_point_diff);
    }

    std::string info_state_key;
    int num_available_actions;
    double* policy;
    if (current_player == 1) {
        info_state_key = state->get_informationstate_string(current_player);
        num_available_actions = state->get_num_available_actions();
    } else {
        info_state_key = opp_abstraction->get_information_abstraction()->get_informationstate_string(state->get_game(), current_player);
        num_available_actions = opp_abstraction->get_action_abstraction()->get_num_available_actions(state->get_game());
    }
    
    if (num_available_actions == 0) {
        std::cout << "Sumthing wrong, no actions available" << std::endl; 
    }

    std::vector<double>* infostate_info = lookup_infostate_info(info_state_key, num_available_actions, current_player);
    policy = new double[num_available_actions];
    
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

    int sampled_action_index = d(*gen); 
    if (current_player == 1) {
        state->apply_action_from_list(sampled_action_index);
    } else {
        opp_abstraction->get_action_abstraction()->apply_action_from_list(state->get_game(), sampled_action_index);
    }

    double new_player_reach;
    double new_opp_reach;
    if (current_player == update_player) {
        new_player_reach = player_reach * policy[sampled_action_index];
        new_opp_reach = opp_reach;
    } else {
        new_player_reach = player_reach;
        new_opp_reach = opp_reach * policy[sampled_action_index];
    }
    double new_chance_reach = chance_reach * sample_strategy[sampled_action_index];

    double child_value = episode_different_abstractions(state, update_player, new_player_reach, new_opp_reach, new_chance_reach, start_point_diff);

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
            double cf_action_value = child_values[i] * opp_reach / chance_reach;
            add_regret(info_state_key, i, cf_action_value - cf_value, current_player);
        }
        for (int i = 0; i < num_available_actions; i++) {
            double increment = player_reach * policy[i] / chance_reach;
            add_avg_strategy(info_state_key, i, increment, current_player);
        }
        
    }
    delete[] child_values;
    delete[] sample_strategy;
    delete[] policy;
    

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

void mccfrplayer::full_update_average(abstraction* state, double* reach_probabilities) {
    /*
     * Runs an iteration of external sampling.
     * Initial position is sampled, and both players moves are exsausthivly checked.
    */
    if (state->is_terminal()) {
        return;
    }
    double sum_reach = reach_probabilities[0] + reach_probabilities[1];
    if (sum_reach == 0) {
        return;
    }

    int current_player = state->get_current_player(); //1 or 2 -> 0 or 1
    int current_player_idx = current_player-1;
    std::string info_state_key = state->get_informationstate_string(current_player);
    int num_available_actions = state->get_num_available_actions();
    if (num_available_actions <= 0) {
        std::cout << "Sumthing wrong, no actions available" << std::endl; 
    }

    std::vector<double>* infostate_info = lookup_infostate_info(info_state_key, num_available_actions);

    double* policy = new double[num_available_actions];
    regret_matching(policy, infostate_info[REGRET_INDEX], num_available_actions);

    for (int a_idx = 0; a_idx < num_available_actions; a_idx++) {
        double new_reach_probs[2];
        new_reach_probs[0] = reach_probabilities[0];
        new_reach_probs[1] = reach_probabilities[1];
        new_reach_probs[current_player_idx] *= policy[a_idx];
        full_update_average(state->child(a_idx), new_reach_probs);
    }

    for (int a_idx = 0; a_idx < num_available_actions; a_idx++) {
        add_avg_strategy(info_state_key, a_idx, reach_probabilities[current_player_idx] * policy[a_idx]);    
    }

}

double mccfrplayer::update_regrets(abstraction* state, int player) {
    /*
     * Runs an episode of external sampling.
     * Returns the weighted average value of the state in the game
     * obtaine by the values of the children
    */
    if (state->is_terminal()) {
        return state->get_point_diff(player);
    }

    int current_player = state->get_current_player(); // returns either 1 or 2;
    std::string info_state_key = state->get_informationstate_string(current_player);
    int num_available_actions = state->get_num_available_actions();
    if (num_available_actions <= 0) {
        std::cout << "Sumthing wrong, no actions available" << std::endl; 
    }

    std::vector<double>* infostate_info = lookup_infostate_info(info_state_key, num_available_actions);

    double* policy = new double[num_available_actions];
    regret_matching(policy, infostate_info[REGRET_INDEX], num_available_actions);

    double value = 0.0;
    double* child_values = new double[num_available_actions];

    if (current_player != player) {
        std::discrete_distribution<int> d(policy, policy + num_available_actions);
        int sampled_action_index = d(*gen);
        value = update_regrets(state->child(sampled_action_index), player);
    } else {
        for (int a_idx=0; a_idx<num_available_actions; a_idx++) {
            child_values[a_idx] = update_regrets(state->child(a_idx), player);
            value += child_values[a_idx] * policy[a_idx];
        }
    }

    if (current_player == player) {
        for (int a_idx=0; a_idx<num_available_actions; a_idx++) {
            add_regret(info_state_key, a_idx, child_values[a_idx] - value);
        }
    }

    if (!AVERAGE_TYPE_FULL && current_player != player) {
        for (int a_idx = 0; a_idx < num_available_actions; a_idx++) {
            add_avg_strategy(info_state_key, a_idx, policy[a_idx]);
        }
        
    }
    delete[] child_values;
    return value;
}

void mccfrplayer::set_opp_policy(policy* opponen_policy, abstraction* in_opp_abstraction, bool locked_opp) {
    /*
     * Sets opponent policy to a locked policy
     * Changes internal variables to run with locked policy.
    */
    opponent = opponen_policy;
    opp_abstraction = in_opp_abstraction;
    opp_set = true;
    run_with_locked_opp = locked_opp;
}

void mccfrplayer::delete_opp_policy() {
    /*
     * Makes it so that training is done against itself
    */
    run_with_locked_opp = false;
    opp_set = false;
}


void mccfrplayer::iteration() {

    if (opp_set && !run_with_locked_opp) {
        for (int i=1; i<3; i++) {
            for (int j=0; j<2; j++) {
                int update_player = i;
                game->setup_round();
                if (opp_set) {
                    opp_abstraction->setup_round(true);
                }
                episode_different_abstractions(game, update_player, 1.0, 1.0, 1.0, game->get_point_diff(update_player)); //update chance_reach to be correct (maybe it is fine as long as the chance-reach-weight is the same)
            }
        }
    } else {
        int update_player = 1;
        curr_infostates = &infostates;        
        game->setup_round();
        if (opp_set) {
            opp_abstraction->setup_round(true);
        }
        episode(game, update_player, 1.0, 1.0, 1.0, game->get_point_diff(update_player));       
    }
}

void mccfrplayer::iteration_external() {
    /* 
     * Performs one iteration of external sampling.
     * An iteration consists of one episode for each player as the update player.
    */
    // for(int p=0; p < num_players; p++) {

    // }
    abstraction* new_state = game->new_initial_state();
    update_regrets(new_state, 1);
    delete new_state;
    if (AVERAGE_TYPE_FULL) {
      double* reach_probs = new double[num_players];
      for (int i=0; i < num_players; i++) {
        reach_probs[i] = 1.0;
      }
      abstraction* new_state2 = game->new_initial_state();
      full_update_average(new_state2, reach_probs);
      delete new_state2;
      delete[] reach_probs;
    }
}

policy* mccfrplayer::average_policy() {
    return new policy(infostates, AVG_POLICY_INDEX);
}

policy* mccfrplayer::average_policy_opp() {
    return new policy(infostates_opp, AVG_POLICY_INDEX);
}

std::unordered_map<std::string, std::vector<double>*>* mccfrplayer::get_infostates() {
    return &infostates;
}

double mccfrplayer::measure_exploitability(policy* policy_to_be_measured, abstraction* ab, double util_max_min, double p, double delta, double M) {
    /*
     * Measure exploitability by finding a best response, and measure utility against that best response.
    */
    set_opp_policy(policy_to_be_measured, ab);

    //hopefully some nice equation to give number of games needed to find BR with certainty better than epsilon
    //For now lets just do 10^6
    const int max_num_stagnant = 10;
    double rolling_avg_regret_lst[max_num_stagnant];
    double curr_min_regret = 1e20;
    double exploitability = 0;
    double epsilon = 0.005;
    int num_stagnant = 0;
    int num_iter_per_check = 100'000;
    int max_num_iterations = 1'000 * num_iter_per_check;
    for (int i=0; i < max_num_iterations/num_iter_per_check; i++) {
        for (int j=0; j < num_iter_per_check; j++) {
            iteration();
        }
        double regret_new = amount_of_avg_regret(num_iter_per_check*(i+1));
        rolling_avg_regret_lst[num_stagnant] = regret_new;
        if (regret_new + epsilon < curr_min_regret) {
            curr_min_regret = regret_new;
            num_stagnant = 0;
        } else {
            num_stagnant++;
        }

        if (num_stagnant >= max_num_stagnant) {
            std::cout << "Rolling avg regret the last 5 rounds have been: " << rolling_avg_regret_lst[max_num_stagnant-5] << " " << rolling_avg_regret_lst[max_num_stagnant-4] << " " << rolling_avg_regret_lst[max_num_stagnant-3] << " " << rolling_avg_regret_lst[max_num_stagnant-2] << " " << rolling_avg_regret_lst[max_num_stagnant-1] << " " << std::endl;
            break;
        }
        double regret_bound = util_max_min * (1 + sqrt(2) / sqrt(p)) * delta * sqrt(24) / (sqrt((i+1) *num_iter_per_check));
        std::cout << "Total regret should be bound by: " << regret_bound << std::endl;
        std::cout << "min avg regret after: " << num_iter_per_check*(i+1) << " rounds is: " << curr_min_regret << ". And avg regret is: " << regret_new << std::endl;
        if(curr_min_regret > regret_bound) {
            std::cout << "wtf! bound is lower than actual" << std::endl;
        }
            
        policy* br_policy = average_policy();

        int num_games = 10'000; //100 000 games should be more than enough to measure exploitability (I think)
        int point_diff = 0;

        for (int i = 0; i < num_games; i++) {
            ab->setup_round();
            while (!ab->is_terminal()) {
                std::string key = ab->get_informationstate_string(ab->get_current_player());
                int num_available_actions = ab->get_num_available_actions();
                int sampled_action_index;
                if (ab->get_current_player() == 1) {
                    std::vector<double> ac = policy_to_be_measured->action_probabilities(key, num_available_actions);
                    std::discrete_distribution<int> d(ac.begin(), ac.end());
                    sampled_action_index = d(*gen);
                } else {
                    std::vector<double> ac = br_policy->action_probabilities(key, num_available_actions);        
                    std::discrete_distribution<int> d(ac.begin(), ac.end());
                    sampled_action_index = d(*gen);
                }
                ab->apply_action_from_list(sampled_action_index);
            }
            point_diff += ab->get_point_diff(1); //point diff in favour of policy_to_be_measured        
        }
        exploitability = (double) point_diff/num_games;
        std::cout << "Total number of point difference: " << point_diff << " Over " << num_games << " number of games." << std::endl;
        std::cout << "The policy is exploitable: " << exploitability << " points per round, within its own abstraction" << std::endl;
        delete br_policy;
    }
    return exploitability;
    
}



double mccfrplayer::amount_of_avg_regret(int num_iter) {
    double regret = 0;
    for (auto state : infostates) {
        double max_regret_in_infostate = 0;
        for (double r : state.second[REGRET_INDEX]) {
            max_regret_in_infostate = std::max(max_regret_in_infostate, r);
        }
        regret += max_regret_in_infostate;
    }
    return regret / num_iter;
}


policy* mccfrplayer::get_br(policy* policy_to_beat, abstraction* opp_ab, br_abstraction* br_ab) {
    /*
     * iterates over all informationstates and finds a best response strategy.
    */
    for(int i=1; i<3; i++) {
        br_ab->reset(i);
        while(!br_ab->all_visited()) {
            br_abstraction* next_state_p1  = static_cast<br_abstraction*> (br_ab->new_initial_state());
            dfs(policy_to_beat, opp_ab, next_state_p1, 1, 1.0);
            delete next_state_p1;
        }
    }
    for (auto state : infostates) {
        double sum = 0;
        double best_value = -1000;
        int index = -1;
        for(int i=0; i< state.second[AVG_POLICY_INDEX].size(); i++) {
            sum += state.second[AVG_POLICY_INDEX][i];
            if(best_value < state.second[AVG_POLICY_INDEX][i]) {
                best_value = state.second[AVG_POLICY_INDEX][i];
                index = i;
            }
        }
        if (sum != 1.0) {
            throw std::runtime_error("mccfrplayer: get_br: The entries to the informationsset does not sum to 1 but instead to " + std::to_string(sum));
        }
        for(int i=0; i< state.second[AVG_POLICY_INDEX].size(); i++) {
            state.second[AVG_POLICY_INDEX][i] = 0.0;
        }
        state.second[AVG_POLICY_INDEX][index] = 1.0;
        
    }

    return average_policy();
}


double mccfrplayer::dfs(policy* policy_to_beat, abstraction* opp_ab, br_abstraction* state, int update_player, double opp_reach) {
    /*
     * Requires that state and opp_ab have the same number of actions per information state
    */
    if (state->is_terminal()) {
        return state->get_point_diff(update_player);
    }

    int current_player = state->get_current_player(); // returns either 1 or 2;
    
    double value = -1000.0;
    int index = -1;

    if (current_player != update_player) {        
        value = 0;
        int num_opp_hands = state->get_num_opp_hands(update_player);
        for(int opp_hand=0; opp_hand<num_opp_hands; opp_hand++) {
            state->set_opp_hand(opp_hand, update_player);
            if (!exsists_legal_move(state->get_game()->get_player_hand(current_player)->get_cards(), state->get_game()->get_player_hand_size(current_player), state->get_game()->get_sum_cards_played())) {
                double child_value = dfs(policy_to_beat, opp_ab, state->child(-1), update_player, opp_reach);
                //double check that action_probabilities sum to 1;
                value += child_value * opp_reach;
                continue;
            }
            std::vector<double> policy = policy_to_beat->action_probabilities(opp_ab->get_information_abstraction()->get_informationstate_string(state->get_game(), current_player));
            int num_available_actions = policy.size();
            
            double p_sum = 0;
            for (int i = 0; i < num_available_actions; i++) {
                p_sum += policy[i];
            }
            for (int i = 0; i < num_available_actions; i++) {
                policy[i] = policy[i] / p_sum;
            }

            for (int a_idx=0; a_idx<num_available_actions; a_idx++) {
                double new_opp_reach = policy[a_idx] * opp_reach;
                if (new_opp_reach > 0) {
                    double child_value = dfs(policy_to_beat, opp_ab, state->child(a_idx), update_player, new_opp_reach);
                    //double check that action_probabilities sum to 1;
                    value += child_value * new_opp_reach;
                }  
            }
        }
        value /= num_opp_hands;
    } else {
        std::string info_state_key = state->get_informationstate_string(current_player);
        int num_available_actions = state->get_num_available_actions();
        if (num_available_actions <= 0) {
            std::cout << "Sumthing wrong, no actions available" << std::endl; 
        }
        
        std::vector<double> *infostate = lookup_infostate_info(info_state_key, num_available_actions);
        for (double act : infostate[1]) {
            if (act != 1.0/1e6) {
                return act*opp_reach;
            }
        }
        
        double tmp_value = -1;
        
        for (int a_idx=0; a_idx<num_available_actions; a_idx++) {
            tmp_value = dfs(policy_to_beat, opp_ab, state->child(a_idx), update_player, opp_reach);
            if (tmp_value > value) {
                index = a_idx;
                value = tmp_value;
            }
        }
        value = value * opp_reach;
        add_avg_strategy(info_state_key, index, opp_reach);
    }

    return value;
}