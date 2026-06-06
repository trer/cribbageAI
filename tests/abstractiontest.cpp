#pragma once
#include <gtest/gtest.h>
#include "../simulator/abstraction.h"
#include ".././simulator/player.h"
#include "../simulator/greedyplayer.h"
#include "../simulator/utils.h"
#include "../simulator/statistics.h"



TEST(abstraction, run_test) {
    GTEST_SKIP();
    int seed = 1;

    
    simulator::cribbage game = simulator::cribbage(seed, 1);
    std::shared_ptr<simulator::cribbage> g2 = std::make_shared<simulator::cribbage>();
    std::shared_ptr<simulator::cribbage> g3 = std::make_shared<simulator::cribbage>(game);
    abstraction ab = abstraction(g3, std::make_shared<new_stack_and_hand_cards>(), std::make_shared<play_card_from_hand>());

    game.setup_round();
    ab.setup_round();
    GTEST_ASSERT_FALSE(game.is_discard_done());
    GTEST_ASSERT_EQ(ab.get_current_player(), game.get_current_player());
    GTEST_ASSERT_EQ(ab.get_num_available_actions(), 15);
    ab.apply_action_from_list(2);
    GTEST_ASSERT_EQ(ab.get_current_player(), game.get_current_player());
    GTEST_ASSERT_EQ(ab.get_num_available_actions(), 15);

    ab.apply_action_from_list(4);

    GTEST_ASSERT_TRUE(game.is_discard_done());

    while (!ab.is_terminal())
    {
        GTEST_ASSERT_TRUE(!game.is_playphase_done());
        GTEST_ASSERT_EQ(ab.get_num_available_actions(), game.get_num_available_actions());
        GTEST_ASSERT_EQ(ab.get_current_player(), game.get_current_player());
        ab.apply_action_from_list(0);
    }
    game.matching();
    GTEST_ASSERT_EQ(ab.is_terminal(), game.is_playphase_done());
    GTEST_ASSERT_EQ(ab.get_point_diff(1), game.get_point_diff(1));
    
    ab.setup_round();
    GTEST_ASSERT_FALSE(game.is_discard_done());
}


TEST(abstraction, hand_cards) {
    int seed = 1;

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
    simulator::cribbage game = simulator::cribbage(seed, 1, &d);
    game.setup_round();
    hand_cards ab = hand_cards();

    GTEST_ASSERT_EQ(game.get_player_hand_size(1), 6);

    std::string key1 = ab.get_informationstate_string(&game, 1);
    std::string key2 = ab.get_informationstate_string(&game, 2);

    GTEST_ASSERT_EQ(key1, "1_2_2_3_10_11_");
    GTEST_ASSERT_EQ(key2, "1_1_2_4_7_11_");


    game.skip_to_play_phase();

    key1 = ab.get_informationstate_string(&game, 1);
    key2 = ab.get_informationstate_string(&game, 2);
    GTEST_ASSERT_EQ(key1, "1_2_2_3_");
    GTEST_ASSERT_EQ(key2, "1_1_2_4_");

    game.apply_action_from_list(0);

    key1 = ab.get_informationstate_string(&game, 1);
    key2 = ab.get_informationstate_string(&game, 2);
    GTEST_ASSERT_EQ(key1, "1_2_2_3_");
    GTEST_ASSERT_EQ(key2, "1_2_4_");

    game.apply_action_from_list(0);

    key1 = ab.get_informationstate_string(&game, 1);
    key2 = ab.get_informationstate_string(&game, 2);
    GTEST_ASSERT_EQ(key1, "2_2_3_");
    GTEST_ASSERT_EQ(key2, "1_2_4_");

}


TEST(abstraction, new_stack_and_hand_cards) {
    int seed = 1;

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
    simulator::cribbage game = simulator::cribbage(seed, 1, &d);
    game.setup_round();
    new_stack_and_hand_cards ab = new_stack_and_hand_cards();

    std::string key1 = ab.get_informationstate_string(&game, 1);
    std::string key2 = ab.get_informationstate_string(&game, 2);

    GTEST_ASSERT_EQ(key1, "1_2_2_3_10_11_|");
    GTEST_ASSERT_EQ(key2, "1_1_2_4_7_11_|");


    game.skip_to_play_phase();

    key1 = ab.get_informationstate_string(&game, 1);
    key2 = ab.get_informationstate_string(&game, 2);
    GTEST_ASSERT_EQ(key1, "1_2_2_3_|");
    GTEST_ASSERT_EQ(key2, "1_1_2_4_|");

    game.apply_action_from_list(0);

    key1 = ab.get_informationstate_string(&game, 1);
    key2 = ab.get_informationstate_string(&game, 2);
    GTEST_ASSERT_EQ(key1, "1_2_2_3_|1_");
    GTEST_ASSERT_EQ(key2, "1_2_4_|1_");

    game.apply_action_from_list(0);

    key1 = ab.get_informationstate_string(&game, 1);
    key2 = ab.get_informationstate_string(&game, 2);
    GTEST_ASSERT_EQ(key1, "2_2_3_|1_1_");
    GTEST_ASSERT_EQ(key2, "1_2_4_|1_1_");

}

