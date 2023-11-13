#pragma once
#include <gtest/gtest.h>
#include <random>
#include ".././simulator/hand.h"
#include "./utils.h"

TEST(card, hand_test) {
    std::random_device rd;  // a seed source for the random number engine

    deck d1 = deck(2);
    hand h1 = hand();
    GTEST_ASSERT_EQ(h1.get_num_cards(), 0) << "an empty hand should have no cards";
    

    h1.set_deck(&d1);
    h1.draw(6); //no checks for what happens if more than 6 cards are drawn. Possibly program breaking, but also not going to happen, and fixing it would mean the program would be slower.

    GTEST_ASSERT_EQ(h1.get_num_cards(), 6);
    for (int i = 0; i < 6; i++) {
        GTEST_ASSERT_NE((int) h1.get_card(i), 0) << "After cards are drawn they should not be 0";
    }

    card c1 = *h1.get_card(0);
    card c2 = *h1.get_card(1);

    h1.remove_2card(0, 1);
    GTEST_ASSERT_EQ(h1.get_num_cards(), 4);

    card c3 = *h1.get_card(4);
    card c4 = *h1.get_card(5);

    //Check that the two cards at the back of the hand is the removed cards
    //The fact that the first card is placed last is just an implementation detail 
    GTEST_EXPECT_TRUE(compare_cards(&c1, &c4));
    GTEST_EXPECT_TRUE(compare_cards(&c2, &c3));
    

    
    h1.set_num_cards(6);

    c1 = *h1.get_card(1);
    c2 = *h1.get_card(5);

    //Check that cards are removed in the right order
    h1.remove_2card(1, 5);
    GTEST_ASSERT_EQ(h1.get_num_cards(), 4);

    c3 = *h1.get_card(4);
    c4 = *h1.get_card(5);

    //Check that the two cards at the back of the hand is the removed cards
    GTEST_EXPECT_TRUE(compare_cards(&c1, &c3));
    GTEST_EXPECT_TRUE(compare_cards(&c2, &c2));
    
    //test that the cut is the same as the cut of the deck
    h1.draw_cut();
    card * cut_card = h1.get_card(h1.get_num_cards());
    card * cut_card_deck = d1.cut();

    GTEST_EXPECT_TRUE(compare_cards(cut_card, cut_card_deck));

    //Test that the cut is the same if we try to draw the cut again
    h1.draw_cut();
    card * cut_card2 = h1.get_card(h1.get_num_cards());
    GTEST_EXPECT_TRUE(compare_cards(cut_card, cut_card2));
    
    //Test that the cards played are removed from the hand.
    card play_card; 
    card * played_card;
    int pos;
    for (int i = 0; i < h1.get_num_cards(); i++) {
        pos = rd() % h1.get_num_cards();
        play_card = *h1.get_card(pos);
        h1.remove_card(pos);
        played_card = h1.get_card(h1.get_num_cards());
        compare_cards(&play_card, played_card);
    }
    

}
