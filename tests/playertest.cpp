#pragma once
#include <gtest/gtest.h>
#include ".././simulator/player.h"
#include "../simulator/utils.h"

TEST(player, check_policy) {
    
    policyplayer p_player = policyplayer();

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