#pragma once
#include <gtest/gtest.h>
#include <random>
#include ".././simulator/scorer.h"
#include ".././simulator/utils.h"

TEST(scorer, scorer15) {

    card cards[5];
    cards[0] = card(4, 'H');
    cards[1] = card(5, 'S');
    cards[2] = card(7, 'D');
    cards[3] = card(10, 'H');
    cards[4] = card(12, 'H');

    GTEST_ASSERT_EQ(4, score_cards(cards, 5, false, &cards[4]));

    cards[0] = card(2, 'H');
    cards[1] = card(3, 'S');
    cards[2] = card(5, 'D');
    cards[3] = card(10, 'H');
    cards[4] = card(13, 'H');

    GTEST_ASSERT_EQ(8, score_cards(cards, 5, false, &cards[4]));

    cards[0] = card(1, 'H');
    cards[1] = card(2, 'S');
    cards[2] = card(9, 'D');
    cards[3] = card(8, 'H');
    cards[4] = card(5, 'H');

    GTEST_ASSERT_EQ(4, score_cards(cards, 5, false, &cards[4]));

}


TEST(scorer, x_of_a_kind) {

    card cards[5];
    cards[0] = card(4, 'H');
    cards[1] = card(4, 'S');
    cards[2] = card(8, 'D');
    cards[3] = card(10, 'H');
    cards[4] = card(12, 'H');
    GTEST_ASSERT_EQ(2, score_cards(cards, 5, false, &cards[4]));

    cards[0] = card(13, 'H');
    cards[1] = card(3, 'S');
    cards[2] = card(13, 'D');
    cards[3] = card(10, 'H');
    cards[4] = card(13, 'C');

    GTEST_ASSERT_EQ(6, score_cards(cards, 5, false, &cards[4]));

    cards[0] = card(12, 'H');
    cards[1] = card(12, 'S');
    cards[2] = card(9, 'D');
    cards[3] = card(12, 'H');
    cards[4] = card(12, 'H');

    GTEST_ASSERT_EQ(12, score_cards(cards, 5, false, &cards[4]));

}


TEST(scorer, runs) {
    std::random_device rd;  // a seed source for the random number engine

    card cards[5];
    // run of 3
    cards[0] = card(4, 'H');
    cards[1] = card(11, 'S');
    cards[2] = card(7, 'D');
    cards[3] = card(10, 'H');
    cards[4] = card(12, 'H');

    GTEST_ASSERT_EQ(3, score_cards(cards, 5, false, &cards[4]));

    //Doble run of 3 + pair
    cards[0] = card(5, 'H');
    cards[1] = card(3, 'S');
    cards[2] = card(9, 'D');
    cards[3] = card(4, 'H');
    cards[4] = card(5, 'C');

    GTEST_ASSERT_EQ(8, score_cards(cards, 5, false, &cards[4]));

    //run of 5
    cards[0] = card(10, 'H');
    cards[1] = card(9, 'S');
    cards[2] = card(8, 'D');
    cards[3] = card(12, 'H');
    cards[4] = card(11, 'H');

    GTEST_ASSERT_EQ(5, score_cards(cards, 5, false, &cards[4]));

    //double run of 4
    cards[0] = card(10, 'H');
    cards[1] = card(9, 'S');
    cards[2] = card(8, 'D');
    cards[3] = card(10, 'H');
    cards[4] = card(11, 'H');

    GTEST_ASSERT_EQ(10, score_cards(cards, 5, false, &cards[4]));

}


TEST(scorer, nobs) {

    card cards[5];
    //Jack the same suit as the cut
    cards[0] = card(4, 'H');
    cards[1] = card(11, 'S');
    cards[2] = card(7, 'D');
    cards[3] = card(9, 'H');
    cards[4] = card(12, 'S');
    GTEST_ASSERT_EQ(1, score_cards(cards, 5, false, &cards[4]));



    //Jack the same suit as the cut
    cards[0] = card(4, 'H');
    cards[1] = card(11, 'C');
    cards[2] = card(7, 'D');
    cards[3] = card(9, 'H');
    cards[4] = card(12, 'S');
    GTEST_ASSERT_EQ(0, score_cards(cards, 5, false, &cards[4]));
    
}


TEST(scorer, flush) {

    card cards[5];
    //flush of 4
    cards[0] = card(4, 'H');
    cards[1] = card(10, 'S');
    cards[2] = card(7, 'S');
    cards[3] = card(9, 'S');
    cards[4] = card(12, 'S');
    GTEST_ASSERT_EQ(4, score_cards(cards, 5, false, &cards[4]));



    //Flush of 5
    cards[0] = card(4, 'H');
    cards[1] = card(10, 'H');
    cards[2] = card(7, 'H');
    cards[3] = card(9, 'H');
    cards[4] = card(12, 'H');
    GTEST_ASSERT_EQ(5, score_cards(cards, 5, false, &cards[4]));

    //flush of 4 for crib
    cards[0] = card(4, 'H');
    cards[1] = card(10, 'S');
    cards[2] = card(7, 'S');
    cards[3] = card(9, 'S');
    cards[4] = card(12, 'S');
    GTEST_ASSERT_EQ(0, score_cards(cards, 5, true, &cards[4]));



    //Flush of 5 for crib
    cards[0] = card(4, 'H');
    cards[1] = card(10, 'H');
    cards[2] = card(7, 'H');
    cards[3] = card(9, 'H');
    cards[4] = card(12, 'H');
    GTEST_ASSERT_EQ(5, score_cards(cards, 5, false, &cards[4]));
    
}


