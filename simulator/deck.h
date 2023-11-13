#pragma once
#include "./card.h"
#include <random>

class deck {
    private:
        int num_cards;
        card cards[52];
        card cut_card;
        int seed;
        std::mt19937 gen;
        std::uniform_int_distribution<int> distrib;
        void init();
    public:
        deck();
        deck(int seed);
        void set_seed(int seed);
        void shuffle();
        card *deal_top();
        card * cut();
        void print_deck();
};