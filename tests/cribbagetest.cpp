#pragma once
#include <gtest/gtest.h>
#include "../simulator/cribbage.h"
#include "../simulator/player.h"
#include "../simulator/greedyplayer.h"

TEST(cribbage, test_mocks) {
    
    mockplayer p1 = mockplayer();
    mockplayer p2 = mockplayer();
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1);

    mock_deck deck = mock_deck();
    
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
    player_and_opp_hand[8] = card(13, 'C');
    player_and_opp_hand[9] = card(5, 'S');
    player_and_opp_hand[10] = card(6, 'H');
    player_and_opp_hand[11] = card(7, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');

    card cards_played[8];
    card crib[5];

    //set order for both players
    p1.order[0] = 4;
    p1.order[1] = 5;
    p1.order[2] = 3;
    p1.order[3] = 2;
    p1.order[4] = 1;
    p1.order[5] = 0;

    p2.order[0] = 4;
    p2.order[1] = 5;
    p2.order[2] = 3;
    p2.order[3] = 2;
    p2.order[4] = 1;
    p2.order[5] = 0;
    
    

    deck.set_top_13_cards(player_and_opp_hand);
    game.set_deck(&deck);
    int winner = game.round(); 

    GTEST_ASSERT_EQ(winner, 0);
    GTEST_ASSERT_EQ(game.get_player1_score(), 19);
    GTEST_ASSERT_EQ(game.get_player2_score(), 11);

    winner = game.round();
    
    GTEST_ASSERT_EQ(winner, 0);
    GTEST_ASSERT_EQ(game.get_player1_score(), 30);
    GTEST_ASSERT_EQ(game.get_player2_score(), 30);

    winner = game.round();
    
    GTEST_ASSERT_EQ(winner, 0);
    GTEST_ASSERT_EQ(game.get_player1_score(), 49);
    GTEST_ASSERT_EQ(game.get_player2_score(), 41);

    winner = game.round();
    
    GTEST_ASSERT_EQ(winner, 0);
    GTEST_ASSERT_EQ(game.get_player1_score(), 60);
    GTEST_ASSERT_EQ(game.get_player2_score(), 60);

    winner = game.round();
    
    GTEST_ASSERT_EQ(winner, 0);
    GTEST_ASSERT_EQ(game.get_player1_score(), 79);
    GTEST_ASSERT_EQ(game.get_player2_score(), 71);

    winner = game.round();
    
    GTEST_ASSERT_EQ(winner, 0);
    GTEST_ASSERT_EQ(game.get_player1_score(), 90);
    GTEST_ASSERT_EQ(game.get_player2_score(), 90);

    winner = game.round();
    
    GTEST_ASSERT_EQ(winner, 0);
    GTEST_ASSERT_EQ(game.get_player1_score(), 109);
    GTEST_ASSERT_EQ(game.get_player2_score(), 101);

    winner = game.round();
    
    GTEST_ASSERT_EQ(winner, 0);
    GTEST_ASSERT_EQ(game.get_player1_score(), 120);
    GTEST_ASSERT_EQ(game.get_player2_score(), 120);

    winner = game.round();
    
    GTEST_ASSERT_EQ(winner, 1);
    GTEST_ASSERT_EQ(game.get_player1_score(), 122);
    GTEST_ASSERT_EQ(game.get_player2_score(), 120);

}

