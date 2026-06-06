#pragma once
#include <filesystem>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include "../simulator/cribbage.h"
#include "../simulator/greedyplayer.h"
#include "../simulator/mccfrplayer.h"
#include "../simulator/statistics.h"


TEST(mccfrplayer, run_test) {
    // GTEST_SKIP();
    std::mt19937 gen(1);
    
    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1);
    std::shared_ptr<simulator::cribbage> game_ptr(&game);
    abstraction* ab = new new_stack_4_choose_1_abstraction(game_ptr);
    int num_copies = 1;

    mccfrplayer mccfr = mccfrplayer(ab, 0.06, &gen);
    
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
    // GTEST_SKIP();
    std::string filename = "mccfr_policy_10.bin"; 
    std::mt19937 gen(1);
    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(2, &p1, &p2);
    std::shared_ptr<simulator::cribbage> game_ptr(&game);
    abstraction* ab = new new_stack_4_choose_1_abstraction(game_ptr);
    mccfrplayer mccfr = mccfrplayer(ab, 0.06, &gen);

    int num_iter = 10;//0000
    for (int i = 0; i < num_iter; i++) {
        mccfr.iteration();
    }
    policy* p = mccfr.average_policy();
    p->serialize(filename);

    int num_games = 100;
    int greedypoints = 0;
    int mccfrpoints = 0;
    int win = 0;
    
    for (int i = 0; i < num_games; i++)
    {
        game.reset();
        game.setup_round();
        game.skip_to_play_phase();
        
        while (!game.is_playphase_done()) {
            // game.set_action_for_player(game.get_current_player());
            // win = game.resolve_action();
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
        //game.matching_setup();
        //game.matching();

        mccfrpoints += game.get_player1_score();
        greedypoints += game.get_player2_score();
    }

    GTEST_ASSERT_TRUE((double) mccfrpoints/ greedypoints > 0.8);
    GTEST_ASSERT_TRUE((double) mccfrpoints/ greedypoints < 9);

}

TEST(mccfrplayer, permutation_player_performance) {
    GTEST_SKIP(); //We don't need to check the performance each time (ain't nobody got time for that)
    int num_training_iter = 10000;
    
    std::mt19937 gen(1);
    
    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1);
    std::shared_ptr<simulator::cribbage> game_ptr(&game);
    abstraction* ab = new hand_cards_choose_24(game_ptr);
    int num_copies = 1;

    mccfrplayer mccfr = mccfrplayer(ab, 0.06, &gen);
    
    std::unordered_map<std::string, std::vector<double>*>* infostates = mccfr.get_infostates();

    std::chrono::steady_clock::time_point training_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_training_iter; i++){
        mccfr.iteration();
    }
    std::chrono::steady_clock::time_point training_end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(training_end - training_start);
    GTEST_ASSERT_LT((double) dur.count()/num_training_iter, 10.0);
    
}

TEST(mccfrplayer, performance_test) {
    GTEST_SKIP(); //We don't need to check the performance each time (ain't nobody got time for that)
    int num_training_iter = 10000;
    
    std::mt19937 gen(1);
    
    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1);
    abstraction* ab = new new_stack_4_choose_1_abstraction(std::shared_ptr<simulator::cribbage>(&game));
    int num_copies = 1;

    mccfrplayer mccfr = mccfrplayer(ab, 0.06, &gen);
    
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
            std::vector<double> ac1 = p->action_probabilities(game.get_informationstate_string(game.get_current_player()), game.get_num_available_actions());
            std::vector<double> ac2 = loaded_policy.action_probabilities(game.get_informationstate_string(game.get_current_player()), game.get_num_available_actions());
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


