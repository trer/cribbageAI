#pragma once
#include <gtest/gtest.h>
#include "../simulator/cribbage.h"
#include "../simulator/greedyplayer.h"
#include "../simulator/cheatingutcplayer.h"

TEST(cheatingutcplayer, run_test) {
    std::mt19937 gen(1);
    
    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1);
    game.setup_round();
    int num_copies = 1;

    cheatingutcplayer utc = cheatingutcplayer(1.75, 0, 1000, &gen);

    std::vector<int> visits = utc.search_and_return_visit_counts(&game);

    GTEST_ASSERT_EQ(num_copies, 1);
    
}