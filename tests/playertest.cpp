#pragma once
#include <gtest/gtest.h>
#include ".././simulator/player.h"
#include "../simulator/utils.h"


TEST(player, check_policy) {
    int seed = 1;
    policyplayer p_player = policyplayer(seed);

    bool loaded = p_player.load_policy("C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_100000.bin");
    
    GTEST_ASSERT_TRUE(loaded);

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

    hand p_hand = hand(nullptr, player_hand, 6);
    

    for (int i = 0; i < num_cards_played; i++) {
        sum_cards += cards_played[i].get_value(false);
    }

    action c = p_player.poll_player(true, &p_hand, nullptr, 0, 0, 6, 0, 0, true);

    bool legal = check_valid_move(true, nullptr, 0, 0, nullptr, 0, player_hand, 6, c);

    GTEST_ASSERT_TRUE(legal);
}

TEST(player, permutationpolicyplayer) {
    int seed = 1;
    
    permutationpolicyplayer p_player = permutationpolicyplayer(seed);

    bool loaded = p_player.load_policy("C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_24_choose_1_100000.bin");
    GTEST_ASSERT_TRUE(loaded);

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

    hand p_hand = hand(nullptr, player_hand, 6);
    


    action c = p_player.poll_player(true, &p_hand, nullptr, 0, 0, 6, 0, 0, true);

    bool legal = check_valid_move(true, nullptr, 0, 0, nullptr, 0, player_hand, 6, c);

    GTEST_ASSERT_TRUE(legal);
    
    num_cards_played = 2;
    num_cards_in_player_hand = 3;
    sum_cards = 0;

    cards_played[0] = player_hand[3];
    cards_played[1] = card(6, 'S');

    for (int i = 0; i < num_cards_played; i++) {
        sum_cards += cards_played[i].get_value(false);
    }

    p_hand = hand(nullptr, player_hand, 3);

    c = p_player.poll_player(false, &p_hand, cards_played, num_cards_played, sum_cards, 3, 0, 0, false);

    legal = check_valid_move(false, cards_played, num_cards_played, sum_cards, crib, 0, p_hand.get_cards(), p_hand.get_num_cards(), c);

    GTEST_ASSERT_TRUE(legal);


    num_cards_played = 0;
    num_cards_in_player_hand = 4;
    sum_cards = 0;


    p_hand = hand(nullptr, player_hand, 4);

    c = p_player.poll_player(false, &p_hand, cards_played, num_cards_played, sum_cards, 4, 0, 0, false);

    legal = check_valid_move(false, cards_played, num_cards_played, sum_cards, nullptr, 0, p_hand.get_cards(), p_hand.get_num_cards(), c);

    GTEST_ASSERT_TRUE(legal);
}

TEST(player, mockplayer) {
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

    hand p_hand = hand(nullptr, player_and_opp_hand, 6);
    card cards_played[8];

    
    mockplayer p1 = mockplayer();

    action a = p1.poll_player(true, &p_hand, cards_played, 0, 0, 6, 0, 0, true);

    
}