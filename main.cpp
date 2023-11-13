#include "./simulator/player.h"
#include "./simulator/cribbage.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
    randomplayer p1 = randomplayer();
    realplayer p2 = realplayer();
    cribbage game = cribbage(1, &p1, &p2);
    int* winner = game.start_games(100);
    cout << "P1 wins: " << winner[0] << "  P2 wins: " << winner[1] << endl;
    return 0;
}
