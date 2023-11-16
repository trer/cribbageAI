#pragma once
#include "./card.h"
#include "./deck.h"
#include "./hand.h"
#include "./player.h"
#include "./scorer.h"
#include"./utils.h"
#include <iostream>

class cribbage {
    private:
        deck base_deck;
        deck* game_deck;
        card * played_cards;
        player *player1;
        player *player2;
        player *dealer;
        player *pone;
        player *current_player;
        player *current_opp;
        //hand *current_player_hand_mirror;
        //hand *current_opp_hand_mirror;
        hand crib;
        card *cut_card;
        int *dealer_score;
        int *pone_score;
        int *current_player_score;
        int *current_opp_score;
        int player1_score;
        int player2_score;
        int seed;
        bool player1_ready;
        bool player2_ready;
        int wins[2];

        int current_dealer;

        std::mt19937 gen;
        std::uniform_int_distribution<int> distrib;

        
        void swap_dealer();
        void set_current_player(bool pone_to_play);
        
        int play_phase();
        int matching();
        
        int check_win();
        void reset(int first_dealer=0);

    
    public:
        int round();
        cribbage(int seed, int first_dealer=0, deck* in_deck=nullptr);
        cribbage(int seed, player *player1, player *player2, int first_dealer=0, deck* in_deck=nullptr);
        void init(int first_dealer=0, deck* in_deck=nullptr);
        void set_deck(deck* in_deck);
        void set_player(player *player, int num);
        int start_game();
        int* start_games(int num_games);

        int get_player1_score();
        int get_player2_score();

        

};
