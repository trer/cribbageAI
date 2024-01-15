#pragma once
#include "./simulator/player.h"
#include "./simulator/cribbage.h"
#include "./simulator/test.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
    // person::runner r1 = person::runner();
    // cout << r1.add(2, 4) << endl;
    randomplayer p1 = randomplayer();
    realplayer p2 = realplayer();
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2);
    int num_games = 10000;
    int p1_wins = game.start_games(num_games);
    cout << "P1 wins: " << p1_wins << "  P2 wins: " << num_games-p1_wins << endl;
    return 0;
}