TEST(abstraction, play_card_from_hand) {
    int seed = 1;

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
    simulator::cribbage game = simulator::cribbage(seed, 1, &d);
    game.setup_round();
    play_card_from_hand ab = play_card_from_hand();

    GTEST_ASSERT_EQ(ab.get_num_available_actions(&game), 15);
    GTEST_ASSERT_EQ(ab.get_num_available_actions(&game), 15);
    ab.apply_action_from_list(&game, 14);
    GTEST_ASSERT_EQ(ab.get_num_available_actions(&game), 15);
    ab.apply_action_from_list(&game, 14);

    GTEST_ASSERT_EQ(ab.get_num_available_actions(&game), 4);
    ab.apply_action_from_list(&game, 3);
    GTEST_ASSERT_EQ(game.get_cards_played()[0].get_value(true), 4);

    GTEST_ASSERT_EQ(ab.get_num_available_actions(&game), 4);
    ab.apply_action_from_list(&game, 3);
    GTEST_ASSERT_EQ(game.get_cards_played()[1].get_value(true), 3);

    GTEST_ASSERT_EQ(ab.get_num_available_actions(&game), 3);
    ab.apply_action_from_list(&game, 1);
    GTEST_ASSERT_EQ(game.get_cards_played()[2].get_value(true), 1);

    GTEST_ASSERT_EQ(ab.get_num_available_actions(&game), 3);
    GTEST_ASSERT_EQ(ab.get_num_available_actions(&game), 3);
    ab.apply_action_from_list(&game, 1);
    GTEST_ASSERT_EQ(game.get_cards_played()[3].get_value(true), 2);
}

TEST(abstraction, play_permutation) {
    int seed = 1;

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
    simulator::cribbage game = simulator::cribbage(seed, 1, &d);
    game.setup_round();
    play_permutation ab = play_permutation();

    try {
        ab.get_num_available_actions(&game);
        GTEST_FAIL();
    } catch (const std::runtime_error& ex) {
        
    }

    try {
        ab.apply_action_from_list(&game, 14);   
        GTEST_FAIL();
    } catch (const std::runtime_error& ex) {
    
    }
    
    game.skip_to_play_phase();

    try {
    GTEST_ASSERT_EQ(ab.get_num_available_actions(&game), 24);
    ab.apply_action_from_list(&game, 0);
    GTEST_ASSERT_EQ(game.get_cards_played()[0].get_value(true), 1);

    GTEST_ASSERT_EQ(ab.get_num_available_actions(&game), 24);
    ab.apply_action_from_list(&game, 0);
    } catch (const std::runtime_error& ex) {
        GTEST_FAIL();
    }
    GTEST_ASSERT_EQ(game.get_cards_played()[1].get_value(true), 1);
    GTEST_ASSERT_EQ(game.get_cards_played()[2].get_value(true), 1);
    GTEST_ASSERT_EQ(game.get_cards_played()[3].get_value(true), 2);
    GTEST_ASSERT_EQ(game.get_cards_played()[4].get_value(true), 2);
    GTEST_ASSERT_EQ(game.get_cards_played()[5].get_value(true), 2);
    GTEST_ASSERT_EQ(game.get_cards_played()[6].get_value(true), 4);
    GTEST_ASSERT_EQ(game.get_cards_played()[7].get_value(true), 3);


}

