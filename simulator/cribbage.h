#pragma once
#include "./card.h"
#include "./deck.h"
#include "./hand.h"
#include "./player.h"
#include "./scorer.h"

class cribbage {
    private:
        deck game_deck;
        card * played_cards;
        player *player1;
        player *player2;
        player *dealer;
        player *pone;
        hand crib;
        card *cut_card;
        int *dealer_score;
        int *pone_score;
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
        int play_phase();
        int matching();
        int round();
        int check_win();
        void reset();

    
    public:
        cribbage(int seed);
        void init();
        void set_player(player *player, int num);
        cribbage(int seed, player *player1, player *player2);
        int start_game();
        int* start_games(int num_games);

        

};
