#pragma once
#include <filesystem>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include "../simulator/cribbage.h"
#include "../simulator/greedyplayer.h"
#include "../simulator/mccfrplayer.h"
#include "../simulator/statistics.h"

TEST(meta, better_policy_will_be_chosen) {
    // GTEST_SKIP();
    std::mt19937 gen(1);

    std::string run_name = "meta_choose_best_policy";
    std::string filename = run_name +".bin"; 
    std::string log_name = run_name +"_log.txt";

    std::ofstream logger(log_name);

    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1);
    std::shared_ptr<simulator::cribbage> game_ptr = std::make_shared<simulator::cribbage>(game);

    hand_cards_choose_24 rp_abstraction = hand_cards_choose_24(game_ptr);
    new_stack_4_choose_1_abstraction ab = new_stack_4_choose_1_abstraction(game_ptr);

    greedypolicy gp = greedypolicy();
    randompolicy rp = randompolicy();
    
    meta_abstraction meta = meta_abstraction(game_ptr, 1);
    meta.add_policy(&gp, std::make_shared<new_stack_and_hand_cards>(), std::make_shared<play_card_from_hand>());
    meta.add_policy(&rp, std::make_shared<new_stack_and_hand_cards>(), std::make_shared<play_card_from_hand>());

    mccfrplayer mccfr = mccfrplayer(&meta, 0.06, &gen);

    for (int i = 0; i < 10'000; i++) {
        mccfr.iteration();
    }
    
    std::unordered_map<std::string, std::vector<double> *>* infostates = mccfr.get_infostates();
    int greedy_better = 0;
    int random_better = 0;
    int equal = 0;
    for(auto state : *infostates) {
        if (state.second[1][0] > state.second[1][1]) {
            greedy_better++;
        } else if (state.second[1][0] < state.second[1][1]) {
            random_better++;
        } else {
            equal++;
        }
    }
    GTEST_ASSERT_NE(equal, 0);
    GTEST_ASSERT_GT(greedy_better - random_better, -30) << "random is better than greedy " << random_better << " times." ;
    // GTEST_ASSERT_LT(greedy_better - random_better, 30) << "random is better than greedy " << random_better << " times." ;
    
    for (int i = 0; i < 100'000; i++) {
        mccfr.iteration();
    }
    

    


    //do same test with many random policies added, diluting greedypolicy.
    for (int i = 0; i < 18; i++) {
        meta.add_policy(&rp, std::make_shared<new_stack_and_hand_cards>(), std::make_shared<play_card_from_hand>());
    }

    mccfrplayer mccfr2 = mccfrplayer(&meta, 0.06, &gen);

    for (int i = 0; i < 100'000; i++) {
        mccfr2.iteration();
    }
    
    std::unordered_map<std::string, std::vector<double> *>* infostates2 = mccfr2.get_infostates();
    int greedy_better2 = 0;
    int random_better2 = 0;
    for(auto state : *infostates2) {
        if (state.second[1][0] > state.second[1][1]) {
            greedy_better2++;
        } else if (state.second[1][0] < state.second[1][1]) {
            random_better2++;
        }
    }
    GTEST_ASSERT_GT(greedy_better, greedy_better2);

    policy* pol1 = mccfr.average_policy();
    policy* pol2 = mccfr2.average_policy();
    
    statistics stats = statistics();

    double head_to_head = stats.head_to_head(&game, pol1, pol2, &meta, &meta, 10'000, &gen);
    GTEST_ASSERT_GT(head_to_head, 0);
}


