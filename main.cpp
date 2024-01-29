#pragma once
#include "./simulator/player.h"
#include "./simulator/greedyplayer.h"
#include "./simulator/cribbage.h"
#include <chrono>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
    randomplayer p1 = randomplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(2, &p1, &p2);
    int num_games = 100000;
    int counter = 0;
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_games; i++)
    {
        game.round();
        game.reset();
        counter++;
    }
    
    // int p1_wins = game.start_games(num_games);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    // cout << "P1 wins: " << p1_wins << "  P2 wins: " << num_games-p1_wins << endl;
    cout << "Duration in microseconds: " << duration.count() << endl;
    cout << "With " << num_games << " rounds, that is " << duration.count() / num_games << " per round!" << endl;
    return 0;
}
