#pragma once
#include <gtest/gtest.h>
#include ".././simulator/statistics.h"

TEST(statistics, plausible_results) {
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

    abstraction* ab = new new_stack_4_choose_1_abstraction(game_ptr);
    determenisticpolicy dp = determenisticpolicy(0);

    statistics stats = statistics();

    double roll_out_1 = stats.head_to_head(&game, &dp, &dp, ab, ab, 1, &gen);
    GTEST_ASSERT_EQ(roll_out_1, -4.0);

    double roll_out_2 = stats.head_to_head(&game, &dp, &dp, ab, ab, 2, &gen); 
    GTEST_ASSERT_EQ(roll_out_2, 0.0);

    double roll_out_100 = stats.head_to_head(&game, &dp, &dp, ab, ab, 100, &gen);
    GTEST_ASSERT_EQ(roll_out_100, 0.0);

    
}

TEST(statistics, bound_on_results) {
    GTEST_SKIP();
    int seed = 3;
    std::mt19937 gen(seed);

    card player_and_opp_hand[13];

    mockplayer p1 = mockplayer();
    mockplayer p2 = mockplayer();
    simulator::cribbage game = simulator::cribbage(seed, &p1, &p2, 1);
    std::shared_ptr<simulator::cribbage> game_ptr = std::make_shared<simulator::cribbage>(game);

    abstraction* ab = new new_stack_4_choose_1_abstraction(game_ptr);
    determenisticpolicy dp = determenisticpolicy(0);

    statistics stats = statistics();

    randompolicy rp = randompolicy();

    double bias = 0;
    for (int _ = 0; _ < 1000; _++) {
        double almost_0 = stats.head_to_head(&game, &rp, &rp, ab, ab, 100'000, &gen); //100'000 games usually enough to get it within 0.1
        // GTEST_ASSERT_GT(almost_0, -0.01);
        // GTEST_ASSERT_LT(almost_0, 0.01);
        std::cout << almost_0 << std::endl;
        bias += almost_0;
    }
    GTEST_ASSERT_GT(bias, -0.01);
    GTEST_ASSERT_LT(bias, 0.01);


    //You need about 100'000 test games to get good bounds    
}