TEST(meta, diverse_policies) {
    /*
     * Test/show that using diverse policies will lead to better performance than using non-diverse
    */
    // GTEST_SKIP();
    int seed = 1;
    std::mt19937 gen(seed);
    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1);
    std::shared_ptr<simulator::cribbage> game_ptr = std::make_shared<simulator::cribbage>(game);
    
    new_stack_4_choose_1_abstraction ab = new_stack_4_choose_1_abstraction(game_ptr);

    std::string filepath = "C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_108.bin";
    randompolicy rp = randompolicy();
    greedypolicy gp = greedypolicy();
    policy precomuted_policy = policy();
    precomuted_policy.deserialize(filepath);



    meta_abstraction meta1 = meta_abstraction(game_ptr, seed);
    meta_abstraction meta2 = meta_abstraction(game_ptr, seed);
    meta_abstraction meta3 = meta_abstraction(game_ptr, seed);
    
    meta1.add_policy(&rp, ab.get_information_abstraction(), ab.get_action_abstraction());
    meta1.add_policy(&gp, ab.get_information_abstraction(), ab.get_action_abstraction());

    meta2.add_policy(&rp, ab.get_information_abstraction(), ab.get_action_abstraction());
    meta2.add_policy(&precomuted_policy, ab.get_information_abstraction(), ab.get_action_abstraction());

    meta3.add_policy(&rp, ab.get_information_abstraction(), ab.get_action_abstraction());
    meta3.add_policy(&gp, ab.get_information_abstraction(), ab.get_action_abstraction());
    meta3.add_policy(&precomuted_policy, ab.get_information_abstraction(), ab.get_action_abstraction());

    mccfrplayer mccfr1 = mccfrplayer(&meta1, 0.06, &gen);
    mccfrplayer mccfr2 = mccfrplayer(&meta2, 0.06, &gen);
    mccfrplayer mccfr3 = mccfrplayer(&meta3, 0.06, &gen);

    int num_iter = 1'000'000;
    
    for(int i=0; i<num_iter; i++) {
        mccfr1.iteration();
    }
    for(int i=0; i<num_iter; i++) {
        mccfr2.iteration();
    }
    for(int i=0; i<num_iter; i++) {
        mccfr3.iteration();
    }

    std::unordered_map<std::string, std::vector<double> *>* infostates1 = mccfr1.get_infostates();
    std::unordered_map<std::string, std::vector<double> *>* infostates2 = mccfr2.get_infostates();
    std::unordered_map<std::string, std::vector<double> *>* infostates3 = mccfr3.get_infostates();
    int greedy_better = 0;
    int random_better = 0;
    for(auto state : *infostates1) {
        if (state.second[1][0] < state.second[1][1]) {
            greedy_better++;
        } else if (state.second[1][0] > state.second[1][1]) {
            random_better++;
        }
    }
    GTEST_ASSERT_GT(greedy_better, random_better) << "random is better than greedy " << random_better << " times." ;

    int policy_better = 0;
    random_better = 0;
    for(auto state : *infostates2) {
        if (state.second[1][0] < state.second[1][1]) {
            policy_better++;
        } else if (state.second[1][0] > state.second[1][1]) {
            random_better++;
        }
    }
    GTEST_ASSERT_GT(policy_better, random_better) << "random is better than greedy " << random_better << " times." ;

    greedy_better = 0;
    policy_better = 0;
    random_better = 0;
    int equal = 0;
    int what = 0;
    for(auto state : *infostates3) {
        if (state.second[1][0] == state.second[1][1] && state.second[1][1] == state.second[1][2]) {
            equal++;
        } else if (std::max(std::max(state.second[1][0], state.second[1][1]), state.second[1][2]) == state.second[1][0]) {
            random_better++;
        } else if (std::max(std::max(state.second[1][0], state.second[1][1]), state.second[1][2]) == state.second[1][1]) {
            greedy_better++;
        } else if (std::max(std::max(state.second[1][0], state.second[1][1]), state.second[1][2]) == state.second[1][2]) {
            policy_better++;
        } else {
            what++;
        }
    }
    // GTEST_ASSERT_NE(equal, 0);
    GTEST_ASSERT_EQ(what, 0);
    GTEST_ASSERT_GT(policy_better, random_better) << "random is better than policy " << random_better << " times." ;
    GTEST_ASSERT_GT(policy_better, greedy_better) << "greedy is better than policy " << greedy_better << " times." ;
    GTEST_ASSERT_GT(greedy_better, random_better) << "random is better than greedy " << random_better << " times." ;
    



    policy* pol1 = mccfr1.average_policy();
    policy* pol2 = mccfr2.average_policy();
    policy* pol3 = mccfr3.average_policy();
    
    statistics stats = statistics();

    double head_to_head12 = stats.head_to_head(&game, pol1, pol2, &meta1, &meta2, 100'000, &gen);
    double head_to_head13 = stats.head_to_head(&game, pol1, pol3, &meta1, &meta3, 100'000, &gen);
    double head_to_head23 = stats.head_to_head(&game, pol2, pol3, &meta2, &meta3, 100'000, &gen);
    
    GTEST_ASSERT_LT(head_to_head12, 0);
    GTEST_ASSERT_LT(head_to_head13, 0);

    GTEST_ASSERT_LT(head_to_head23, 0);

    GTEST_ASSERT_LT(head_to_head13, head_to_head12);

    
}



TEST(meta, better_than_the_sum_of_its_parts) {
    /*
     * Test/show that using diverse policies will lead to better performance than using non-diverse
    */
    GTEST_SKIP();
    int seed = 1;
    std::mt19937 gen(seed);
    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1);
    std::shared_ptr<simulator::cribbage> game_ptr = std::make_shared<simulator::cribbage>(game);
    
    new_stack_4_choose_1_abstraction ab = new_stack_4_choose_1_abstraction(game_ptr);

    randompolicy rp = randompolicy();
    greedypolicy gp = greedypolicy();



    meta_abstraction meta1 = meta_abstraction(game_ptr, seed);
    
    meta1.add_policy(&rp, ab.get_information_abstraction(), ab.get_action_abstraction());
    meta1.add_policy(&gp, ab.get_information_abstraction(), ab.get_action_abstraction());

    mccfrplayer mccfr1 = mccfrplayer(&meta1, 0.06, &gen);

    int num_iter = 1'000'000;
    
    for(int i=0; i<num_iter; i++) {
        mccfr1.iteration();
    }

    std::unordered_map<std::string, std::vector<double>*>* infostates1 = mccfr1.get_infostates();


    policy* pol1 = mccfr1.average_policy();
    
    statistics stats = statistics();

    double head_to_head_greedy_random = stats.head_to_head(&game, &gp, &rp, &ab, &ab, 10'000, &gen);
    double head_to_head_meta_random = stats.head_to_head(&game, pol1, &rp, &meta1, &ab, 10'000, &gen);
    double head_to_head_meta_greedy = stats.head_to_head(&game, pol1, &gp, &meta1, &ab, 10'000, &gen);
    
    GTEST_ASSERT_GT(head_to_head_greedy_random, 0);
    GTEST_ASSERT_GT(head_to_head_meta_random, 0);
    GTEST_ASSERT_GT(head_to_head_meta_greedy, 0);
    GTEST_ASSERT_GT(head_to_head_meta_random, head_to_head_meta_greedy);
    
}