#pragma once
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

    for (int i = 0; i < 10000; i++){
        mccfr.iteration(); //SEH exception somewhere (crashes program after like 5 turns :)
    }
    
    
    game.reset();
    game.setup_round();

    policy* p = mccfr.average_policy();

    while (!game.is_round_done()) {
        std::vector<double> ac = p->action_probabilities(&game, 1);
        std::discrete_distribution<int> d(ac.begin(), ac.end());
        
        std::cout << "Current player is: " << game.get_current_player() << std::endl;
        std::cout << "infoset is: " << game.get_informationstate_string(game.get_current_player()) << std::endl;
        std::cout << "number of available actions: " << game.num_available_actions << std::endl;
        int i = 0;
        for (auto prob : ac) {
            std::cout << "Action " << i++ << " with prob: " << prob << std::endl;
        }
        
        int sampled_action_index = d(gen); // replace with mersene twister
        game.apply_action_from_list(sampled_action_index);
        
    }
    
    
    
    
    
}