#pragma once
#include "./hand.h"
#include "./action.h"
#include "./utils.h"



class player {
    protected:
        hand *player_hand;        
        bool find_legal_moves(int sum_cards_played);
        int legal_moves[6];
        card c1;
        card c2;

    public:
        player();
        
        virtual action poll_player(bool discard_phase, hand* p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer) = 0;
        void set_hand(hand *new_hand);
        hand *get_hand();

        

};

class randomplayer : public player{

    private:
        action discard_two_random_cards();
        action play_random_card();

    public:
        randomplayer();
        action poll_player(bool discard_phase, hand* p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer);     

};


class realplayer : public player{

    private:
        action discard_two_cards();
        action play_a_card(int sum_played_cards);

    public:
        realplayer();
        action poll_player(bool discard_phase, hand* p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer);     

};

class mockplayer : public player{


    public:
        int order[6];
        int count;
        mockplayer();
        action poll_player(bool discard_phase, hand* p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer);     

};


class policyplayer : public player {

    private:
        policy precomputed_policy = policy();
        randomplayer r = randomplayer();
        std::mt19937 gen;

        bool policy_loaded = false;
        int num_available_actions;
        int available_actions_indexes[15] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
        int* available_actions[15];

        
        std::string get_informationstate_string(bool discard_phase, hand* p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer);


    public:
        policyplayer();
        policyplayer(std::string filepath, int inseed);

        bool load_policy(std::string filepath);
        void set_seed(int in_seed);
        action poll_player(bool discard_phase, hand* p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer);     

};