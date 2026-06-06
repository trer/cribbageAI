#pragma once
#include <gtest/gtest.h>
#include ".././simulator/deck.h"
#include ".././simulator/utils.h"

TEST(deck, deck_init) {
    deck d1 = deck();
    d1.set_seed(3);
    card* c1 = d1.deal_top();
    int c1_value = c1->get_value(true);
    char c1_suit = c1->get_suit(false);
    card* c2 = d1.deal_top();
    
    GTEST_ASSERT_NE(c1, c2) << "The first and second card in the deck should not be the same";
    d1.shuffle();

    card* c3 = d1.deal_top();
    card* c4 = d1.deal_top();

    GTEST_ASSERT_NE(c3, c4) << "The first and second card in the deck should not be the same";
    // GTEST_ASSERT_NE(c1_value, c3->get_value(true)) << "The first card should not be the same after a shuffle (This might happen due to rng) but as the seed is set it should always be fine in this test";
    GTEST_ASSERT_FALSE(c1_value == c3->get_value(true) && c1_suit == c3->get_suit(false)) << "The first card should not be the same after a shuffle (This might happen due to rng) but as the seed is set it should always be fine in this test";

    card* cut1 = d1.cut();
    int cut1_value = cut1->get_value(true);
    card* cut2 = d1.cut();

    GTEST_ASSERT_EQ(cut1, cut2) << "If the deck is polled for the cut card multiple times it should give the same cut each time";

    d1.shuffle();

    card * cut3 = d1.cut();
    GTEST_ASSERT_NE(cut1_value, cut3->get_value(true)) << "After a shuffle the cut card should be different (This could be rng)";

    // deal all cards
    for (int i = 0; i < MAX_NUM_DECKCARDS-1; i++)
    {
        d1.deal_top();
    }

    card * zero_card1 = d1.deal_top();
    card * zero_card2 = d1.deal_top();
    GTEST_ASSERT_EQ(zero_card1, nullptr) << "After all cards have been drawn only a 0 should be returned";
    GTEST_ASSERT_EQ(zero_card2, nullptr) << "After all cards have been drawn only a 0 should be returned 2";

    d1.shuffle();

    card * non_zero_card = d1.deal_top();
    GTEST_ASSERT_NE(zero_card1, non_zero_card) << "After shuffling again we should not get a 0 card";

}

TEST(deck, mockdeck) {
    mock_deck d1 = mock_deck();
    d1.set_seed(2);

    card player_and_opp_hand[13];

    //player hand
    player_and_opp_hand[0] = card(13, 'D');
    player_and_opp_hand[1] = card(12, 'D');
    player_and_opp_hand[2] = card(11, 'D');
    player_and_opp_hand[3] = card(5, 'C');
    player_and_opp_hand[4] = card(7, 'S');
    player_and_opp_hand[5] = card(3, 'S');
    //opp hand
    player_and_opp_hand[6] = card(10, 'H');
    player_and_opp_hand[7] = card(11, 'H');
    player_and_opp_hand[8] = card(1, 'H');
    player_and_opp_hand[9] = card(1, 'S');
    player_and_opp_hand[10] = card(6, 'H');
    player_and_opp_hand[11] = card(7, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');

    d1.set_top_13_cards(player_and_opp_hand);

    card out[13];

    for(int i=0; i<13; i++) {
        out[i] = *d1.deal_top();
    }

    for(int i=0; i<13; i++) {
        GTEST_EXPECT_TRUE(compare_cards(&out[i], &player_and_opp_hand[i]));
    }


    d1.shuffle();
    for(int i=0; i<13; i++) {
        out[i] = *d1.deal_top();
    }

    for(int i=0; i<13; i++) {
        GTEST_EXPECT_TRUE(compare_cards(&out[i], &player_and_opp_hand[i]));
    }

    deck *d2 = &d1;

    d2->shuffle();
    for(int i=0; i<13; i++) {
        out[i] = *d2->deal_top();
    }

    for(int i=0; i<13; i++) {
        GTEST_EXPECT_TRUE(compare_cards(&out[i], &player_and_opp_hand[i]));
    }
}


TEST(deck, mockdeck_set_top6) {
    mock_deck d1 = mock_deck();
    d1.set_seed(2);

    card player_and_opp_hand[13];

    //player hand
    player_and_opp_hand[0] = card(4, 'D');
    player_and_opp_hand[1] = card(3, 'D');
    player_and_opp_hand[2] = card(2, 'D');
    player_and_opp_hand[3] = card(5, 'C');
    player_and_opp_hand[4] = card(2, 'S');
    player_and_opp_hand[5] = card(3, 'S');
    //opp hand
    player_and_opp_hand[6] = card(2, 'C');
    player_and_opp_hand[7] = card(4, 'H');
    player_and_opp_hand[8] = card(1, 'H');
    player_and_opp_hand[9] = card(1, 'S');
    player_and_opp_hand[10] = card(3, 'H');
    player_and_opp_hand[11] = card(5, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');

    d1.set_top_6_cards(player_and_opp_hand);

    card out[13];

    for(int i=0; i<13; i++) {
        out[i] = *d1.deal_top();
    }

    for(int i=0; i<13; i++) {
        if (i < 6) {
            GTEST_EXPECT_TRUE(compare_cards(&out[i], &player_and_opp_hand[i]));
        } else {
            GTEST_EXPECT_FALSE(compare_cards(&out[i], &player_and_opp_hand[i])) << "i is: " << i;
        }
    }
    int const ranks = MAX_NUM_DECKCARDS/4;
    int seen[ranks][4] = {0};
    int num_deals = 10'000;
    for (int i = 0; i < ranks; i++) {
        for(int j=0; j<4; j++) {
            seen[i][j] = 0;
        }
    }
    for (int j=0; j < num_deals; j++) {
        d1.shuffle();
        for(int i=0; i<(6+6+1); i++) {
        out[i] = *d1.deal_top();
        }

        for(int i=0; i<(6+6+1); i++) {
            seen[out[i].get_value(true)-1][(int) out[i].get_suit(true)]++;
        }
    }

    //Assert that the chance to draw top6 cards are 1.0 and the chance to draw the rest are somewhat evenly distributed.

    double chance_to_random_draw = 0.0;
    double chance_to_not_draw = 1.0;
    for (int i=0; i<(6+1); i++) {
        chance_to_random_draw += chance_to_not_draw * (1.0/((MAX_NUM_DECKCARDS-6)-i));
        chance_to_not_draw -= chance_to_not_draw * (1.0/((MAX_NUM_DECKCARDS-6)-i));
    }
    // std::cout << "chance to random draw is: " << chance_to_random_draw << " and chance to NOT draw is: " << chance_to_not_draw << std::endl;
    for (int i = 0; i < ranks; i++) {
        for(int j=0; j<4; j++) {
            bool in_top_6 = false;
            for (int k=0; k<6; k++) {
                if (player_and_opp_hand[k].get_value(true)-1 == i && player_and_opp_hand[k].get_suit(true) == j) {
                    in_top_6 = true;
                }
            }
            if (in_top_6) {
                GTEST_ASSERT_EQ(seen[i][j], num_deals);
            } else {
                GTEST_ASSERT_LT(seen[i][j], num_deals*chance_to_random_draw + 200) << "rank: " << i << " suit: " << j;
                GTEST_ASSERT_GT(seen[i][j], num_deals*chance_to_random_draw - 200) << "rank: " << i << " suit: " << j;
            }            
        }
    }
    
}