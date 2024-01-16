#pragma once
#include "./card.h"
#include <random>

class deck {
    protected:
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
        virtual void shuffle();
        card *deal_top();
        card * cut();
        void print_deck();
};

class mock_deck : public deck {
    private:
        card top13[13];
        bool top13_set;
    public:
        mock_deck();
        void shuffle() override;
        void set_top_13_cards(card* cards);
};

class internal_deck : public deck {
    public:
        internal_deck();
        card *get_cards();
};