TEST(cribbage, test_cribbage) {
    
    mockplayer p1 = mockplayer();
    mockplayer p2 = mockplayer();
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1);

    mock_deck deck = mock_deck();
    
    card player_and_opp_hand[13];

    //player hand
    player_and_opp_hand[0] = card(13, 'D');
    player_and_opp_hand[1] = card(3, 'D');
    player_and_opp_hand[2] = card(7, 'D');
    player_and_opp_hand[3] = card(5, 'C');
    player_and_opp_hand[4] = card(10, 'S');
    player_and_opp_hand[5] = card(11, 'S');
    //opp hand
    player_and_opp_hand[6] = card(4, 'H');
    player_and_opp_hand[7] = card(4, 'S');
    player_and_opp_hand[8] = card(4, 'D');
    player_and_opp_hand[9] = card(3, 'S');
    player_and_opp_hand[10] = card(11, 'H');
    player_and_opp_hand[11] = card(7, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');

    card cards_played[8];
    card crib[5];

    //set order for both players
    p1.order[0] = 4;
    p1.order[1] = 5;
    p1.order[2] = 3;
    p1.order[3] = 2;
    p1.order[4] = 1;
    p1.order[5] = 0;

    p2.order[0] = 4;
    p2.order[1] = 5;
    p2.order[2] = 3;
    p2.order[3] = 2;
    p2.order[4] = 1;
    p2.order[5] = 0;
    
    

    deck.set_top_13_cards(player_and_opp_hand);
    game.set_deck(&deck);
    int winner = game.round(); 

    GTEST_ASSERT_EQ(winner, 0);
    GTEST_ASSERT_EQ(game.get_player1_score(), 10);
    GTEST_ASSERT_EQ(game.get_player2_score(), 21);


   //opp hand
    player_and_opp_hand[0] = card(4, 'H');
    player_and_opp_hand[1] = card(4, 'S');
    player_and_opp_hand[2] = card(4, 'D');
    player_and_opp_hand[3] = card(3, 'S');
    player_and_opp_hand[4] = card(11, 'H');
    player_and_opp_hand[5] = card(7, 'H');

    //player hand
    player_and_opp_hand[6] = card(13, 'D');
    player_and_opp_hand[7] = card(3, 'D');
    player_and_opp_hand[8] = card(7, 'D');
    player_and_opp_hand[9] = card(5, 'C');
    player_and_opp_hand[10] = card(10, 'S');
    player_and_opp_hand[11] = card(11, 'S');

    deck.set_top_13_cards(player_and_opp_hand);

    winner = game.round();
 
    GTEST_ASSERT_EQ(winner, 0);
    GTEST_ASSERT_EQ(game.get_player1_score(), 10+9);
    GTEST_ASSERT_EQ(game.get_player2_score(), 21+23);

    winner = game.round();
    
    GTEST_ASSERT_EQ(winner, 0);
    GTEST_ASSERT_EQ(game.get_player1_score(), 10+9+23);
    GTEST_ASSERT_EQ(game.get_player2_score(), 21+23+9);

    winner = game.round();
    
    GTEST_ASSERT_EQ(winner, 0);
    GTEST_ASSERT_EQ(game.get_player1_score(), 10+9+23+9);
    GTEST_ASSERT_EQ(game.get_player2_score(), 21+23+9+23);

    winner = game.round();
    
    GTEST_ASSERT_EQ(winner, 0);
    GTEST_ASSERT_EQ(game.get_player1_score(), 10+9+23+9+23);
    GTEST_ASSERT_EQ(game.get_player2_score(), 21+23+9+23+9);


}


TEST(cribbage, endgame_test) {
    
    mockplayer p1 = mockplayer();
    mockplayer p2 = mockplayer();
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1);

    mock_deck deck = mock_deck();
    
    card player_and_opp_hand[13];

    //player hand
    player_and_opp_hand[0] = card(13, 'D');
    player_and_opp_hand[1] = card(3, 'D');
    player_and_opp_hand[2] = card(7, 'D');
    player_and_opp_hand[3] = card(5, 'C');
    player_and_opp_hand[4] = card(10, 'S');
    player_and_opp_hand[5] = card(11, 'S');
    //opp hand
    player_and_opp_hand[6] = card(4, 'H');
    player_and_opp_hand[7] = card(4, 'S');
    player_and_opp_hand[8] = card(4, 'D');
    player_and_opp_hand[9] = card(3, 'S');
    player_and_opp_hand[10] = card(11, 'H');
    player_and_opp_hand[11] = card(7, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');

    card cards_played[8];
    card crib[5];

    //set order for both players
    p1.order[0] = 4;
    p1.order[1] = 5;
    p1.order[2] = 3;
    p1.order[3] = 2;
    p1.order[4] = 1;
    p1.order[5] = 0;

    p2.order[0] = 4;
    p2.order[1] = 5;
    p2.order[2] = 3;
    p2.order[3] = 2;
    p2.order[4] = 1;
    p2.order[5] = 0;
    
    

    deck.set_top_13_cards(player_and_opp_hand);
    game.set_deck(&deck);



    for (int i = 0; i < 7; i++)
    {
        game.round();
    }
    
    GTEST_ASSERT_EQ(game.get_player1_score(), 103);
    GTEST_ASSERT_EQ(game.get_player2_score(), 114);

    //This round both players would reach 124 points if we played to the end of the round.
    int winner = game.round();

    GTEST_ASSERT_EQ(winner, 1);
    GTEST_ASSERT_EQ(game.get_player1_score(), 124);
    GTEST_ASSERT_EQ(game.get_player2_score(), 114+1);
    
}