TEST(abstraction, play_high_low) {
    int seed = 1;

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
    simulator::cribbage game = simulator::cribbage(seed, 1, &d);
    game.setup_round();
    game.skip_to_play_phase();
    high_low_play ab = high_low_play();

    GTEST_ASSERT_EQ(ab.get_num_available_actions(&game), 2);
    ab.apply_action_from_list(&game, 0);
    GTEST_ASSERT_EQ(game.get_all_cards_played()[0].get_value(true), 1);
    GTEST_ASSERT_EQ(game.get_all_cards_played()[0].get_suit(false), 'H');
    
    GTEST_ASSERT_EQ(ab.get_num_available_actions(&game), 2);
    ab.apply_action_from_list(&game, 1);
    GTEST_ASSERT_EQ(game.get_all_cards_played()[1].get_value(true), 3);
    
    GTEST_ASSERT_EQ(ab.get_num_available_actions(&game), 2);
    ab.apply_action_from_list(&game, 1);
    GTEST_ASSERT_EQ(game.get_all_cards_played()[2].get_value(true), 4);

    GTEST_ASSERT_EQ(ab.get_num_available_actions(&game), 2);
    ab.apply_action_from_list(&game, 0);
    GTEST_ASSERT_EQ(game.get_all_cards_played()[3].get_value(true), 1);

    GTEST_ASSERT_EQ(ab.get_num_available_actions(&game), 2);
    ab.apply_action_from_list(&game, 1);
    GTEST_ASSERT_EQ(game.get_all_cards_played()[4].get_value(true), 2);

    GTEST_ASSERT_EQ(ab.get_num_available_actions(&game), 2);
    ab.apply_action_from_list(&game, 1);
    GTEST_ASSERT_EQ(game.get_all_cards_played()[5].get_value(true), 2);
    GTEST_ASSERT_EQ(game.get_all_cards_played()[5].get_suit(false), 'S');

    GTEST_ASSERT_EQ(ab.get_num_available_actions(&game), 1);
    ab.apply_action_from_list(&game, 0);
    GTEST_ASSERT_EQ(game.get_all_cards_played()[6].get_value(true), 1);
    
    GTEST_ASSERT_EQ(ab.get_num_available_actions(&game), 1);
    ab.apply_action_from_list(&game, 0);
    GTEST_ASSERT_EQ(game.get_all_cards_played()[7].get_value(true), 2);

}


