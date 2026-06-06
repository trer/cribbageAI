#pragma once
#include "./card.h"
#include "utils.h"
#include <random>

#define MAX_NUM_DECKCARDS 52 //must be divisible by 4

class deck {
    protected:
        int num_cards;
        card cards[MAX_NUM_DECKCARDS];
        card cut_card;
        int seed;
        std::mt19937 gen;
        std::uniform_int_distribution<int> distrib;
        void init();
    public:
        deck();
        deck(int seed);
        deck(const deck &source);
        void set_seed(int seed);
        virtual void shuffle();
        virtual card *deal_top();
        card * cut();
        void print_deck();
};

class mock_deck : public deck {
    private:
        card top13[13];
        bool top13_set=false;
        bool top6_set=false;
    public:
        mock_deck();
        void shuffle() override;
        void set_top_13_cards(card* cards);
        void set_top_6_cards(card* in_cards);
        card* deal_top() override;
};

class internal_deck : public deck {
    public:
        internal_deck();
        card *get_cards();
};