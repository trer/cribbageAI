#pragma once
#include "./hand.h"
#include "./action.h"
#include "./utils.h"



class player {
    protected:
        hand *player_hand;        
        bool update_legal_moves(int sum_cards_played);
        int legal_moves[6];
        card c1;
        card c2;

    public:
        player();
        
        virtual action poll_player(bool discard_phase, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp) = 0;
        void set_hand(hand *new_hand);
        hand *get_hand();

        

};

class randomplayer : public player{

    private:
        action discard_two_random_cards();
        action play_random_card();

    public:
        randomplayer();
        action poll_player(bool discard_phase, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp);     

};


class realplayer : public player{

    private:
        action discard_two_cards();
        action play_a_card(int sum_played_cards);

    public:
        realplayer();
        action poll_player(bool discard_phase, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp);     

};

class mockplayer : public player{


    public:
        int order[6];
        int count;
        mockplayer();
        action poll_player(bool discard_phase, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp);     

};