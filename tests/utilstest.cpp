#pragma once
#include <gtest/gtest.h>
#include ".././simulator/utils.h"

TEST(utils, check_valid_move_discards) {
    
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

    //legal discard
    action player_action = action(&player_hand[0], &player_hand[5]);
    num_cards_in_player_hand = num_cards_in_player_hand - 2;
    bool valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, 0,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_TRUE(valid);

    // discard the same card twice
    player_action = action(&player_hand[0], &player_hand[0]);
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, 0,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_FALSE(valid);


    // Try to not discard the right amount of cards
    player_action = action(&player_hand[0]);
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, 0,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_FALSE(valid);

    player_action = action();
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, 0,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_FALSE(valid);

    player_action = action(0, &player_hand[0]);
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, 0,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_FALSE(valid);

    //Test what happens if we try to discard a card not a part of the hand
    card fake_discard_card = card(13, 'H');
    player_action = action(&player_hand[0], &fake_discard_card);
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, 0,
                     player_hand, num_cards_in_player_hand, player_action);
    
    GTEST_EXPECT_FALSE(valid);
    player_action = action(&fake_discard_card, &player_hand[4]);
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, 0,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_FALSE(valid);
}


TEST(utils, check_valid_move_play_first_card) {
    
    card cards_played[8];
    card crib[5];
    card player_hand[6];

    bool discard_phase = false;
    int num_cards_played = 0;
    int num_cards_in_player_hand = 4;
    int sum_cards = 0;

    //define player hand
    player_hand[0] = card(10, 'H');
    player_hand[1] = card(9, 'S');
    player_hand[2] = card(11, 'H');
    player_hand[3] = card(2, 'H');
    player_hand[4] = card(1, 'D');
    player_hand[5] = card(6, 'C');

    //define crib
    crib[0] = card(1, 'D'); // card 5 in player hand
    crib[1] = card(6, 'C'); // card 6 in player hand
    crib[2] = card(6, 'H');
    crib[3] = card(6, 'D');
    crib[4] = card(5, 'S'); //This is the cut card (though, I dont think it matters)

    int num_cards_in_crib = 5;

    //define played cards


    for (int i = 0; i < num_cards_played; i++) {
        sum_cards += cards_played[i].get_value(false);
    }

    //try to discard / wrong amount of cards
    action player_action = action(&player_hand[0], &player_hand[3]);
    num_cards_in_player_hand = 2;
    bool valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, num_cards_in_crib,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_FALSE(valid);

    player_action = action(0, &player_hand[3]);
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, num_cards_in_crib,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_FALSE(valid);

    player_action = action();
    num_cards_in_player_hand = 4;
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, num_cards_in_crib,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_FALSE(valid);

    // legal action
    player_action = action(&player_hand[3]);
    num_cards_in_player_hand = 3;
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, num_cards_in_crib,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_TRUE(valid);

    player_action = action(&player_hand[0]);
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, num_cards_in_crib,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_TRUE(valid);

    //Try to play card which is in crib
    player_action = action(&player_hand[4]);
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, num_cards_in_crib,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_FALSE(valid);

    player_action = action(&player_hand[5]);
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, num_cards_in_crib,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_FALSE(valid);

    //try to play cut card
    player_action = action(&crib[4]);
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, num_cards_in_crib,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_FALSE(valid);

    //try to play card not in player hand
    card fake_card = card(3, 'D');
    player_action = action(&fake_card);
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, num_cards_in_crib,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_FALSE(valid);

}

TEST(utils, check_valid_move_play_around_15) {
    
    card cards_played[8];
    card crib[5];
    card player_hand[6];

    bool discard_phase = false;
    int num_cards_played = 3;
    int num_cards_in_player_hand = 3;
    int sum_cards = 0;

    //define player hand
    player_hand[0] = card(10, 'H');
    player_hand[1] = card(9, 'S');
    player_hand[2] = card(11, 'H');
    player_hand[3] = card(2, 'H');
    player_hand[4] = card(1, 'D');
    player_hand[5] = card(6, 'C');

    //define crib
    crib[0] = player_hand[4]; // card 5 in player hand
    crib[1] = player_hand[5]; // card 6 in player hand
    crib[2] = card(6, 'H');
    crib[3] = card(6, 'D');
    crib[4] = card(5, 'S'); //This is the cut card (though, I dont think it matters)

    int num_cards_in_crib = 5;

    //define played cards
    cards_played[0] = card(3, 'H');
    cards_played[1] = player_hand[3];
    cards_played[2] = card(12, 'S');

    for (int i = 0; i < num_cards_played; i++) {
        sum_cards += cards_played[i].get_value(false);
    }


    // legal action
    action player_action = action(&player_hand[1]);
    num_cards_in_player_hand = 2;
    bool valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, num_cards_in_crib,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_TRUE(valid);

    player_action = action(&player_hand[0]);
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, num_cards_in_crib,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_TRUE(valid);

    //Try to play card which is in crib
    player_action = action(&player_hand[4]);
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, num_cards_in_crib,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_FALSE(valid);

    player_action = action(&player_hand[5]);
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, num_cards_in_crib,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_FALSE(valid);

    //try to play cut card
    player_action = action(&crib[4]);
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, num_cards_in_crib,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_FALSE(valid);

    //try to play card not in player hand
    card fake_card = card(3, 'D');
    player_action = action(&fake_card);
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, num_cards_in_crib,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_FALSE(valid);

    // try to play card allready played earlier
    player_action = action(&player_hand[3]);
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, num_cards_in_crib,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_FALSE(valid);
    

}


TEST(utils, check_valid_move_play_close_30) {
    
    card cards_played[8];
    card crib[5];
    card player_hand[6];

    bool discard_phase = false;
    int num_cards_played = 4;
    int num_cards_in_player_hand = 2;
    int sum_cards = 0;

    //define player hand
    player_hand[0] = card(10, 'H');
    player_hand[1] = card(3, 'S');
    player_hand[2] = card(11, 'H');
    player_hand[3] = card(2, 'H');
    player_hand[4] = card(1, 'D');
    player_hand[5] = card(6, 'C');

    //define crib
    crib[0] = player_hand[4]; // card 5 in player hand
    crib[1] = player_hand[5]; // card 6 in player hand
    crib[2] = card(6, 'H');
    crib[3] = card(6, 'D');
    crib[4] = card(5, 'S'); //This is the cut card (though, I dont think it matters)

    int num_cards_in_crib = 5;

    //define played cards
    cards_played[0] = card(3, 'H');
    cards_played[1] = player_hand[3]; //2
    cards_played[2] = card(12, 'S');
    cards_played[3] = player_hand[2]; //10



    for (int i = 0; i < num_cards_played; i++) {
        sum_cards += cards_played[i].get_value(false);
    }


    // legal action
    action player_action = action(&player_hand[1]);
    
    num_cards_in_player_hand = 1;
    bool valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, num_cards_in_crib,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_TRUE(valid);

    //card pushing sum over 31
    player_action = action(&player_hand[0]);
    valid = check_valid_move(discard_phase, cards_played, num_cards_played, sum_cards, crib, num_cards_in_crib,
                     player_hand, num_cards_in_player_hand, player_action);
    GTEST_EXPECT_FALSE(valid);
  

}