TEST(mccfrplayer, hand_cards_choose_24_test) {
    // GTEST_SKIP();// Using a deck which should have allways give the same cards, letting causing the same infoset to be encountered several times
    std::string filename = "hand_cards_choose_24_test.bin";
    std::mt19937 gen(1);

    card player_and_opp_hand[13];

    //player hand
    player_and_opp_hand[0] = card(3, 'C');
    player_and_opp_hand[1] = card(3, 'D');
    player_and_opp_hand[2] = card(1, 'D');
    player_and_opp_hand[3] = card(2, 'C');
    player_and_opp_hand[4] = card(10, 'S');
    player_and_opp_hand[5] = card(11, 'S');
    //opp hand
    player_and_opp_hand[6] = card(4, 'H');
    player_and_opp_hand[7] = card(4, 'S');
    player_and_opp_hand[8] = card(1, 'S');
    player_and_opp_hand[9] = card(1, 'H');
    player_and_opp_hand[10] = card(11, 'H');
    player_and_opp_hand[11] = card(7, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');
    
    randomplayer p1 = randomplayer();
    randomplayer p2 = randomplayer();
    mock_deck d = mock_deck(); //check num_cards
    d.set_top_13_cards(player_and_opp_hand);
    int dealer = 1;
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, dealer, &d);
    abstraction* ab = new hand_cards_choose_24(std::shared_ptr<simulator::cribbage>(&game));
    std::unordered_map<std::string, int> map;
    int num_available_actions = 0;
    for (int i = 0; i < num_available_actions; i++) {
        for (int j = 0; j < num_available_actions; j++) {
            ab->setup_round();
            dealer = 3 - dealer;
            GTEST_ASSERT_EQ(game.get_current_pone(), dealer);
            ab->apply_action_from_list(i, false);
            ab->apply_action_from_list(j, false);
            card* cs = game.get_all_cards_played();
            std::string card_string = "";
            for (int k=0; k<8; k++) {
                card_string.append(std::to_string(cs[k].get_value(true)) + cs[k].get_suit(false) + "_");
            }
            if (map.find(card_string) == map.end()) {
                map[card_string] = 1;
            } else {
                map[card_string]++;
            }
        }
    }
    int count = 0;
    for (auto it : map) {
        count++;
        GTEST_ASSERT_EQ(it.second, 1) << it.first <<  " count is: " << count;
    }
    


    mccfrplayer mccfr = mccfrplayer(ab, 0.06, &gen);
    

    mccfr.iteration();

    for (int i = 0; i < 1000; i++) {
        mccfr.iteration();
    }
    

    
    game.reset();
    game.setup_round();

    policy* p = mccfr.average_policy();

    while (!game.is_playphase_done()) {
        std::vector<double> ac = p->action_probabilities(game.get_informationstate_string(game.get_current_player()), game.get_num_available_actions());
        std::discrete_distribution<int> d(ac.begin(), ac.end());
        
        int i = 0;
        for (auto prob : ac) {
            GTEST_ASSERT_GT(prob, 0.0); //assert that there is a chance for each move
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
        game.skip_to_play_phase();
        ac = p->action_probabilities(ab->get_informationstate_string(1), ab->get_num_available_actions());
        for (auto prob : ac) {
            GTEST_ASSERT_NE(prob, 1.0/24);
            GTEST_ASSERT_NE(prob, 1.0/1e7);
            GTEST_ASSERT_NE(prob, 1.0/1e6);
        }
    }  

    deck d2 = deck();
    card new_cards[13];
    for (int i=0; i<13; i++ ){
        new_cards[i] = *d2.deal_top();
    }
    d.set_top_13_cards(new_cards);
    ab->setup_round();
    std::vector<double> action_probabilities = p->action_probabilities(ab->get_informationstate_string(1), ab->get_num_available_actions());
    for (auto prob : action_probabilities) {
        GTEST_ASSERT_EQ(prob, 1.0/24);
    }

    game.set_deck(&d2);
    game.reset();
    game.setup_round();
    game.skip_to_play_phase();

    policyplayer pp = permutationpolicyplayer(p, 0);
    game.set_player(&pp, 1);
    int win = 0;
    for (int i=0; i < 8; i++) {
        game.set_action_for_player(game.get_current_player());
        game.resolve_action();
    }
    GTEST_ASSERT_TRUE(game.is_playphase_done());

    p->serialize(filename);
    
    game.reset();
    game.setup_round();
    game.skip_to_play_phase();

    permutationpolicyplayer pp2 = permutationpolicyplayer(filename, 0);
    game.set_player(&p2, 1);

    for (int i=0; i < 8; i++) {
        game.set_action_for_player(game.get_current_player());
        game.resolve_action();
    }
    GTEST_ASSERT_TRUE(game.is_playphase_done());
    
}



TEST(mccfrplayer, high_low_abstraction) {
    // GTEST_SKIP();// Using a deck which should have allways give the same cards, letting causing the same infoset to be encountered several times
    std::mt19937 gen(1);

    card player_and_opp_hand[13];

    //player hand
    player_and_opp_hand[0] = card(3, 'C');
    player_and_opp_hand[1] = card(3, 'D');
    player_and_opp_hand[2] = card(1, 'D');
    player_and_opp_hand[3] = card(2, 'C');
    player_and_opp_hand[4] = card(10, 'S');
    player_and_opp_hand[5] = card(11, 'S');
    //opp hand
    player_and_opp_hand[6] = card(4, 'H');
    player_and_opp_hand[7] = card(4, 'S');
    player_and_opp_hand[8] = card(1, 'S');
    player_and_opp_hand[9] = card(1, 'H');
    player_and_opp_hand[10] = card(11, 'H');
    player_and_opp_hand[11] = card(7, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');
    
    randomplayer p1 = randomplayer();
    randomplayer p2 = randomplayer();
    mock_deck d = mock_deck();
    d.set_top_13_cards(player_and_opp_hand);
    int dealer = 1;
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, dealer, &d);
    abstraction* ab = new high_low_abstraction(std::shared_ptr<simulator::cribbage>(&game));
    std::unordered_map<std::string, int> map;
    
    ab->setup_round();
    int num_available_actions = ab->get_num_available_actions();
    GTEST_ASSERT_EQ(num_available_actions, 2);
    ab->apply_action_from_list(0, false);
    card* cs = game.get_all_cards_played();
    GTEST_ASSERT_EQ(cs[0].get_value(true), 1);
    GTEST_ASSERT_EQ(cs[0].get_suit(false), 'H');

    num_available_actions = ab->get_num_available_actions();
    GTEST_ASSERT_EQ(num_available_actions, 2);
    ab->apply_action_from_list(0, false);
    cs = game.get_all_cards_played();
    GTEST_ASSERT_EQ(cs[1].get_value(true), 1);
    GTEST_ASSERT_EQ(cs[1].get_suit(false), 'D');


    num_available_actions = ab->get_num_available_actions();
    GTEST_ASSERT_EQ(num_available_actions, 2);
    ab->apply_action_from_list(1, false);
    cs = game.get_all_cards_played();
    GTEST_ASSERT_EQ(cs[2].get_value(true), 4);
    GTEST_ASSERT_EQ(cs[2].get_suit(false), 'S');

    num_available_actions = ab->get_num_available_actions();
    GTEST_ASSERT_EQ(num_available_actions, 2);
    ab->apply_action_from_list(1, false);
    cs = game.get_all_cards_played();
    GTEST_ASSERT_EQ(cs[3].get_value(true), 3);
    GTEST_ASSERT_EQ(cs[3].get_suit(false), 'D');
    
}


TEST(mccfrplayer, exploitabililty) {
    GTEST_SKIP();// Using a deck which should have allways give the same cards, letting causing the same infoset to be encountered several times
    std::mt19937 gen(1);

    card player_and_opp_hand[13];

    //player hand
    player_and_opp_hand[0] = card(1, 'D');
    player_and_opp_hand[1] = card(3, 'D');
    player_and_opp_hand[2] = card(2, 'S');
    player_and_opp_hand[3] = card(2, 'C');
    player_and_opp_hand[4] = card(10, 'S');
    player_and_opp_hand[5] = card(11, 'S');
    //opp hand
    player_and_opp_hand[6] = card(1, 'H');
    player_and_opp_hand[7] = card(2, 'D');
    player_and_opp_hand[8] = card(1, 'S');
    player_and_opp_hand[9] = card(4, 'H');
    player_and_opp_hand[10] = card(11, 'H');
    player_and_opp_hand[11] = card(7, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');
    
    randomplayer p1 = randomplayer();
    randomplayer p2 = randomplayer();
    mock_deck d = mock_deck();
    d.set_top_13_cards(player_and_opp_hand);
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1, &d);
    hand_cards_choose_24 abitest = hand_cards_choose_24(std::shared_ptr<simulator::cribbage>(&game));
    abstraction* ab = &abitest;
    int num_copies = 1;

    mccfrplayer mccfr = mccfrplayer(ab, 0.06, &gen);
    
    std::unordered_map<std::string, std::vector<double>*>* infostates = mccfr.get_infostates();

    // int num_iterations = 1'000;
    // for (int i=0; i < num_iterations; i++) {
    //     for (int j=0; j < 100; j++) {
    //         mccfr.iteration();
    //     }
    //     double regret = mccfr.amount_of_avg_regret(num_iterations*(i+1));
    //     std::cout << "Average Total regret after: " << num_iterations*(i+1) << " rounds is: " << regret << std::endl;
    // }

    double util_max_min = 10;
    double p = 0.9;
    double delta = 2*24*24*1820; // all possible endstates I think
    double M = 1820; //?

    determenisticpolicy dp = determenisticpolicy();
    policy loaded_policy = policy();
    loaded_policy.deserialize("C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_108.bin");
    mccfr.measure_exploitability(&loaded_policy, ab, util_max_min, p, delta, M);

    GTEST_ASSERT_EQ(1, 1);
    // randompolicy rp = randompolicy();
    // mccfr.measure_exploitability(&rp, ab, util_max_min, p, delta, M);

    // std::string filename = "C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_24_choose_1_100000.bin";
    // policy pp = policy();

    // pp.deserialize(filename);
    // mccfr.measure_exploitability(&pp, ab, util_max_min, p, delta, M);


}


TEST(mccfrplayer, meta_abstraction) {
    GTEST_SKIP();// Using a deck which should have allways give the same cards, letting causing the same infoset to be encountered several times
    std::mt19937 gen(1);

    card player_and_opp_hand[13];

    //player hand
    player_and_opp_hand[0] = card(1, 'D');
    player_and_opp_hand[1] = card(3, 'D');
    player_and_opp_hand[2] = card(2, 'S');
    player_and_opp_hand[3] = card(2, 'C');
    player_and_opp_hand[4] = card(10, 'S');
    player_and_opp_hand[5] = card(11, 'S');
    //opp hand
    player_and_opp_hand[6] = card(1, 'H');
    player_and_opp_hand[7] = card(2, 'D');
    player_and_opp_hand[8] = card(1, 'S');
    player_and_opp_hand[9] = card(4, 'H');
    player_and_opp_hand[10] = card(11, 'H');
    player_and_opp_hand[11] = card(7, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');
    
    randomplayer p1 = randomplayer();
    randomplayer p2 = randomplayer();
    mock_deck d = mock_deck();
    d.set_top_13_cards(player_and_opp_hand);
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1, &d);
    determenisticpolicy dp = determenisticpolicy(0);
    determenisticpolicy rp = determenisticpolicy(2);

    meta_abstraction meta = meta_abstraction(std::shared_ptr<simulator::cribbage>(&game), 1);

    meta.add_policy(&dp, std::make_shared<hand_cards>(), std::make_shared<play_permutation>());
    meta.add_policy(&rp, std::make_shared<hand_cards>(), std::make_shared<play_permutation>());
    mccfrplayer mccfr = mccfrplayer(&meta, 0.06, &gen);

    std::unordered_map<std::string, std::vector<double>*>* infostates = mccfr.get_infostates();

    int num_iterations = 1'000;
    int num_iter_per_check = 100;
    for (int i=0; i < num_iterations/num_iter_per_check; i++) {
        for (int j=0; j < num_iter_per_check; j++) {
            mccfr.iteration();
        }
        double regret = mccfr.amount_of_avg_regret(num_iter_per_check*(i+1));
        //print some values
        // for (auto state : *infostates) {
        //     std::cout << state.first << std::endl << "Regret" << std::endl;
        //     for (double n : state.second[0]) {
        //         std::cout << n << " ";
        //     }
        //     std::cout << std::endl << "Strategy" << std::endl;
        //     for (double n : state.second[1]) {
        //         std::cout << n << " ";
        //     }
        //     std::cout << std::endl;
        // }
        // std::cout << "Average Total regret after: " << num_iter_per_check*(i+1) << " rounds is: " << regret << std::endl;
    }

    GTEST_ASSERT_GT((*infostates)["1_2_2_3_"][1][0], (*infostates)["1_2_2_3_"][1][1]);
    GTEST_ASSERT_LT((*infostates)["1_1_2_4_"][1][0], (*infostates)["1_1_2_4_"][1][1]);
    
}


TEST(mccfrplayer, meta_abstraction_choose_best_policy) {
    // GTEST_SKIP();// Using a deck which should have allways give the same cards, causing the same infoset to be encountered several times
    std::mt19937 gen(3);

    card player_and_opp_hand[13];

    //player hand
    player_and_opp_hand[0] = card(1, 'D');
    player_and_opp_hand[1] = card(3, 'D');
    player_and_opp_hand[2] = card(2, 'S');
    player_and_opp_hand[3] = card(2, 'C');
    player_and_opp_hand[4] = card(10, 'S');
    player_and_opp_hand[5] = card(11, 'S');
    //opp hand
    player_and_opp_hand[6] = card(1, 'H');
    player_and_opp_hand[7] = card(2, 'D');
    player_and_opp_hand[8] = card(1, 'S');
    player_and_opp_hand[9] = card(4, 'H');
    player_and_opp_hand[10] = card(11, 'H');
    player_and_opp_hand[11] = card(7, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');
    
    randomplayer p1 = randomplayer();
    randomplayer p2 = randomplayer();
    mock_deck d = mock_deck();
    d.set_top_13_cards(player_and_opp_hand);
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1, &d);
    new_stack_4_choose_1_abstraction good_policy = new_stack_4_choose_1_abstraction(std::make_shared<simulator::cribbage>(game));
    mccfrplayer mccfr1 = mccfrplayer(&good_policy, 0.06, &gen);
    
    int num_iterations = 10'000;
    int num_iter_per_check = 1'000;
    double regret_last = 10'000.0;
    for (int i = 0; i < num_iterations/num_iter_per_check; i++) {
        for (int j=0; j < num_iter_per_check; j++) {
            mccfr1.iteration();
        }
        double regret = mccfr1.amount_of_avg_regret(num_iter_per_check*(i+1));
        GTEST_ASSERT_GT(regret_last, regret);
        regret_last = regret;
        // std::cout << "Average Total regret after: " << num_iter_per_check*(i+1) << " rounds is: " << regret << std::endl;
    }
    
    determenisticpolicy dp2 = determenisticpolicy(0);
    randompolicy rp = randompolicy();

    meta_abstraction meta = meta_abstraction(std::make_shared<simulator::cribbage>(game), 1);
    meta.add_policy(mccfr1.average_policy(), std::make_shared<new_stack_and_hand_cards>(), std::make_shared<play_card_from_hand>());
    meta.add_policy(&dp2, std::make_shared<hand_cards>(), std::make_shared<play_permutation>());
    meta.add_policy(&rp, std::make_shared<hand_cards>(), std::make_shared<play_permutation>());
    mccfrplayer mccfr2 = mccfrplayer(&meta, 0.06, &gen);

    std::unordered_map<std::string, std::vector<double>*>* infostates = mccfr2.get_infostates();

    num_iterations= 1'000;
    num_iter_per_check = 100;
    for (int i=0; i < num_iterations/num_iter_per_check; i++) {
        for (int j=0; j < num_iter_per_check; j++) {
            mccfr2.iteration();
        }
        double regret = mccfr2.amount_of_avg_regret(num_iter_per_check*(i+1));
        // print some values
        // for (auto state : *infostates) {
        //     std::cout << state.first << std::endl << "Regret" << std::endl;
        //     for (double n : state.second[0]) {
        //         std::cout << n << " ";
        //     }
        //     std::cout << std::endl << "Strategy" << std::endl;
        //     for (double n : state.second[1]) {
        //         std::cout << n << " ";
        //     }
        //     std::cout << std::endl;
        // }
        // std::cout << "Average Total regret after: " << num_iter_per_check*(i+1) << " rounds is: " << regret << std::endl;
    }
    std::unordered_map<std::string, std::vector<double>*> infostates2 = *infostates;
    //The good policy should be the favored strategy
    GTEST_ASSERT_GT(infostates2["1_2_2_3_"][1][0], infostates2["1_2_2_3_"][1][1]);
    GTEST_ASSERT_GT(infostates2["1_1_2_4_"][1][0], infostates2["1_1_2_4_"][1][1]);
    
    GTEST_ASSERT_GT(infostates2["1_2_2_3_"][1][0], infostates2["1_2_2_3_"][1][2]);
    GTEST_ASSERT_GT(infostates2["1_1_2_4_"][1][0], infostates2["1_1_2_4_"][1][2]);

}

TEST(mccfrplayer, external_sampling_test) {
    GTEST_SKIP(); //discontinued for now.
    std::string filepath = "C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_108.bin";
    std::mt19937 gen(1);

    card player_and_opp_hand[13];

    //player hand
    player_and_opp_hand[0] = card(1, 'D');
    player_and_opp_hand[1] = card(3, 'D');
    player_and_opp_hand[2] = card(2, 'S');
    player_and_opp_hand[3] = card(2, 'C');
    player_and_opp_hand[4] = card(10, 'S');
    player_and_opp_hand[5] = card(11, 'S');
    //opp hand
    player_and_opp_hand[6] = card(1, 'H');
    player_and_opp_hand[7] = card(2, 'D');
    player_and_opp_hand[8] = card(1, 'S');
    player_and_opp_hand[9] = card(3, 'H');
    player_and_opp_hand[10] = card(11, 'H');
    player_and_opp_hand[11] = card(7, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');
    
    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    mock_deck d = mock_deck();
    d.set_top_13_cards(player_and_opp_hand);
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1, &d);

    // good_policy.setup_round();
    // abstraction* a = good_policy.new_initial_state();
    
    // GTEST_ASSERT_EQ(a->get_num_available_actions(), good_policy.get_num_available_actions());
    // GTEST_ASSERT_EQ(a->get_informationstate_string(1), good_policy.get_informationstate_string(2));
    // GTEST_ASSERT_EQ(a->get_informationstate_string(2), good_policy.get_informationstate_string(1));


    int num_iterations = 1;
    int num_iter_per_check = 1;
    // try {
    //     for (int i = 0; i < num_iterations/num_iter_per_check; i++) {
    //         for (int j=0; j < num_iter_per_check; j++) {
    //             mccfr1.iteration_external();
    //         }
    //         double regret = mccfr1.amount_of_avg_regret(num_iter_per_check*(i+1));
    //         std::cout << "Average Total regret after: " << num_iter_per_check*(i+1) << " rounds is: " << regret << std::endl;
    //     }

    // } catch (std::runtime_error e) {
    //     std::cout << e.what() << std::endl;
    //     GTEST_FAIL();
    // }
    
    policy good_policy = policy();
    good_policy.deserialize(filepath);

    game.setup_round();
    game.skip_to_play_phase();

    for (auto prob : good_policy.action_probabilities("1_1_2_3_|")) {
        GTEST_ASSERT_NE(prob, 1/game.get_num_available_actions()) << "policy should contain this infoset";    
    }
    
    for (auto prob : good_policy.action_probabilities("1_2_2_3_|")) {
        GTEST_ASSERT_NE(prob, 1/game.get_num_available_actions()) << "policy should contain this infoset";    
    }
    

    

    determenisticpolicy dp2 = determenisticpolicy(3);
    randompolicy rp = randompolicy();


    meta_abstraction meta = meta_abstraction(std::make_shared<simulator::cribbage>(game), 1);
    meta.add_policy(&good_policy, std::make_shared<new_stack_and_hand_cards>(), std::make_shared<play_card_from_hand>());
    meta.add_policy(&dp2, std::make_shared<hand_cards>(), std::make_shared<play_permutation>());
    meta.add_policy(&rp, std::make_shared<hand_cards>(), std::make_shared<play_permutation>());
    mccfrplayer mccfr2 = mccfrplayer(&meta, 0.06, &gen);

    std::unordered_map<std::string, std::vector<double>*>* infostates = mccfr2.get_infostates();

    num_iterations= 1;
    num_iter_per_check = 1;
    try {
        for (int i=0; i < num_iterations/num_iter_per_check; i++) {
            for (int j=0; j < num_iter_per_check; j++) {
                mccfr2.iteration_external();
            }
            double regret = mccfr2.amount_of_avg_regret(num_iter_per_check*(i+1));
            // print some values
            for (auto state : *infostates) {
                std::cout << state.first << std::endl << "Regret" << std::endl;
                for (double n : state.second[0]) {
                    std::cout << n << " ";
                }
                std::cout << std::endl << "Strategy" << std::endl;
                for (double n : state.second[1]) {
                    std::cout << n << " ";
                }
                std::cout << std::endl;
            }
            std::cout << "Average Total regret after: " << num_iter_per_check*(i+1) << " rounds is: " << regret << std::endl;
        }
    } catch(std::runtime_error e) {
        std::cout << e.what() << std::endl;
    }
    std::unordered_map<std::string, std::vector<double>*> infostates2 = *infostates; 
    std::cout << infostates2["1_2_2_3_"][0].size() << std::endl;
    std::cout << infostates2["1_2_2_3_"][1].size() << std::endl;
    std::cout << infostates2["1_2_2_3_"][1][1] << std::endl;
    std::cout << infostates2["1_2_2_3_"][1][0] << std::endl;
    // GTEST_ASSERT_FALSE(true);
    // //The good policy should be the favored strategy
    GTEST_ASSERT_GT(infostates2["1_1_2_3_"][1][0], infostates2["1_1_2_3_"][1][1]);
    GTEST_ASSERT_GT(infostates2["1_1_2_3_"][1][0], infostates2["1_1_2_3_"][1][2]);

}


TEST(mccfrplayer, generating_policies) {
    GTEST_SKIP();
    std::string filepath = "C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_108.bin";
    std::mt19937 gen(1);

    std::string run_name = "gernerating_policies_start_greedy_";
    std::string filename = run_name +".bin"; 
    std::string log_name = run_name +"_log.txt";

    std::ofstream logger(log_name);
    logger << "starting test generating policies" << std::endl;
    logger.flush();

    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1);
    std::shared_ptr<simulator::cribbage> game_ptr = std::make_shared<simulator::cribbage>(game);

    hand_cards_choose_24 rp_abstraction = hand_cards_choose_24(game_ptr);
    new_stack_4_choose_1_abstraction ab = new_stack_4_choose_1_abstraction(game_ptr);
    greedy_abstraction start_ab = greedy_abstraction(game_ptr);
    meta_abstraction meta = meta_abstraction(game_ptr, 1);
    const int n = 5;
    policy* policies[n];
    abstraction* abs[n];
    abs[0] = &start_ab;
    for (int i=1; i<n; i++) {
        abs[i] = &ab;
    }
    
    policy* meta_policies[n];
    mccfrplayer* mccfr_lst[n];    
    mccfrplayer* mccfr_meta[n];




    policies[0] = new greedypolicy();
    // policies[0]->deserialize("C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_108.bin");
    // policies[0]->name = "cfr trained 10^8";

    mccfr_lst[0] = new mccfrplayer(&ab, 0.06, &gen);

    meta.add_policy(policies[0], start_ab.get_information_abstraction(), start_ab.get_action_abstraction());
    mccfr_meta[0] = new mccfrplayer(&meta, 0.06, &gen);
    meta_policies[0] = mccfr_meta[0]->average_policy();

    const int num_external = 3;
    policy* external_policies[num_external];
    external_policies[0] = new greedypolicy();
    external_policies[1] = new policy();
    external_policies[1]->deserialize("C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_108.bin");
    external_policies[1]->name = "cfr trained 10^8";
    external_policies[2] = new policy();

    
    abstraction* external_abstractions[num_external] = {new greedy_abstraction(game_ptr), new new_stack_4_choose_1_abstraction(game_ptr), new new_stack_4_choose_1_abstraction(game_ptr)};

    int num_iter = 10'000'000;
    std::chrono::steady_clock::time_point training_start = std::chrono::high_resolution_clock::now();

    for (int i = 1; i < n; i++) {
        logger << "starting training" << std::endl;

        std::chrono::steady_clock::time_point iter_start = std::chrono::high_resolution_clock::now();
        
        filename = run_name + "_" +ab.get_name() + "_v" + std::to_string(i) + ".bin";
        if (std::filesystem::exists(filename)) {
            logger << "Policy found!" << std::endl;
            policies[i]->deserialize(filename); 
            logger << "Policy deserialized from " << filename << std::endl;
        } else {
            logger << filename << " not found" << std::endl;
            mccfr_lst[i] = new mccfrplayer(&ab, 0.06, &gen);
            mccfr_lst[i]->set_opp_policy(meta_policies[i-1], &meta);

            for (int j=0; j<num_iter; j++) {
                mccfr_lst[i]->iteration();
            }
            policies[i] = mccfr_lst[i]->average_policy();
        }
        meta.add_policy(policies[i], std::make_shared<new_stack_and_hand_cards>(), std::make_shared<play_card_from_hand>());

        filename = run_name+ "_" + meta.get_name() + "_v" + std::to_string(i) + ".bin";
        if (std::filesystem::exists(filename)) {
            logger << "Policy found!" << std::endl;
            meta_policies[i]->deserialize(filename); 
            logger << "Policy deserialized from " << filename << std::endl;

        } else {

            mccfr_meta[i] = new mccfrplayer(&meta, 0.06, &gen);

            for (int j = 0; j < num_iter; j++) {
                mccfr_meta[i]->iteration();
            }

            meta_policies[i] = mccfr_meta[i]->average_policy();
        }
        logger.flush();
    } 
    std::chrono::steady_clock::time_point training_end = std::chrono::high_resolution_clock::now();
    logger << "Duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(training_end - training_start).count() << std::endl;
    
    for(int i=0; i<n; i++) {
        policies[i]->serialize(run_name + "_" +ab.get_name() + "_v" + std::to_string(i) + ".bin");
        meta_policies[i]->serialize(run_name+ "_" + meta.get_name() + "_v" + std::to_string(i) + ".bin");
    }

    statistics stats = statistics();
    std::vector<std::vector<double>> head_to_head_policy = std::vector<std::vector<double>>();
    std::vector<std::vector<double>> head_to_head_policy_meta = std::vector<std::vector<double>>();
    std::vector<std::vector<double>> head_to_head_meta = std::vector<std::vector<double>>();
    std::vector<std::vector<double>> head_to_head_policy_external = std::vector<std::vector<double>>();
    std::vector<std::vector<double>> head_to_head_meta_external = std::vector<std::vector<double>>();
    

    for(int i=0; i<n;i++) {
        head_to_head_policy.push_back(std::vector<double>());
        head_to_head_policy_meta.push_back(std::vector<double>());
        head_to_head_meta.push_back(std::vector<double>());
        head_to_head_policy_external.push_back(std::vector<double>());
        head_to_head_meta_external.push_back(std::vector<double>());
        for (int j = 0; j<n; j++) {
            head_to_head_policy[i].push_back(0.0);
            head_to_head_policy_meta[i].push_back(0.0);
            head_to_head_meta[i].push_back(0.0);
            head_to_head_policy_external[i].push_back(0.0);
            head_to_head_meta_external[i].push_back(0.0);
        }
    }
    int num_test = 100'000;
    for(int i=0; i<n; i++) {
        for(int j=i; j<n; j++) {
            head_to_head_policy[i][j] = stats.head_to_head(&game, policies[i], policies[j], abs[i], abs[j], num_test, &gen, &logger);
            logger << "head-to-head policy " << i << " vs. policy " << j << ": " << head_to_head_policy[i][j] << std::endl;
        }
        for (int j=0; j<n; j++) {
            head_to_head_policy_meta[i][j] = stats.head_to_head(&game, policies[i], meta_policies[j], abs[i], &meta, num_test, &gen, &logger);
            logger << "head-to-head policy: " << i << " vs. meta-policy " << j << ": " << head_to_head_policy_meta[i][j] << std::endl;
        }
        for(int j=i; j<n; j++) {
            head_to_head_meta[i][j] = stats.head_to_head(&game, meta_policies[i], meta_policies[j], &meta, &meta, num_test, &gen, &logger);
            logger << "head-to-head meta-policy: " << i << " vs. meta-policy" << j << ": " << head_to_head_meta[i][j] << std::endl;
        }
        for(int j=0; j<num_external; j++) {
            head_to_head_policy_external[i][j] = stats.head_to_head(&game, policies[i], external_policies[j], abs[i], external_abstractions[j], num_test, &gen, &logger);
            logger << "head-to-head policy: " << i << " vs. external " << external_policies[j]->name << ": " << head_to_head_policy_external[i][j] << std::endl;
        }
        for(int j=0; j<num_external; j++) {
            head_to_head_meta_external[i][j] = stats.head_to_head(&game, meta_policies[i], external_policies[j], &meta, external_abstractions[j], num_test, &gen, &logger);
            logger << "head-to-head meta-policy: " << i << " vs. external policy" << external_policies[j]->name << ": " << head_to_head_meta_external[i][j] << std::endl;
        }
        logger.flush();
    }
    
    logger.close();
    GTEST_ASSERT_LT(head_to_head_policy[0][1], head_to_head_policy[0][0]);


}