TEST(cribbage, predictability_test) {

    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage g1 = simulator::cribbage(1, &p1, &p2, 1);
    simulator::cribbage g2 = simulator::cribbage(1, &p1, &p2, 1);
    
    g1.setup_round();
    g2.setup_round();

    for (int i = 0; i < g1.get_player_hand_size(1); i++) {
        GTEST_ASSERT_EQ(g1.get_player_hand(1)->get_card(i)->get_value(true), g2.get_player_hand(1)->get_card(i)->get_value(true));
        GTEST_ASSERT_EQ(g1.get_player_hand(2)->get_card(i)->get_value(true), g2.get_player_hand(2)->get_card(i)->get_value(true));
    }
    
    g1.round();
    g2.round();

    g1.setup_round();
    g2.setup_round();

    for (int i = 0; i < g1.get_player_hand_size(1); i++) {
        GTEST_ASSERT_EQ(g1.get_player_hand(1)->get_card(i)->get_value(true), g2.get_player_hand(1)->get_card(i)->get_value(true));
        GTEST_ASSERT_EQ(g1.get_player_hand(2)->get_card(i)->get_value(true), g2.get_player_hand(2)->get_card(i)->get_value(true));
    }

    g1.set_action_for_player(1);
    g1.set_action_for_player(2);
    g2.set_action_for_player(1);
    g2.set_action_for_player(2);
    
    for (int i = 0; i < g1.get_player_hand_size(1); i++) {
        GTEST_ASSERT_EQ(g1.get_player_hand(1)->get_card(i)->get_value(true), g2.get_player_hand(1)->get_card(i)->get_value(true));
        GTEST_ASSERT_EQ(g1.get_player_hand(2)->get_card(i)->get_value(true), g2.get_player_hand(2)->get_card(i)->get_value(true));
    }

    g1.set_action_for_player(2);
    g2.set_action_for_player(2);

    for (int i = 0; i < g1.get_player_hand_size(1); i++) {
        GTEST_ASSERT_EQ(g1.get_player_hand(1)->get_card(i)->get_value(true), g2.get_player_hand(1)->get_card(i)->get_value(true));
        GTEST_ASSERT_EQ(g1.get_player_hand(2)->get_card(i)->get_value(true), g2.get_player_hand(2)->get_card(i)->get_value(true));
    }

}


