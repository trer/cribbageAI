#pragma once
#include "./card.h"
#include <random>

class deck {
    private:
        int num_cards;
        card cards[52];
        int seed;
        void init();
    public:
        deck();
        deck(int seed);
        void set_seed(int seed);
        void shuffle();
        card * deal(int deal_num);
        card * cut();
        void print_deck();
};