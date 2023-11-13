#pragma once
#include <gtest/gtest.h>
#include ".././simulator/deck.h"

TEST(deck, deck_init) {
    deck d1 = deck();
    d1.set_seed(2);
    card* c1 = d1.deal_top();
    int c1_value = c1->get_value(true);
    card* c2 = d1.deal_top();
    
    GTEST_ASSERT_NE(c1, c2) << "The first and second card in the deck should not be the same";
    d1.shuffle();

    card* c3 = d1.deal_top();
    card* c4 = d1.deal_top();

    GTEST_ASSERT_NE(c3, c4) << "The first and second card in the deck should not be the same";
    GTEST_ASSERT_NE(c1_value, c3->get_value(true)) << "The first card should not be the same after a shuffle (This might happen due to rng) but as the seed is set it should always be fine in this test";

    card* cut1 = d1.cut();
    int cut1_value = cut1->get_value(true);
    card* cut2 = d1.cut();

    GTEST_ASSERT_EQ(cut1, cut2) << "If the deck is polled for the cut card multiple times it should give the same cut each time";

    d1.shuffle();

    card * cut3 = d1.cut();
    GTEST_ASSERT_NE(cut1_value, cut3->get_value(true)) << "After a shuffle the cut card should be different (This could be rng)";

    // deal all cards
    for (int i = 0; i < 51; i++)
    {
        d1.deal_top();
    }

    card * zero_card1 = d1.deal_top();
    card * zero_card2 = d1.deal_top();
    GTEST_ASSERT_EQ((int) zero_card1, 0) << "After all cards have been drawn only a 0 should be returned";
    GTEST_ASSERT_EQ((int) zero_card2, 0) << "After all cards have been drawn only a 0 should be returned 2";

    d1.shuffle();

    card * non_zero_card = d1.deal_top();
    GTEST_ASSERT_NE(zero_card1, non_zero_card) << "After shuffling again we should not get a 0 card";

}
