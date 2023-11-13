#pragma once
#include "./card.h"
#include "./deck.h"



class hand {
    private:
        int num_cards;
        card cards[6];
        int score;
        deck * draw_deck;
        int cut_drawn;

    public:
        hand();
        hand(deck * indeck);
        hand(deck *in_deck, card* in_cards, int in_num_cards);
        void set_deck(deck* indeck);
        void draw(int num_cards);
        void draw_cut();
        card *get_card(int pos);
        card *get_cards();
        void set_num_cards(int num);
        int get_num_cards();
        void remove_card(int card_pos);
        void remove_2card(int card_pos, int card_pos2);
        void set_score(int set_score);
        int get_score();
        void print_hand();

};