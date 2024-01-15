#pragma once
#include "./card.h"
#include "./deck.h"
#include "./hand.h"
#include "./player.h"
#include "./scorer.h"
#include"./utils.h"
#include <iostream>

namespace simulator {

class cribbage {
    private:
        deck base_deck;
        card * played_cards;
        player *current_opp;
        //hand *current_player_hand_mirror;
        //hand *current_opp_hand_mirror;
        hand dealer_hand;
        hand pone_hand;
        card crib_cards[4];
        card *cut_card;
        action discard2;
        action discard1;
        action acting_player_action;
        
        
        
        bool pone_to_play = true;


        int *dealer_score;
        int *pone_score;
        int *current_player_score;
        int *current_opp_score;
        
        int seed;
        bool player1_ready;
        bool player2_ready;
        int wins[2];

        int current_dealer;

        std::mt19937 gen;
        std::uniform_int_distribution<int> distrib;

        


        int play_phase();
        int matching();
        

    
    public:
        deck* game_deck;
        player *player1;
        player *player2;   
        player *dealer;
        player *pone;  
        player *current_player;
        hand crib;
        card cards_played[8];
        int num_cards_played = 0;
        int sum_cards = 0;
        bool pone_go = false;
        bool dealer_go = false;

        int player1_score;
        int player2_score;
    
        int check_win();
        void swap_dealer();
        void set_current_player();
        void setup_play_phase();
        void resolve_action();
        void last_card_played();
        void reset(int first_dealer=0);
        void setup_round();
        int handle_discards();
        void matching_setup();
        int matching_score_pone();
        int matching_score_dealer();


        int round();
        cribbage();
        cribbage(int seed, int first_dealer=0, deck* in_deck=nullptr);
        cribbage(int seed, player *player1, player *player2, int first_dealer=0, deck* in_deck=nullptr);
        cribbage(int seed, char player1, char player2, int first_dealer=0);
        void init(int first_dealer=0, deck* in_deck=nullptr);
        void set_deck(deck* in_deck);
        void set_player(player *player, int num);
        int start_game();
        int start_games(int num_games);

        int get_player1_score();
        int get_player2_score();

        hand* get_player_hand(int player);
        int get_player_hand_size(int player);
        void set_discard(action a, int player);
        void set_play_action(action a);

};

};