TEST(abstraction, new_initial_state) {
    GTEST_SKIP();
    int seed = 1;

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
    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(seed, &p1, &p2, 1, &d);
    simulator::cribbage ga1 = simulator::cribbage(game);
    game.setup_round();

    std::shared_ptr<simulator::cribbage> g1 = std::make_shared<simulator::cribbage>(game);

    action_abstraction* act_ab = new play_card_from_hand();
    information_abstraction* info_ab = new new_stack_and_hand_cards();
    information_abstraction* ptr1 = new new_stack_and_hand_cards(dynamic_cast<new_stack_and_hand_cards const &>(*info_ab));
    std::shared_ptr<information_abstraction> ptr2 = std::make_shared<new_stack_and_hand_cards>(static_cast<new_stack_and_hand_cards const &>(*info_ab));

    std::shared_ptr<information_abstraction> info_ab_clone = info_ab->clone();
    GTEST_ASSERT_EQ(info_ab->get_informationstate_string(&game, 1), info_ab_clone->get_informationstate_string(&game, 1));

    GTEST_ASSERT_EQ(game.get_num_available_actions(), g1->get_num_available_actions());

    game.setup_round();
    game.skip_to_play_phase();
    new_stack_4_choose_1_abstraction ab = new_stack_4_choose_1_abstraction(std::make_shared<simulator::cribbage>(game));

    ab.new_initial_state();

    abstraction* ab1 = new new_stack_4_choose_1_abstraction(ab);

    GTEST_ASSERT_EQ(ab.get_num_available_actions(), game.get_num_available_actions());
    GTEST_ASSERT_EQ(ab.get_num_available_actions(), ab1->get_num_available_actions());
    GTEST_ASSERT_EQ(ab.get_informationstate_string(1), ab1->get_informationstate_string(1));

    abstraction* ab2 = new abstraction(ab);

    GTEST_ASSERT_EQ(ab.get_num_available_actions(), ab2->get_num_available_actions());
    GTEST_ASSERT_EQ(ab.get_informationstate_string(1), ab2->get_informationstate_string(1));


    abstraction* ab3 = new abstraction(std::make_shared<simulator::cribbage>(game),
                                      info_ab->clone(), 
                                      act_ab->clone()
                                      );

    GTEST_ASSERT_EQ(ab.get_num_available_actions(), ab3->get_num_available_actions());
    GTEST_ASSERT_EQ(ab.get_informationstate_string(1), ab3->get_informationstate_string(1));

    std::shared_ptr<simulator::cribbage> game_ptr = std::make_shared<simulator::cribbage>(game);
    GTEST_ASSERT_EQ(game_ptr->get_num_available_actions(), game.get_num_available_actions());

    std::shared_ptr<simulator::cribbage> game_ptr2 = std::make_shared<simulator::cribbage>( *(game_ptr.get()) );
    GTEST_ASSERT_EQ(game_ptr2->get_num_available_actions(), game.get_num_available_actions());

    


    abstraction* new_state = ab.new_initial_state();
    GTEST_ASSERT_EQ(ab.get_num_available_actions(), new_state->get_num_available_actions());
    GTEST_ASSERT_EQ(ab.get_informationstate_string(1), new_state->get_informationstate_string(2));
    
    new_state->child(1);


    abstraction* new_ab = ab.new_initial_state();

    GTEST_ASSERT_EQ(ab.get_num_available_actions(), new_ab->get_num_available_actions());
    GTEST_ASSERT_EQ(ab.get_informationstate_string(1), new_ab->get_informationstate_string(2));

    new_ab->child(1);

    GTEST_ASSERT_EQ(new_state->get_num_available_actions(), new_ab->get_num_available_actions());
    GTEST_ASSERT_EQ(new_state->get_informationstate_string(1), new_ab->get_informationstate_string(1));
    GTEST_ASSERT_EQ(new_state->get_informationstate_string(2), new_ab->get_informationstate_string(2));

    game.reset(1);
    game.setup_round();
    game.skip_to_play_phase();

    determenisticpolicy dp = determenisticpolicy(0);
    std::shared_ptr<information_abstraction> hand_info_abstraction = std::make_shared<hand_cards>();
    std::shared_ptr<action_abstraction> permutation_action_abstraction = std::make_shared<play_permutation>();
    information_abstraction hand_and_stack = new_stack_and_hand_cards();
    std::shared_ptr<action_abstraction> play_ab = std::make_shared<play_card_from_hand>();

    meta_abstraction mt = meta_abstraction(std::make_shared<simulator::cribbage>(game), 1);
    mt.add_policy(&dp, hand_info_abstraction, play_ab);
    mt.add_policy(&dp, hand_info_abstraction, permutation_action_abstraction);
    mt.add_policy(&dp, hand_info_abstraction, permutation_action_abstraction);

    std::shared_ptr<action_abstraction> permutation_copy = permutation_action_abstraction->clone();

    abstraction* ab_copy= mt.child(2);
    GTEST_ASSERT_FALSE(ab_copy->is_chance_node());
    abstraction* ab_copy_2 = ab_copy->child(2);

    GTEST_ASSERT_FALSE(mt.is_chance_node());
    GTEST_ASSERT_FALSE(ab_copy->is_chance_node());
    GTEST_ASSERT_TRUE(ab_copy_2->is_chance_node());
    GTEST_ASSERT_EQ(mt.get_current_player(), 2);
    GTEST_ASSERT_EQ(ab_copy->get_current_player(), 1);
    
    ab_copy= mt.child(0);
    ab_copy_2 = ab_copy->child(0);

    GTEST_ASSERT_FALSE(mt.is_chance_node());
    GTEST_ASSERT_FALSE(ab_copy->is_chance_node());
    GTEST_ASSERT_TRUE(ab_copy_2->is_chance_node());
    GTEST_ASSERT_EQ(mt.get_current_player(), 2);
    GTEST_ASSERT_EQ(ab_copy->get_current_player(), 1);
    
    
    ab_copy= mt.child(0);
    ab_copy_2 = ab_copy->child(1);

    GTEST_ASSERT_FALSE(mt.is_chance_node());
    GTEST_ASSERT_FALSE(ab_copy->is_chance_node());
    GTEST_ASSERT_TRUE(ab_copy_2->is_chance_node());
    GTEST_ASSERT_EQ(mt.get_current_player(), 2);
    GTEST_ASSERT_EQ(ab_copy->get_current_player(), 1);

    std::mt19937 gen(1);
    while (ab_copy_2->is_chance_node()) {
        std::vector<double> probs = ab_copy_2->chance_outcomes();
        std::discrete_distribution<int> d(probs.begin(), probs.end());
        int sampled_action_index = d(gen);
        ab_copy_2->apply_action_from_list(sampled_action_index);
    }
    
    GTEST_ASSERT_FALSE(mt.is_terminal());
    GTEST_ASSERT_FALSE(ab_copy->is_terminal());
    GTEST_ASSERT_TRUE(ab_copy_2->is_terminal());


}