TEST(mccfrplayer, multiple_br) {
    /*
     * A test (hopefully) showing that simple best response iteration leads to cyclic behaviour. 
    */
    GTEST_SKIP();
    std::mt19937 gen(1);

    std::string run_name = "best_response_start_greedy_";
    
    std::string filename = run_name +".bin"; 
    std::string log_name = run_name +"_log.txt";
    std::ofstream logger(log_name);

    logger << "starting test multiple br" << std::endl;
    logger.flush();

    mockplayer p1 = mockplayer();
    mockplayer p2 = mockplayer();
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1);
    std::shared_ptr<simulator::cribbage> game_ptr = std::make_shared<simulator::cribbage>(game);

    hand_cards_choose_24 rp_abstraction = hand_cards_choose_24(game_ptr);
    new_stack_4_choose_1_abstraction ab = new_stack_4_choose_1_abstraction(game_ptr);
    greedy_abstraction start_ab = greedy_abstraction(game_ptr);
    meta_abstraction meta = meta_abstraction(game_ptr, 1);
    const int n = 5;
    const int num_external = 3;
    policy* policies[n];

    abstraction* abs[n];
    abs[0] = &start_ab;
    for (int i=1; i<n; i++) {
        abs[i] = &ab;
    }
    policy* external_policies[num_external];
    external_policies[0] = new greedypolicy();
    external_policies[1] = new policy();
    external_policies[1]->deserialize("C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_108.bin");
    external_policies[1]->name = "cfr trained 10^8";
    external_policies[2] = new policy();
    abstraction* external_abstractions[num_external] = {new greedy_abstraction(game_ptr), new new_stack_4_choose_1_abstraction(game_ptr), new new_stack_4_choose_1_abstraction(game_ptr)};
    // external_abstractions[0] = greedy_abstraction(game_ptr);
    // external_abstractions[1] = new_stack_4_choose_1_abstraction(game_ptr);
    

    mccfrplayer* mccfr_lst[n];

    policies[0] = new greedypolicy();
    // policies[0]->deserialize("C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_108.bin");
    // policies[0]->name = "cfr trained 10^8";

    int num_iter = 10'000'000;


    std::chrono::steady_clock::time_point training_start = std::chrono::high_resolution_clock::now();
    for (int i = 1; i < n; i++) {
        std::chrono::steady_clock::time_point iter_start = std::chrono::high_resolution_clock::now();
        filename = run_name + "_v" + std::to_string(i) + ".bin";
        if (std::filesystem::exists(filename)) {
            logger << "Policy found!" << std::endl;
            policies[i] = new policy();
            policies[i]->deserialize(filename); 
            logger << "Policy deserialized from " << filename << std::endl;
        } else {
            mccfr_lst[i] = new mccfrplayer(&ab, 0.06, &gen);
            mccfr_lst[i]->set_opp_policy(policies[i-1], abs[i-1]);
            

            for (int j=0; j<num_iter; j++) {
                mccfr_lst[i]->iteration();
            }
            policies[i] = mccfr_lst[i]->average_policy();
        }
        std::chrono::steady_clock::time_point iter_done = std::chrono::high_resolution_clock::now();
        logger << "iteration " << std::to_string(i) << " done in " << std::chrono::duration_cast<std::chrono::milliseconds>(iter_done - iter_start).count() << " milliseconds." << std::endl;
        logger.flush();
    } 
    std::chrono::steady_clock::time_point training_end = std::chrono::high_resolution_clock::now();
    

    logger << "Duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(training_end - training_start).count() << std::endl;
    logger.flush();



    statistics stats = statistics();
    std::vector<std::vector<double>> head_to_head_policy = std::vector<std::vector<double>>();
    std::vector<std::vector<double>> head_to_head_external = std::vector<std::vector<double>>();
    
    for(int i=0; i<n;i++) {
        head_to_head_policy.push_back(std::vector<double>());
        for (int j = 0; j<n; j++) {
            head_to_head_policy[i].push_back(0.0);
        }
        head_to_head_external.push_back(std::vector<double>());
        for (int j = 0; j<num_external; j++) {
            head_to_head_external[i].push_back(0.0);
        }
    }
    int num_test = 100'000;
    for(int i=0; i<n; i++) {
        for(int j=i; j<n; j++) {
            head_to_head_policy[i][j] = stats.head_to_head(&game, policies[i], policies[j], abs[i], abs[j], num_test, &gen, &logger);
            logger << "head-to-head policy " << i << " vs. policy " << j << ": " << head_to_head_policy[i][j] << std::endl;
        }        
    }
    for(int i=0; i<n; i++) {
        for (int j=0; j<num_external; j++) {
            head_to_head_external[i][j] = stats.head_to_head(&game, policies[i], external_policies[j], abs[i], external_abstractions[j], num_test, &gen, &logger);
            logger << "head-to-head policy " << i << " vs. policy " << external_policies[j]->name << ": " << head_to_head_external[i][j] << std::endl;
        
        }
        logger.flush();
    }

    for(int i=0; i<n; i++) {
        policies[i]->serialize(run_name + "_v" + std::to_string(i) + ".bin");
    }
    logger.flush();
    logger.close();

}


