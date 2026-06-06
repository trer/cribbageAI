#pragma once
#include <gtest/gtest.h>
#include ".././simulator/greedyplayer.h"
#include "../simulator/abstraction.h"

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
    player_hand[2] = card(8, 'H');
    player_hand[3] = card(4, 'H');
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


TEST(greedyplayer, greedypolicy) {
    greedyplayer g_player = greedyplayer();
    greedypolicy g_policy = greedypolicy();
    int best_card;

    std::mt19937 gen(1);

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
    player_and_opp_hand[7] = card(1, 'C');
    player_and_opp_hand[8] = card(1, 'S');
    player_and_opp_hand[9] = card(3, 'H');
    player_and_opp_hand[10] = card(11, 'H');
    player_and_opp_hand[11] = card(7, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');
    
    mock_deck d = mock_deck();
    d.set_top_13_cards(player_and_opp_hand);
    simulator::cribbage game = simulator::cribbage(1, 1, &d);

    greedy_play act_ab = greedy_play();
    greedy_view info_ab = greedy_view();

    

    game = simulator::cribbage(1, 1);

    for (int i = 0; i < 10000; i++) {
        game.reset();
        game.setup_round();
        game.skip_to_play_phase();
        while(!game.is_playphase_done()) {
            int greedyplayer_index = g_player.get_best_card(game.get_player_hand(game.get_current_player())->get_cards(), game.get_player_hand_size(game.get_current_player()), game.get_cards_played(), game.get_num_cards_played(), game.get_sum_cards_played());
            card first = *game.get_player_hand(game.get_current_player())->get_card(greedyplayer_index);
            std::vector<double> ac = g_policy.action_probabilities(&game);
            std::vector<double> ac2 = g_policy.action_probabilities(info_ab.get_informationstate_string(&game, game.get_current_player()));
            
            card before = *game.get_player_hand(game.get_current_player())->get_card(greedyplayer_index);

            double sum = 0.0;
            int index = 0;
            double max = -1.0;
            for (int j=0; j<ac.size(); j++) {
                sum += ac[j];
                GTEST_ASSERT_TRUE(ac[j] == 0.0 || ac[j] == 1.0);
                GTEST_ASSERT_EQ(ac[j], ac2[j]) << "j is: " << j << " and i is: " << i;
                if (max < ac[j]) {
                    max = ac[j];
                    index = j;
                }
            }
            GTEST_ASSERT_EQ(sum, 1.0);

            // GTEST_ASSERT_EQ(index, greedyplayer_index);

            act_ab.apply_action_from_list(&game, index);
            if(game.get_num_cards_played_since_new_stack() == 0) {
                bool played = false;
                for (int i=0; i<8; i++) {
                    if (compare_cards(&before, &game.get_all_cards_played()[i])) {
                        played = true;
                        break;
                    }
                }
                GTEST_ASSERT_TRUE(played);
            } else {
                card after = game.get_cards_played()[game.get_num_cards_played_since_new_stack()-1];
                GTEST_ASSERT_TRUE(compare_cards(&before, &after)) << "before card: " << before.string_format() << " after card: " << after.string_format();
            }
        }
    }   
    

}