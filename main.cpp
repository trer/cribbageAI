#pragma once
#include "simulator/player.h"
#include "simulator/greedyplayer.h"
#include "simulator/cribbage.h"
#include "simulator/mccfrplayer.h"
#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>

using namespace std;

void test_policy(simulator::cribbage game, int num_games, policy* p, std::mt19937 gen, int mccfrpoints, int greedypoints) {
    int win;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_games; i++)
    {
        game.reset();
        game.setup_round();
        game.skip_to_play_phase();
        while (!game.is_playphase_done()) {
            if (game.get_current_player() == 1) {
                if (game.get_num_available_actions() == 0 && !game.is_playphase_done()) {
                    win = game.apply_action_from_list(-1);
                } else {
                    std::vector<double> ac1 = p->action_probabilities(game.get_informationstate_string(1), game.get_num_available_actions());
                    std::discrete_distribution<int> d(ac1.begin(), ac1.end());
                    int sampled_action_index = d(gen);
                    win = game.apply_action_from_list(sampled_action_index);
                }
            } else {
                game.set_action_for_player(2);
                win = game.resolve_action();
            }
            if (win != 0) {
                std::cout << "hold up" << std::endl;
            }
        }
        // game.matching_setup();
        // game.matching();

        mccfrpoints += game.get_player1_score();
        greedypoints += game.get_player2_score();
    }
    
    // int p1_wins = game.start_games(num_games);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    cout << "mccfr points: " << mccfrpoints << "  greedypoints: " << greedypoints << endl;
    cout << "mccfrpoints / greedypoints: " << (double) mccfrpoints / greedypoints << endl;
    cout << "Duration in milliseconds: " << duration.count() << endl;
    cout << "With " << num_games << " rounds, that is " << (double) duration.count() / num_games << " milliseconds per round!" << endl;

}

int main(int argc, char const *argv[])
{
    std::string filename = "mccfr_policy_100000.bin"; 
    std::mt19937 gen(20);
    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(5, &p1, &p2);
    policy* p = new policy();


    int num_games = 10000;
    int greedypoints = 0;
    int mccfrpoints = 0;


    cout << "Testing a random policy" << endl;
    test_policy(game, num_games, p, gen, mccfrpoints, greedypoints);

    delete p;


    if (std::filesystem::exists(filename)) {
        cout << "Policy found!" << endl;
        p = new policy();
        p->deserialize(filename); 
        cout << "Policy deserialized" << endl;
    } else {
        mccfrplayer mccfr = mccfrplayer(&game, 0.06, &gen);

        int num_iter = 100000;
        cout << "starting training" << endl;
        auto training_start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_iter; i++) {
            mccfr.iteration();
        }
        
        auto training_end = std::chrono::high_resolution_clock::now();
        cout << "finished training : " << std::chrono::duration_cast<std::chrono::milliseconds>(training_end - training_start).count() << " milliseconds." << endl;
        p = mccfr.average_policy();
        p->serialize(filename);
        auto save_done = std::chrono::high_resolution_clock::now();
        cout << "saved policy: " << std::chrono::duration_cast<std::chrono::milliseconds>(save_done - training_end) << " milliseconds." << endl;
    }

    std::mt19937 gen2(1);
    simulator::cribbage game2 = simulator::cribbage(5, &p1, &p2);


    num_games = 10000;
    greedypoints = 0;
    mccfrpoints = 0;

    test_policy(game2, num_games, p, gen, mccfrpoints, greedypoints);
    return 0;
}