TEST(cribbage, copy_test) {

    
    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1);
    game.setup_round();

    simulator::cribbage copy = game;
    GTEST_ASSERT_EQ(game.is_discard_done(), copy.is_discard_done());

    simulator::cribbage cp1;
    cp1 = simulator::cribbage(1);

    GTEST_ASSERT_NE(game.is_discard_done(), cp1.is_discard_done());  
    int num_copies = 1;
    
    simulator::cribbage *start_copy[20];
    simulator::cribbage *past_copy[20];
    start_copy[0] = &game;
    past_copy[0] = &game;
    int rounds_counter = 0;
    int win = 0;

    while (!win) {
        //copy state of first game
        start_copy[num_copies] = new simulator::cribbage(game);
        //copy state of last game (should be the same state as the first game, but different memory locations)
        past_copy[num_copies] = new simulator::cribbage(*past_copy[num_copies-1]);
        num_copies++;

        //advance all games 1 step forward
        game.round();
        for (int i = 1; i < num_copies; i++)
        {
            win = start_copy[i]->round();
            past_copy[i]->round();

            // assert that the result is the for all games (implying that the internal gamestate is the same)
            GTEST_ASSERT_EQ(start_copy[i]->get_player1_score(), game.get_player1_score()) << "i is: " << i << ", round is: " << rounds_counter;
            GTEST_ASSERT_EQ(start_copy[i]->get_player2_score(), game.get_player2_score());
            GTEST_ASSERT_EQ(past_copy[i]->get_player1_score(), game.get_player1_score()) << "i is: " << i << ", round is: " << rounds_counter;
            GTEST_ASSERT_EQ(past_copy[i]->get_player2_score(), game.get_player2_score());
        }
        rounds_counter++;
    }

    game = simulator::cribbage(1, &p1, &p2, 1);

    game.setup_round();
    game.set_action_for_player(2);
    game.set_action_for_player(1);
    game.setup_play_phase(); //redundant

    simulator::cribbage g2 = simulator::cribbage(game);
    game.set_action_for_player(2);
    game.resolve_action();

    GTEST_ASSERT_EQ(game.get_player_hand_size(2), 3); //if no other errors prob copy error
    
    g2.set_action_for_player(2);
    g2.resolve_action();

    GTEST_ASSERT_EQ(g2.get_player_hand_size(1), game.get_player_hand_size(1));
    for (int i = 0; i < g2.get_player_hand_size(1); i++)
    {
        GTEST_ASSERT_TRUE(compare_cards(g2.get_player_hand(1)->get_card(i), game.get_player_hand(1)->get_card(i)));
    }
    
    GTEST_ASSERT_EQ(g2.get_player_hand_size(2), game.get_player_hand_size(2));
    for (int i = 0; i < g2.get_player_hand_size(2); i++)
    {
        GTEST_ASSERT_TRUE(compare_cards(g2.get_player_hand(2)->get_card(i), game.get_player_hand(2)->get_card(i)));
    }

}