TEST(abstraction, meta_play_clone) {
    int seed = 1;

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
    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(seed, &p1, &p2, 1, &d);
    game.setup_round();
    game.skip_to_play_phase();

    determenisticpolicy dp = determenisticpolicy(0);
    std::shared_ptr<information_abstraction> hand_info_abstraction = std::make_shared<hand_cards>();
    std::shared_ptr<action_abstraction> permutation_action_abstraction = std::make_shared<play_permutation>();
    information_abstraction hand_and_stack = new_stack_and_hand_cards();
    action_abstraction play_ab = play_card_from_hand();

    meta_play mt = meta_play(0);
    mt.add_policy(&dp, hand_info_abstraction, permutation_action_abstraction);

    std::shared_ptr<action_abstraction> permutation_copy = permutation_action_abstraction->clone();

    std::shared_ptr<action_abstraction> ab_copy= mt.clone();
    simulator::cribbage g_copy1 = game;
    ab_copy->apply_action_from_list(&g_copy1, 0);
    GTEST_ASSERT_TRUE(ab_copy->is_chance_node(&g_copy1));
    ab_copy->apply_action_from_list(&g_copy1, 0);
    GTEST_ASSERT_FALSE(ab_copy->is_chance_node(&g_copy1));

    std::shared_ptr<action_abstraction> ab_copy_2 = ab_copy->clone();
    simulator::cribbage g_copy2 = game;
    ab_copy_2->apply_action_from_list(&g_copy2, 0);
    GTEST_ASSERT_FALSE(ab_copy_2->is_chance_node(&g_copy2));

    try {
        ab_copy->apply_action_from_list(&g_copy1, 0);
        GTEST_ASSERT_TRUE(ab_copy->is_chance_node(&g_copy1));
        
        GTEST_ASSERT_FALSE(mt.is_chance_node(&game));
        mt.apply_action_from_list(&game, 0);
        GTEST_ASSERT_FALSE(mt.is_chance_node(&game));
        
        mt.apply_action_from_list(&game, 0);
        GTEST_ASSERT_TRUE(mt.is_chance_node(&game));

    } catch(std::runtime_error e) {
        std::cout << e.what() << std::endl;
        GTEST_FAIL() << "Program Should not crash here";
    }

    
    try {
        ab_copy->apply_action_from_list(&g_copy1, 0);
        GTEST_FAIL() << "Should fail here";
    } catch(std::runtime_error e) {
        std::cerr << e.what() << std::endl;
    }   


}



TEST(abstraction, state_based_player_copy) {
    int seed = 1;

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
    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(seed, &p1, &p2, 1, &d);
    game.setup_round();
    game.skip_to_play_phase();

    std::shared_ptr<simulator::cribbage> g1 = std::make_shared<simulator::cribbage>(game);

    action_abstraction* act_ab = new play_permutation();
    information_abstraction* info_ab = new hand_cards();

    abstraction* ab = new hand_cards_choose_24(g1);

    abstraction* ab2 = ab->child(0);

    abstraction* ab3 = ab->child(1);
    abstraction* ab4 = ab->child(2);

    abstraction* ab22 = ab2->child(0);

    
}


TEST(abstraction, hand_and_last_card_and_sum) {
    int seed = 1;

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
    game.setup_round();
    game.skip_to_play_phase();

    std::shared_ptr<simulator::cribbage> g1 = std::make_shared<simulator::cribbage>(game);

    hand_and_last_card_and_sum last_card = hand_and_last_card_and_sum();

    std::string key = last_card.get_informationstate_string(&game, 1);
    std::string key2 = last_card.get_informationstate_string(&game, 2);

    GTEST_ASSERT_EQ(key, "2_2_10_11_|0_");
    GTEST_ASSERT_EQ(key2, "1_4_7_11_|0_");

    game.set_action_for_player(2);
    game.resolve_action();

    key = last_card.get_informationstate_string(&game, 1);
    key2 = last_card.get_informationstate_string(&game, 2);

    GTEST_ASSERT_EQ(key, "2_2_10_11_|1_1_");
    GTEST_ASSERT_EQ(key2, "4_7_11_|1_1_");

    game.set_action_for_player(1);
    game.resolve_action();

    key = last_card.get_informationstate_string(&game, 1);
    key2 = last_card.get_informationstate_string(&game, 2);

    GTEST_ASSERT_EQ(key, "2_10_11_|2_3_");
    GTEST_ASSERT_EQ(key2, "4_7_11_|2_3_");
}


