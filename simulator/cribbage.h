#pragma once
#include "./card.h"
#include "./deck.h"
#include "./hand.h"
#include "./player.h"
#include "./scorer.h"
#include"./utils.h"

#include <cstdlib>
#include <iostream>


namespace simulator {

class cribbage {
    private:
        deck base_deck;
        deck* game_deck;

        player *player1;
        player *player2;   
        player *dealer;
        player *pone;  
        player *current_player;
        player *current_opp;

        card crib_cards[4]; //this can be placed directly into crib hand
        hand crib;
        hand dealer_hand;
        hand pone_hand;
        hand *current_hand;
        hand *current_opp_hand;
        card *cut_card;
        card cards_played[8];
        card cards_played_since_new_stack[8];

        action pone_discards;
        action dealer_discard;
        action acting_player_action;

        int player1_score;
        int player2_score;
        int *dealer_score;
        int *pone_score;
        int *current_player_score;
        int *current_opp_score;
        
        int seed;

        bool player1_ready;
        bool player2_ready;
        bool dealer_discard_done;
        bool pone_discard_done;
        bool round_done = false;
        bool round_setup_once = false;

        std::mt19937 gen;
        std::uniform_int_distribution<int> distrib;
        
        int first_dealer = 0;
        int next_dealer;
        int num_cards_played = 0;
        int num_cards_played_since_new_stack = 0;
        int sum_cards = 0;
        bool pone_to_play = true;
        bool pone_go = false;
        bool dealer_go = false;
        
        int num_available_actions;
        int available_actions_indexes[15] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
        int* available_actions[15];
    
        void copy_self(const cribbage &source);
        
        void init(int first_dealer=0, deck* in_deck=nullptr);

        int play_phase();
        
        void set_discard(action a, char player);
        
        int check_win();
        void swap_dealer();
        void set_dealer_and_pone(int current_dealer);
        void set_current_player();
        void last_card_played();
    
    public:

        cribbage();
        cribbage(const cribbage &source);
        cribbage(int seed, int first_dealer=0, deck* in_deck=nullptr);
        cribbage(int seed, player *player1, player *player2, int first_dealer=0, deck* in_deck=nullptr);
        const cribbage& operator= (const cribbage&);
    
        int round();
        int start_game();
        int start_games(int num_games);

        
        void reset(int f_dealer=-1);
        void setup_round();
        void setup_play_phase();
        void matching_setup();

        void skip_to_play_phase(player* discard_policy=nullptr);

        //I would like to remove these, but that is a long dream.
        int resolve_action();
        int handle_discards();
        int matching();
        int matching_score_pone();
        int matching_score_dealer();
        
        void set_deck(deck* in_deck);
        void set_player(player *player, int num);

        //Set_discard and set_play_action functionally the same?
        void set_discard(action a);
        void set_discard(action a, int player);
        void set_play_action(action a);
        void set_action_for_player(int player);

        int get_player1_score();
        int get_player2_score();
        int get_point_diff(int player);
        int get_current_player();
        int get_current_pone();
        bool player_has_called_go(int player);
        hand* get_player_hand(int player);
        int get_player_hand_size(int player);
        int get_num_cards_played();
        card* get_cards_played();
        int get_sum_cards_played();

        bool is_discard_done();
        bool is_playphase_done();
        bool is_round_done();

        std::string get_informationstate_string(int player);
        std::string get_informationstate_string_hand_only(int player);
        int get_num_available_actions();
        int* get_available_action_indexes();
        int** get_available_actions();
        int apply_action_from_list(int action_index); //switch players to give indexes?

        void randomize(int player); //not implemented

};

};