TEST(cribbage, state_key_apply_action_from_list) {

    mockplayer p1 = mockplayer();
    mockplayer p2 = mockplayer();
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1);

    mock_deck deck = mock_deck();
    
    card player_and_opp_hand[13];

    //player hand
    player_and_opp_hand[0] = card(13, 'D');
    player_and_opp_hand[1] = card(3, 'D');
    player_and_opp_hand[2] = card(7, 'D');
    player_and_opp_hand[3] = card(5, 'C');
    player_and_opp_hand[4] = card(10, 'S');
    player_and_opp_hand[5] = card(11, 'S');
    //opp hand
    player_and_opp_hand[6] = card(4, 'H');
    player_and_opp_hand[7] = card(4, 'S');
    player_and_opp_hand[8] = card(4, 'D');
    player_and_opp_hand[9] = card(3, 'S');
    player_and_opp_hand[10] = card(11, 'H');
    player_and_opp_hand[11] = card(7, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');

    card cards_played[8];
    card crib[5];

    //set order for both players
    p1.order[0] = 4;
    p1.order[1] = 5;
    p1.order[2] = 3;
    p1.order[3] = 2;
    p1.order[4] = 1;
    p1.order[5] = 0;

    p2.order[0] = 4;
    p2.order[1] = 5;
    p2.order[2] = 3;
    p2.order[3] = 2;
    p2.order[4] = 1;
    p2.order[5] = 0;
    
    

    deck.set_top_13_cards(player_and_opp_hand);
    game.set_deck(&deck);

    game.setup_round();

    std::string p1_dis_key = game.get_informationstate_string(1);
    GTEST_ASSERT_EQ(p1_dis_key, "3_5_7_10_11_13_|");

    std::string p2_dis_key = game.get_informationstate_string(2);
    GTEST_ASSERT_EQ(p2_dis_key, "3_4_4_4_7_11_|");

    game.apply_action_from_list(0); //player 1 discard cards
    game.apply_action_from_list(0); //player 2 discard cards

    //might do some fancy check here if the correct action is selected but can also be viewed from the next game state
    // Hmmm, maybe I should have some safeguards to stop player 1 playing as player 2. That sounds like a painfull bug.
    // TODO: later

    p1_dis_key = game.get_informationstate_string(1);
    GTEST_ASSERT_EQ(p1_dis_key, "7_10_11_13_|");    

    p2_dis_key = game.get_informationstate_string(2);
    GTEST_ASSERT_EQ(p2_dis_key, "4_4_7_11_|");

    GTEST_ASSERT_EQ(game.get_player_hand(2)->get_card(game.get_available_action_indexes()[game.get_available_actions()[0][0]])->get_value(true) ,4);
    GTEST_ASSERT_EQ(game.get_player_hand(2)->get_card(game.get_available_action_indexes()[game.get_available_actions()[1][0]])->get_value(true) ,4);
    GTEST_ASSERT_EQ(game.get_player_hand(2)->get_card(game.get_available_action_indexes()[game.get_available_actions()[2][0]])->get_value(true) ,7);
    GTEST_ASSERT_EQ(game.get_player_hand(2)->get_card(game.get_available_action_indexes()[game.get_available_actions()[3][0]])->get_value(true) ,11);


    game.apply_action_from_list(2);

    p1_dis_key = game.get_informationstate_string(1);
    GTEST_ASSERT_EQ(p1_dis_key, "7_10_11_13_|7_");   

    p2_dis_key = game.get_informationstate_string(2);
    GTEST_ASSERT_EQ(p2_dis_key, "4_4_11_|7_");

    
    game.apply_action_from_list(0);

    p1_dis_key = game.get_informationstate_string(1);
    GTEST_ASSERT_EQ(p1_dis_key, "10_11_13_|7_7_");   

    p2_dis_key = game.get_informationstate_string(2);
    GTEST_ASSERT_EQ(p2_dis_key, "4_4_11_|7_7_");


    game.apply_action_from_list(2);

    p1_dis_key = game.get_informationstate_string(1);
    GTEST_ASSERT_EQ(p1_dis_key, "10_11_13_|7_7_11_");   

    p2_dis_key = game.get_informationstate_string(2);
    GTEST_ASSERT_EQ(p2_dis_key, "4_4_|7_7_11_");


    game.apply_action_from_list(1);

    p1_dis_key = game.get_informationstate_string(1);
    GTEST_ASSERT_EQ(p1_dis_key, "10_11_13_|");   

    p2_dis_key = game.get_informationstate_string(2);
    GTEST_ASSERT_EQ(p2_dis_key, "4_|");


    game.apply_action_from_list(2);

    p1_dis_key = game.get_informationstate_string(1);
    GTEST_ASSERT_EQ(p1_dis_key, "10_11_|13_");   

    p2_dis_key = game.get_informationstate_string(2);
    GTEST_ASSERT_EQ(p2_dis_key, "4_|13_");


    game.apply_action_from_list(0);

    p1_dis_key = game.get_informationstate_string(1);
    GTEST_ASSERT_EQ(p1_dis_key, "10_11_|13_4_");   

    p2_dis_key = game.get_informationstate_string(2);
    GTEST_ASSERT_EQ(p2_dis_key, "|13_4_");


    game.apply_action_from_list(0);

    p1_dis_key = game.get_informationstate_string(1);
    GTEST_ASSERT_EQ(p1_dis_key, "11_|");   

    p2_dis_key = game.get_informationstate_string(2);
    GTEST_ASSERT_EQ(p2_dis_key, "|");


    int il = game.apply_action_from_list(0);

    p1_dis_key = game.get_informationstate_string(1);
    GTEST_ASSERT_EQ(p1_dis_key, "|11_");   

    p2_dis_key = game.get_informationstate_string(2);
    GTEST_ASSERT_EQ(p2_dis_key, "|11_");


    GTEST_ASSERT_EQ(game.is_playphase_done(), true);

    int illigal = game.apply_action_from_list(0);

    GTEST_ASSERT_EQ(illigal, -99);

    p1_dis_key = game.get_informationstate_string(1);
    GTEST_ASSERT_EQ(p1_dis_key, "|11_");   

    p2_dis_key = game.get_informationstate_string(2);
    GTEST_ASSERT_EQ(p2_dis_key, "|11_");



}

