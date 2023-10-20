#pragma once
#include "./card.h"
#include "./deck.h"



class hand {
    private:
        int num_cards;
        card * cards;
        int score;
        deck * draw_deck;

    public:
        hand();
        hand(deck * indeck);
        card * view_hand();
        void draw(int num_cards);
        void remove_card(int card_pos);
        void set_score(int set_score);
        int get_score();



}