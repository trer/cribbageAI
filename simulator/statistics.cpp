#include "statistics.h"


double statistics::head_to_head(simulator::cribbage* game, policy* p1, policy* p2, abstraction* ab1, abstraction* ab2, int num_games_total, std::mt19937* gen, std::ofstream* logger, player* discard_policy, int num_repeat) {
    /*
     * Returns the head to head exploitability of a policy.
     * Returns a number in favour of policy p1.
     * Eg. a head to head of 0.3 means that p1 gains 0.3 more points than p2 on average.
    */

    std::vector<int> point_diffs = std::vector<int>();
    point_diffs.reserve(num_games_total * sizeof(int));

    std::vector<double> means = std::vector<double>();
    std::vector<int> p1_scores = std::vector<int>();
    std::vector<int> p2_scores = std::vector<int>();

    if (logger != nullptr) {
        *logger << "\nTesting " << p1->name << " vs. " << p2->name << "!" << std::endl;
        if (discard_policy != nullptr) {
            *logger << "\nTesting players over " << num_games_total << " games, with a " << discard_policy->get_name() << "as the discard policy." << std::endl;
        } else {
            *logger << "\nTesting players over " << num_games_total << " games, with a randompolicy as the discard policy." << std::endl;
        }
    }

    auto start = std::chrono::high_resolution_clock::now();

    double mean_total = 0.0;
    int num_games_per_test = num_games_total/num_repeat;
    for (int n=0; n<num_repeat; n++) {
        policy* current_policy;
        abstraction* current_abstraction;
        double mean = 0.0;
        int p1_points = 0;
        int p2_points = 0;
        for (int i = 0; i < num_games_per_test; i++) {
            game->reset(set_dealer);
            game->setup_round();
            ab1->setup_round(true);
            ab2->setup_round(true);
            game->skip_to_play_phase();
            while (!game->is_playphase_done()) {
                if (game->get_current_player() == 1) {
                    current_policy = p1;
                    current_abstraction = ab1;
                } else {
                    current_policy = p2;
                    current_abstraction = ab2;
                }
                if (current_abstraction->get_action_abstraction()->is_chance_node(game)) {
                    std::vector<double> probs = current_abstraction->get_action_abstraction()->chance_outcomes(game);
                    std::discrete_distribution<int> d(probs.begin(), probs.end());
                    int sampled_action_index = d(*gen);
                    current_abstraction->get_action_abstraction()->apply_action_from_list(game, sampled_action_index);
                } else {
                    std::string key = current_abstraction->get_information_abstraction()->get_informationstate_string(game, game->get_current_player());
                    int num_available_actions = current_abstraction->get_action_abstraction()->get_num_available_actions(game);
                    std::vector<double> ac = current_policy->action_probabilities(key, num_available_actions);
                    std::discrete_distribution<int> d(ac.begin(), ac.end());
                    int sampled_action_index = d(*gen);
                    current_abstraction->get_action_abstraction()->apply_action_from_list(game, sampled_action_index);
                }
            }
            mean += (double) game->get_point_diff(1);
            p1_points += game->get_player1_score();
            p2_points += game->get_player2_score();
            point_diffs.push_back(game->get_point_diff(1));
        }
        mean_total += mean;
        mean /= num_games_per_test;
        
        means.push_back(mean);
        p1_scores.push_back(p1_points);
        p2_scores.push_back(p2_points);

    }
    mean_total /= num_games_total;

    double max_diff = 0.0;
    for (double m: means) {
        max_diff = std::max(max_diff, std::abs(m - mean_total));
    }
    

    double deviation = 0.0;
    for (double sample: point_diffs) {
        double tmp = sample - mean_total;
        deviation += tmp * tmp;        
    }
    deviation /= (num_games_total - 1);
    deviation = std::sqrt(deviation);

    //assumption is that the two policies are equally good
    double t_value = (mean_total - 0.0) / (deviation/std::sqrt(num_games_total));
    double confidence = (1.96 * deviation) / std::sqrt(num_games_total);
    double very_confidence =  (2.576 * deviation) / std::sqrt(num_games_total);
    

    int p1_score_total = 0;
    for(int p1_score : p1_scores) {
        p1_score_total += p1_score;
    }    
    
    int p2_score_total = 0;
    for(int p2_score : p2_scores) {
        p2_score_total += p2_score;
    }


    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    if (logger != nullptr) {
        *logger << "player1 size: " << p1->size() << " information sets" << std::endl;
        *logger << "player2 size: " << p2->size() << " information sets" << std::endl;
        *logger << "player1 points: " << p1_score_total << "  player2 points: " << p2_score_total << std::endl;
        *logger << "player1 / total: " << (double) p1_score_total / (p2_score_total + p1_score_total) << std::endl;
        *logger << "player2 / total: " << (double) p2_score_total / (p2_score_total + p1_score_total) << std::endl;
        *logger << "P1 expected score vs p2 is: " << mean_total << " points." << std::endl;
        *logger << "The largest deviation over " << num_repeat << " tests is: " << max_diff << std::endl;
        *logger << "Mean over " << num_repeat << " test with " << num_games_per_test << " iterations each is: " << std::endl;
        *logger << "[";
        for (double m: means) {
            *logger << m << ", ";
        }
        *logger << "]" << std::endl;

        *logger << "Standard deviation is: " << deviation << std::endl;
        *logger << "T-score is: " << t_value << std::endl;
        *logger << "95% confidence interval is: +- " << confidence << std::endl; 
        *logger << "99% confidence interval is: +- " << very_confidence << std::endl; 
        *logger << "Duration in milliseconds: " << duration.count() << std::endl;
        *logger << "With " << num_games_total << " rounds, that is " << (double) duration.count() / num_games_total << " milliseconds per round!" << std::endl;
        logger->flush();
    }

    return mean_total;
}   