TEST(cribbage, skip_to_playphase) {

    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1);

    mock_deck deck = mock_deck();
    
    card player_and_opp_hand[13];

    //player hand
    player_and_opp_hand[0] = card(13, 'D');
    player_and_opp_hand[1] = card(3, 'D');
    player_and_opp_hand[2] = card(7, 'D');
    player_and_opp_hand[3] = card(5, 'C');
    player_and_opp_hand[4] = card(10, 'S');
    player_and_opp_hand[5] = card(11, 'S');
    //opp hand
    player_and_opp_hand[6] = card(4, 'H');
    player_and_opp_hand[7] = card(4, 'S');
    player_and_opp_hand[8] = card(4, 'D');
    player_and_opp_hand[9] = card(3, 'S');
    player_and_opp_hand[10] = card(11, 'H');
    player_and_opp_hand[11] = card(7, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');

    card cards_played[8];
    card crib[5];

    deck.set_top_13_cards(player_and_opp_hand);
    game.set_deck(&deck);

    game.setup_round();
    game.skip_to_play_phase();

    std::string p1_dis_key = game.get_informationstate_string(1);
    GTEST_ASSERT_EQ(p1_dis_key, "5_10_11_13_|");

    std::string p2_dis_key = game.get_informationstate_string(2);
    GTEST_ASSERT_EQ(p2_dis_key, "4_4_4_7_|");

    int num = game.get_num_available_actions();
    int win = game.apply_action_from_list(4);

    GTEST_ASSERT_EQ(num, 4);
    GTEST_ASSERT_EQ(win, -99);
    
    win = game.apply_action_from_list(3);

    p2_dis_key = game.get_informationstate_string(2);
    GTEST_ASSERT_EQ(p2_dis_key, "4_4_4_|7_");

}

TEST(cribbage, cribbage_order) {

    greedyplayer p1 = greedyplayer();
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(1, &p1, &p2, 1);

    mock_deck deck = mock_deck();
    
    card player_and_opp_hand[13];

    //player hand
    player_and_opp_hand[0] = card(13, 'D');
    player_and_opp_hand[1] = card(3, 'D');
    player_and_opp_hand[2] = card(7, 'D');
    player_and_opp_hand[3] = card(5, 'C');
    player_and_opp_hand[4] = card(10, 'S');
    player_and_opp_hand[5] = card(11, 'S');
    //opp hand
    player_and_opp_hand[6] = card(4, 'H');
    player_and_opp_hand[7] = card(4, 'S');
    player_and_opp_hand[8] = card(4, 'D');
    player_and_opp_hand[9] = card(3, 'S');
    player_and_opp_hand[10] = card(11, 'H');
    player_and_opp_hand[11] = card(7, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');

    card cards_played[8];
    card crib[5];

    deck.set_top_13_cards(player_and_opp_hand);
    game.set_deck(&deck);

    game.setup_round();

    game.set_action_for_player(2);
    game.set_action_for_player(1);

    GTEST_ASSERT_TRUE(game.is_discard_done());
    GTEST_ASSERT_EQ(game.get_current_player(), 2);

    game.reset();
    game.setup_round();

    GTEST_ASSERT_FALSE(game.is_discard_done());

    game.set_action_for_player(1);
    game.set_action_for_player(2);

    GTEST_ASSERT_TRUE(game.is_discard_done());
    GTEST_ASSERT_EQ(game.get_current_player(), 1);

    game.reset();
    game.setup_round();

    game.set_action_for_player(1);
    game.set_action_for_player(2);

    GTEST_ASSERT_TRUE(game.is_discard_done());
    GTEST_ASSERT_EQ(game.get_current_player(), 2);

    simulator::cribbage g2 = simulator::cribbage(1, &p1, &p2, 1);

    g2.setup_round();
    g2.set_action_for_player(2);
    g2.set_action_for_player(1);
    g2.setup_play_phase(); //redundant

    g2.set_action_for_player(2);
    g2.resolve_action();
}