TEST(abstraction, hand_and_last_card) {
    int seed = 1;

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
    game.setup_round();
    game.skip_to_play_phase();

    std::shared_ptr<simulator::cribbage> g1 = std::make_shared<simulator::cribbage>(game);

    hand_and_last_card last_card = hand_and_last_card();

    std::string key = last_card.get_informationstate_string(&game, 1);
    std::string key2 = last_card.get_informationstate_string(&game, 2);

    GTEST_ASSERT_EQ(key, "4_2_2_10_11_|");
    GTEST_ASSERT_EQ(key2, "4_1_4_7_11_|");

    game.set_action_for_player(2);
    game.resolve_action();

    key = last_card.get_informationstate_string(&game, 1);
    key2 = last_card.get_informationstate_string(&game, 2);

    GTEST_ASSERT_EQ(key, "4_2_2_10_11_|1_");
    GTEST_ASSERT_EQ(key2, "4_4_7_11_|1_");


    game.set_action_for_player(1);
    game.resolve_action();

    key = last_card.get_informationstate_string(&game, 1);
    key2 = last_card.get_informationstate_string(&game, 2);

    GTEST_ASSERT_EQ(key, "3_2_10_11_|2_");
    GTEST_ASSERT_EQ(key2, "3_4_7_11_|2_");
}


TEST(abstraction, br_abstraction) {
    int seed = 1;

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

    new_stack_4_choose_1_abstraction ab = new_stack_4_choose_1_abstraction(game_ptr);

    br_abstraction br_ab = br_abstraction(game_ptr, &ab);

    //todo finnish
    
}


TEST(abstraction, head_to_head_different_abstractions) {
    GTEST_SKIP();
    int seed = 1;
    std::mt19937 gen(seed);
    std::string run_name = "head_to_head_different_abstractions";
    std::string log_name = run_name +"_log.txt";

    std::ofstream logger(log_name);
    mockplayer p1 = mockplayer();
    mockplayer p2 = mockplayer();
    simulator::cribbage game = simulator::cribbage(seed, &p1, &p2, 1);

    std::shared_ptr<simulator::cribbage> game_ptr = std::make_shared<simulator::cribbage>(game);
    
    const int n = 5;
    policy* policies[n];
    abstraction* abstrs[n];
    policies[0] = new policy();
    policies[0]->deserialize("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_new_stack_4_choose_1_abstraction_1000000.bin");
    policies[0]->name = "52_card_deck_mccfr_policy_new_stack_4_choose_1_abstraction_1000000";
    abstrs[0] = new new_stack_4_choose_1_abstraction(game_ptr);
    policies[1] = new policy();
    policies[1]->deserialize("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_high_low_abstraction_1000000.bin");
    policies[1]->name = "52_card_deck_mccfr_policy_high_low_abstraction_1000000";
    abstrs[1] = new high_low_abstraction(game_ptr);
    policies[2] = new policy();
    policies[2]->deserialize("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_last_card_sum_abstraction_1000000.bin");
    policies[2]->name = "52_card_deck_mccfr_policy_hand_last_card_sum_abstraction_1000000";
    abstrs[2] = new hand_last_card_sum_abstraction(game_ptr);
    policies[3] = new policy();
    policies[3]->deserialize("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_last_card_abstraction_1000000.bin");
    policies[3]->name = "52_card_deck_mccfr_policy_hand_last_card_abstraction_1000000";
    abstrs[3] = new hand_last_card_abstraction(game_ptr);
    policies[4] = new policy();
    policies[4]->deserialize("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_cards_choose_24_1000000.bin");
    policies[4]->name = "52_card_deck_mccfr_policy_hand_cards_choose_24_1000000";
    abstrs[4] = new hand_cards_choose_24(game_ptr);


    statistics stats = statistics();
    std::vector<std::vector<double>> head_to_head_policy = std::vector<std::vector<double>>();


    for(int i=0; i<n;i++) {
        head_to_head_policy.push_back(std::vector<double>());
        for (int j = 0; j<n; j++) {
            head_to_head_policy[i].push_back(0.0);
        }
    }
    int num_test = 100'000;
    for(int i=0; i<n; i++) {
        for(int j=i; j<n; j++) {
            head_to_head_policy[i][j] = stats.head_to_head(&game, policies[i], policies[j], abstrs[i], abstrs[j], num_test, &gen, &logger);
            logger << "head-to-head policy " << policies[i]->name << " vs. policy " << policies[j]->name << ": " << head_to_head_policy[i][j] << std::endl;
        }
        logger.flush();
    }

    logger.close();

}