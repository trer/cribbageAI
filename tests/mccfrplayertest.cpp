#pragma once
#include <filesystem>
#include <gtest/gtest.h>

#include "../simulator/cribbage.h"
#include "../simulator/greedyplayer.h"
#include "../simulator/mccfrplayer.h"


TEST(mccfrplayer, run_test) {
    std::mt19937 gen(1);
    
    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1);
    int num_copies = 1;

    mccfrplayer mccfr = mccfrplayer(&game, 0.06, &gen);
    
    std::unordered_map<std::string, std::vector<double>*>* infostates = mccfr.get_infostates();

    for (int i = 0; i < 10; i++){
        mccfr.iteration();
    }
    
    
    game.reset();
    game.setup_round();

    policy* p = mccfr.average_policy();

    while (!game.is_playphase_done()) {
        std::vector<double> ac = p->action_probabilities(game.get_informationstate_string(game.get_current_player()), game.get_num_available_actions());
        std::discrete_distribution<int> d(ac.begin(), ac.end());
        
        // std::cout << "Current player is: " << game.get_current_player() << std::endl;
        // std::cout << "infoset is: " << game.get_informationstate_string(game.get_current_player()) << std::endl;
        // std::cout << "number of available actions: " << game.num_available_actions << std::endl;
        int i = 0;
        for (auto prob : ac) {
            GTEST_ASSERT_GT(prob, 0.0); //assert that there is a chance for each move
            // std::cout << "Action " << i++ << " with prob: " << prob << std::endl;
        }
        
        int sampled_action_index = d(gen); // replace with mersene twister
        game.apply_action_from_list(sampled_action_index);
        
    }

    for (int i = 0; i < 100; i++) {
        game.reset();
        game.setup_round();
        std::vector<double> ac = p->action_probabilities(game.get_informationstate_string(1), game.get_num_available_actions());
        for (auto prob : ac) {
            GTEST_ASSERT_EQ(prob, 1.0/15);
        }
        
    }  
}


TEST(mccfrplayer, maintest) {
    std::string filename = "mccfr_policy_10.bin"; 
    std::mt19937 gen(1);
    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(2, &p1, &p2);
    mccfrplayer mccfr = mccfrplayer(&game, 0.06, &gen);

    int num_iter = 10;//0000
    for (int i = 0; i < num_iter; i++) {
        mccfr.iteration();
    }
    policy* p = mccfr.average_policy();
    p->serialize(filename);

    int num_games = 10;
    int greedypoints = 0;
    int mccfrpoints = 0;
    int win = 0;
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_games; i++)
    {
        game.reset();
        game.setup_round();
        game.skip_to_play_phase();
        
        while (!game.is_playphase_done()) {
            if (game.get_current_player() == 1) {
                std::vector<double> ac1 = p->action_probabilities(game.get_informationstate_string(1), game.get_num_available_actions());
                std::discrete_distribution<int> d(ac1.begin(), ac1.end());
                int sampled_action_index = d(gen);
                win = game.apply_action_from_list(sampled_action_index);
            } else {
                game.set_action_for_player(2);
                win = game.resolve_action();
                if (game.get_num_available_actions() == 0 && !game.is_playphase_done()) {
                    win = game.apply_action_from_list(-1);
                }   
            }
            if (win != 0) {
                std::cout << "hold up" << std::endl;
            }
        }
        game.matching_setup();
        game.matching();

        mccfrpoints += game.get_player1_score();
        greedypoints += game.get_player2_score();
    }


}

TEST(mccfrplayer, performance_test) {
    GTEST_SKIP(); //We don't need to check the performance each time (ain't nobody got time for that)
    int num_training_iter = 10000;
    
    std::mt19937 gen(1);
    
    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1);
    int num_copies = 1;

    mccfrplayer mccfr = mccfrplayer(&game, 0.06, &gen);
    
    std::unordered_map<std::string, std::vector<double>*>* infostates = mccfr.get_infostates();

    std::chrono::steady_clock::time_point training_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_training_iter; i++){
        mccfr.iteration();
    }
    std::chrono::steady_clock::time_point training_end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(training_end - training_start);
    GTEST_ASSERT_LT((double) dur.count()/num_training_iter, 10.0);
    
    game.reset();
    game.setup_round();

    policy* p = mccfr.average_policy();

    for (int i = 0; i < 100; i++) {
        game.reset();
        game.setup_round();
        std::vector<double> ac = p->action_probabilities(game.get_informationstate_string(1), game.get_num_available_actions());
        for (auto prob : ac) {
            GTEST_ASSERT_EQ(prob, 1.0/15);
        }
        
    } 
    std::string filename = "mcf_runtest.bin";
    std::chrono::steady_clock::time_point start_time = std::chrono::high_resolution_clock::now();
    p->serialize(filename);
    std::chrono::steady_clock::time_point saved = std::chrono::high_resolution_clock::now();
    
    
    policy loaded_policy = policy();
    loaded_policy.deserialize(filename);
    std::chrono::steady_clock::time_point loaded = std::chrono::high_resolution_clock::now();
    auto save_duration = std::chrono::duration_cast<std::chrono::milliseconds>(saved - start_time);
    auto load_duration = std::chrono::duration_cast<std::chrono::milliseconds>(loaded - saved);
    std::cout << "Time to save is: " << save_duration.count() << " milliseconds." << std::endl;
    std::cout << "Time to load is: " << load_duration.count() << " milliseconds." << std::endl;

    GTEST_ASSERT_LT(load_duration.count() / loaded_policy.infostates.size(), 1.0/100);
    
    //assert that the loaded action probabilities are the same as the saved ones
    for (int i = 0; i < 100; i++) {
        game.reset();
        game.setup_round();
        while (!game.is_playphase_done())
        {
            std::vector<double> ac1 = p->action_probabilities(game.get_informationstate_string(1), game.get_num_available_actions());
            std::vector<double> ac2 = loaded_policy.action_probabilities(game.get_informationstate_string(1), game.get_num_available_actions());
            for(int i=0; i < ac1.size(); i++) {
                GTEST_ASSERT_EQ(ac1[i], ac2[i]);
            }
            std::discrete_distribution<int> d(ac2.begin(), ac2.end());
            int sampled_action_index = d(gen); // replace with mersene twister
            game.apply_action_from_list(sampled_action_index);
        }  
    } 

    std::filesystem::remove(filename.c_str());
}


TEST(mccfrplayer, policy_serialize) {

    policy p = policy();
    std::string filename = "test.bin";

    p.infostates["4_2_|"] = std::vector<double>();
    p.infostates["4_2_|"].push_back(4.2);
    p.infostates["4_2_|"].push_back(4.3);
    
    p.infostates["4_1_|"] = std::vector<double>();
    p.infostates["4_1_|"].push_back(3.2);
    p.infostates["4_1_|"].push_back(3.3);

    p.serialize(filename);

    policy p2 = policy();

    p2.deserialize(filename);

    GTEST_ASSERT_EQ(p2.infostates["4_2_|"][0], 4.2);
    GTEST_ASSERT_EQ(p2.infostates["4_2_|"][1], 4.3);

    GTEST_ASSERT_EQ(p2.infostates["4_1_|"][0], 3.2);
    GTEST_ASSERT_EQ(p2.infostates["4_1_|"][1], 3.3);

    std::filesystem::remove(filename.c_str());
    
}