#pragma once
#include <gtest/gtest.h>
#include ".././simulator/cribbage.h"
#include ".././simulator/player.h"

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