TEST(mccfrplayer, br_abstraction) {
    GTEST_SKIP();
    int seed = 1;
    std::mt19937 gen(seed);

    card player_and_opp_hand[13];

    //player hand
    player_and_opp_hand[0] = card(1, 'D');
    player_and_opp_hand[1] = card(3, 'D');
    player_and_opp_hand[2] = card(2, 'S');
    player_and_opp_hand[3] = card(2, 'C');
    player_and_opp_hand[4] = card(10, 'S');
    player_and_opp_hand[5] = card(11, 'S');
    //opp hand
    player_and_opp_hand[6] = card(1, 'H');
    player_and_opp_hand[7] = card(2, 'D');
    player_and_opp_hand[8] = card(1, 'S');
    player_and_opp_hand[9] = card(4, 'H');
    player_and_opp_hand[10] = card(11, 'H');
    player_and_opp_hand[11] = card(7, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');
    
    mock_deck d = mock_deck();
    d.set_top_13_cards(player_and_opp_hand);
    mockplayer p1 = mockplayer();
    mockplayer p2 = mockplayer();
    simulator::cribbage game = simulator::cribbage(seed, &p1, &p2, 1, &d);
    std::shared_ptr<simulator::cribbage> game_ptr = std::make_shared<simulator::cribbage>(game);
    game.setup_round();
    game.skip_to_play_phase();

    greedy_abstraction ab = greedy_abstraction(game_ptr);

    br_abstraction br_ab = br_abstraction(game_ptr, &ab);

    mccfrplayer mccfr = mccfrplayer(&ab, 0.06, &gen);
    greedypolicy greed = greedypolicy();

    policy* br_policy = mccfr.get_br(&greed, &ab, &br_ab);

    br_policy->serialize("br_policy_test");

}