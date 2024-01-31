#pragma once
#include <gtest/gtest.h>
#include ".././simulator/greedyplayer.h"

TEST(greedyplayer, check_discard) {
    
    greedyplayer g_player = greedyplayer();
    int* best_cards;

    card cards_played[8];
    card crib[5];
    card player_hand[6];

    bool discard_phase = true;
    int num_cards_played = 0;
    int num_cards_in_player_hand = 6;
    int sum_cards = 0;

    player_hand[0] = card(10, 'H');
    player_hand[1] = card(9, 'S');
    player_hand[2] = card(11, 'H');
    player_hand[3] = card(2, 'H');
    player_hand[4] = card(1, 'D');
    player_hand[5] = card(6, 'C');

    for (int i = 0; i < num_cards_played; i++) {
        sum_cards += cards_played[i].get_value(false);
    }

    //find best discard
    best_cards = g_player.get_best_two_cards(player_hand, num_cards_in_player_hand, true);
    
    GTEST_ASSERT_EQ(best_cards[0], 3);
    GTEST_ASSERT_EQ(best_cards[1], 4);


    // Find best discard, when it depends on if you are the dealer or not
    player_hand[0] = card(11, 'H');
    player_hand[1] = card(9, 'S');
    player_hand[2] = card(13, 'H');
    player_hand[3] = card(12, 'H');
    player_hand[4] = card(1, 'D');
    player_hand[5] = card(6, 'C');


    //9 and 6 should be best when player is dealer
    best_cards = g_player.get_best_two_cards(player_hand, num_cards_in_player_hand, true);
    
    GTEST_ASSERT_EQ(best_cards[0], 1);
    GTEST_ASSERT_EQ(best_cards[1], 5);

    //9 and 1? should be best when player is pone
    best_cards = g_player.get_best_two_cards(player_hand, num_cards_in_player_hand, false);
    
    GTEST_ASSERT_EQ(best_cards[0], 1);
    GTEST_ASSERT_EQ(best_cards[1], 4);

    // Find best discard, when it depends on if you are the dealer or not
    player_hand[0] = card(11, 'H');
    player_hand[1] = card(9, 'S');
    player_hand[2] = card(13, 'H');
    player_hand[3] = card(12, 'H');
    player_hand[4] = card(10, 'D');
    player_hand[5] = card(6, 'C');


    //9 and 6 should be best when player is dealer 15 crib + run of 4
    best_cards = g_player.get_best_two_cards(player_hand, num_cards_in_player_hand, true);
    
    GTEST_ASSERT_EQ(best_cards[0], 1);
    GTEST_ASSERT_EQ(best_cards[1], 5);

    //D and K best card when player is pone 15 + run of 3
    best_cards = g_player.get_best_two_cards(player_hand, num_cards_in_player_hand, false);
    
    GTEST_ASSERT_EQ(best_cards[0], 2);
    GTEST_ASSERT_EQ(best_cards[1], 3);

    
    // Find best discard, when it depends on if you are the dealer or not
    player_hand[0] = card(3, 'H');
    player_hand[1] = card(5, 'S');
    player_hand[2] = card(7, 'H');
    player_hand[3] = card(13, 'H');
    player_hand[4] = card(11, 'D');
    player_hand[5] = card(10, 'C');


    //9 and 6 should be best when player is dealer 15 crib + run of 4
    best_cards = g_player.get_best_two_cards(player_hand, num_cards_in_player_hand, true);
    
    GTEST_ASSERT_EQ(best_cards[0], 0);
    GTEST_ASSERT_EQ(best_cards[1], 2);

}

TEST(greedyplayer, check_play) {
    
    greedyplayer g_player = greedyplayer();
    int best_card;

    card cards_played[8];
    card crib[5];
    card player_hand[6];

    bool discard_phase = false;
    int num_cards_played = 2;
    int num_cards_in_player_hand = 6;
    int sum_cards = 0;

    player_hand[0] = card(10, 'H');
    player_hand[1] = card(9, 'S');
    player_hand[2] = card(11, 'H');
    player_hand[3] = card(2, 'H');
    player_hand[4] = card(1, 'D');
    player_hand[5] = card(6, 'C');

    cards_played[0] = card(2, 'H');
    cards_played[1] = card(9, 'H');


    for (int i = 0; i < num_cards_played; i++) {
        sum_cards += cards_played[i].get_value(false);
    }

    //find best card to play
    best_card = g_player.get_best_card(player_hand, num_cards_in_player_hand, cards_played, num_cards_played, sum_cards);
    
    GTEST_ASSERT_EQ(best_card, 1);

    num_cards_in_player_hand = 6;
    sum_cards = 0;
    num_cards_played = 3;

    player_hand[0] = card(10, 'H');
    player_hand[1] = card(9, 'S');
    player_hand[2] = card(7, 'H');
    player_hand[3] = card(8, 'H');
    player_hand[4] = card(1, 'D');
    player_hand[5] = card(6, 'C');

    cards_played[0] = card(2, 'H');
    cards_played[1] = card(9, 'H');
    cards_played[2] = card(7, 'D');



    for (int i = 0; i < num_cards_played; i++) {
        sum_cards += cards_played[i].get_value(false);
    }

    //find best card to play run of 3 > pair
    best_card = g_player.get_best_card(player_hand, num_cards_in_player_hand, cards_played, num_cards_played, sum_cards);
    
    GTEST_ASSERT_EQ(best_card, 3);


    num_cards_in_player_hand = 6;
    sum_cards = 0;
    num_cards_played = 2;

    player_hand[0] = card(10, 'H');
    player_hand[1] = card(2, 'S');
    player_hand[2] = card(7, 'C');
    player_hand[3] = card(8, 'H');
    player_hand[4] = card(1, 'D');
    player_hand[5] = card(6, 'C');

    cards_played[0] = card(7, 'H');
    cards_played[1] = card(7, 'D');



    for (int i = 0; i < num_cards_played; i++) {
        sum_cards += cards_played[i].get_value(false);
    }

    //find best card to play 3 of a kind > 15
    best_card = g_player.get_best_card(player_hand, num_cards_in_player_hand, cards_played, num_cards_played, sum_cards);
    
    GTEST_ASSERT_EQ(best_card, 2);

    num_cards_in_player_hand = 6;
    sum_cards = 0;
    num_cards_played = 3;

    player_hand[0] = card(12, 'S');
    player_hand[1] = card(2, 'S');
    player_hand[2] = card(1, 'H');
    player_hand[3] = card(8, 'H');
    player_hand[4] = card(1, 'D');
    player_hand[5] = card(6, 'C');

    cards_played[0] = card(12, 'C');
    cards_played[1] = card(12, 'D');
    cards_played[2] = card(12, 'H');



    for (int i = 0; i < num_cards_played; i++) {
        sum_cards += cards_played[i].get_value(false);
    }

    //Don't play illegal card
    best_card = g_player.get_best_card(player_hand, num_cards_in_player_hand, cards_played, num_cards_played, sum_cards);
    
    GTEST_ASSERT_EQ(best_card, 2);
}
