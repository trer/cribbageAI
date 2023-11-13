#pragma once
#include "./hand.h"
#include "./action.h"



class player {
    protected:
        hand *player_hand;        
        bool update_legal_moves(int sum_cards_played);
        int legal_moves[6];

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