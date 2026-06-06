#pragma once
#include "./hand.h"
#include "./action.h"
#include "./utils.h"



class player {
    protected:
        hand *player_hand = nullptr;        
        bool find_legal_moves(int sum_cards_played);
        int legal_moves[6] = {0};
        card c1 = card();
        card c2 = card();
        std::string name = "player";

    public:
        player();
        
        virtual action poll_player(bool discard_phase, hand* p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer) = 0;
        void set_hand(hand *new_hand);
        hand *get_hand();
        std::string get_name();

        

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
        card card_order[6];
        int count;
        mockplayer();
        action poll_player(bool discard_phase, hand* p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer);     
        void reset(hand* p_hand);
};


class policyplayer : public player {

    protected:
        policy precomputed_policy = policy();
        randomplayer r = randomplayer();
        std::mt19937 gen;

        bool policy_loaded = false;
        int num_available_actions;
        int available_actions_indexes[15] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
        int* available_actions[15];

        
        virtual std::string get_informationstate_string(bool discard_phase, hand* p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer);


    public:
        policyplayer(int in_seed);
        policyplayer(policy* in_policy, int in_seed);
        policyplayer(std::string filepath, int inseed);

        bool load_policy(std::string filepath);
        int size_of_policy();
        void set_seed(int in_seed);
        action poll_player(bool discard_phase, hand* p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer);     

};

class highlowpolicyplayer : public policyplayer {


    public:
        highlowpolicyplayer(int in_seed);
        highlowpolicyplayer(policy* in_policy, int in_seed);
        highlowpolicyplayer(std::string filepath, int inseed);

        action poll_player(bool discard_phase, hand* p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer);     

};



class permutationpolicyplayer : public policyplayer {

    private:
        bool actions_set = false;
        int action_counter = 0;
        card player_actions[4];

        std::string get_informationstate_string(bool discard_phase, hand* p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer);


    public:
        permutationpolicyplayer(int in_seed);
        permutationpolicyplayer(policy* in_policy, int in_seed);
        permutationpolicyplayer(std::string filepath, int inseed);

        action poll_player(bool discard_phase, hand* p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer);     

};