TEST(scorer, combos) {

    card cards[5];
    //Double run of 3
    cards[0] = card(11, 'H');
    cards[1] = card(10, 'S');
    cards[2] = card(7, 'S');
    cards[3] = card(11, 'C');
    cards[4] = card(12, 'S');
    GTEST_ASSERT_EQ(8, score_cards(cards, 5, false, &cards[4]));



    //Double run of 4
    cards[0] = card(11, 'H');
    cards[1] = card(10, 'H');
    cards[2] = card(9, 'H');
    cards[3] = card(10, 'C');
    cards[4] = card(12, 'S');
    GTEST_ASSERT_EQ(10, score_cards(cards, 5, false, &cards[4]));

    //Tripple run of 3
    cards[0] = card(8, 'H');
    cards[1] = card(10, 'S');
    cards[2] = card(9, 'S');
    cards[3] = card(10, 'D');
    cards[4] = card(10, 'C');
    GTEST_ASSERT_EQ(15, score_cards(cards, 5, true, &cards[4]));



    //Double double run of three
    cards[0] = card(8, 'H');
    cards[1] = card(10, 'D');
    cards[2] = card(9, 'S');
    cards[3] = card(9, 'H');
    cards[4] = card(10, 'C');
    GTEST_ASSERT_EQ(16, score_cards(cards, 5, false, &cards[4]));


    //Holy grail (only gives 27 as the cut is scored seperatly)
    cards[0] = card(5, 'H');
    cards[1] = card(5, 'D');
    cards[2] = card(11, 'S');
    cards[3] = card(5, 'C');
    cards[4] = card(5, 'S');
    GTEST_ASSERT_EQ(29, score_cards(cards, 5, false, &cards[4]));
    
}




TEST(scorer, played_15_31) {

    card cards[8];
    // 5 + jack for 15
    cards[0] = card(5, 'H');
    cards[1] = card(11, 'S');

    GTEST_ASSERT_EQ(2, score_played_card(cards, 2, &cards[1], 15));


    //Sum of cards reaching 31
    cards[0] = card(5, 'H');
    cards[1] = card(11, 'C');
    cards[2] = card(5, 'D');
    cards[3] = card(9, 'H');
    cards[4] = card(2, 'S');
    GTEST_ASSERT_EQ(1, score_played_card(cards, 5, &cards[4], 31));
    
}


TEST(scorer, x_of_a_kind_played) {

    card cards[8];
    //pair
    cards[0] = card(4, 'H');
    cards[1] = card(4, 'S');

    GTEST_ASSERT_EQ(2, score_played_card(cards, 2, &cards[1], 8));


    //disjointed pair
    cards[0] = card(5, 'H');
    cards[1] = card(11, 'C');
    cards[2] = card(5, 'D');

    GTEST_ASSERT_EQ(0, score_played_card(cards, 3, &cards[2], 20));


    //three of a kind
    cards[0] = card(4, 'H');
    cards[1] = card(4, 'C');
    cards[2] = card(4, 'D');

    GTEST_ASSERT_EQ(6, score_played_card(cards, 3, &cards[2], 12));



    //three of a kind last turn + 15
    cards[0] = card(4, 'H');
    cards[1] = card(4, 'C');
    cards[2] = card(4, 'D');
    cards[3] = card(3, 'D');

    GTEST_ASSERT_EQ(2, score_played_card(cards, 4, &cards[3], 15));
    

    //four of a kind last turn
    cards[0] = card(3, 'H');
    cards[1] = card(4, 'C');
    cards[2] = card(4, 'H');
    cards[3] = card(4, 'D');
    cards[4] = card(4, 'S');

    GTEST_ASSERT_EQ(12, score_played_card(cards, 5, &cards[4], 19));
}


TEST(scorer, run_played) {

    card cards[8];
    //run of 3
    cards[0] = card(4, 'H');
    cards[1] = card(5, 'S');
    cards[2] = card(3, 'S');

    GTEST_ASSERT_EQ(3, score_played_card(cards, 3, &cards[2], 12));


    //not a double run of 3
    cards[0] = card(4, 'H');
    cards[1] = card(4, 'H');
    cards[2] = card(5, 'S');
    cards[3] = card(3, 'S');

    GTEST_ASSERT_EQ(3, score_played_card(cards, 4, &cards[3], 16));

    //run of 4
    cards[0] = card(10, 'D');
    cards[1] = card(2, 'H');
    cards[2] = card(4, 'H');
    cards[3] = card(5, 'S');
    cards[4] = card(3, 'S');

    GTEST_ASSERT_EQ(4, score_played_card(cards, 5, &cards[4], 24));


    //run of 7
    cards[0] = card(1, 'D');
    cards[1] = card(2, 'H');
    cards[2] = card(4, 'H');
    cards[3] = card(5, 'S');
    cards[4] = card(3, 'S');
    cards[5] = card(7, 'D');
    cards[6] = card(6, 'H');
    cards[7] = card(1, 'H');

    GTEST_ASSERT_EQ(7, score_played_card(cards, 8, &cards[7], 29));
}


TEST(scorer, score_crib) {
    
    card cards[5];
    cards[0] = card(7, 'H');
    cards[1] = card(3, 'S');
    cards[2] = card(6, 'D');
    cards[3] = card(7, 'D');
    cards[4] = card(2, 'H');

    GTEST_ASSERT_EQ(6, score_cards(cards, 5, true, &cards[4]));
}