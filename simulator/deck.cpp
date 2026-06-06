#pragma once
#include "./deck.h"
#include <iostream>
#include <string>
#include <random>

using namespace std;

deck::deck() : gen(seed), distrib(0, MAX_NUM_DECKCARDS-1){
    
    std::random_device rd;  // a seed source for the random number engine
    seed = rd();

    num_cards = MAX_NUM_DECKCARDS;
    init();
}

deck::deck(int inSeed): gen(inSeed), distrib(0, MAX_NUM_DECKCARDS-1) {
    
    seed = inSeed;
    num_cards = MAX_NUM_DECKCARDS;
    init();
}

deck::deck(const deck &source) {
    seed = source.seed;
    num_cards = source.num_cards;
    for (int i = 0; i < MAX_NUM_DECKCARDS; i++)
    {
        cards[i] = source.cards[i];
    }
    
    cut_card = source.cut_card;
    gen = source.gen;
    distrib = source.distrib;
}

void deck::set_seed(int inSeed){
    seed = inSeed;
    gen.seed(inSeed);
}


void deck::init() {    

    char inSuit = 'H';
    cut_card = card();

    for (int i=0; i<4; i++) {

		if (i == 1)
			inSuit = 'D';
		if (i == 2)
			inSuit = 'C';
		if (i == 3)
			inSuit = 'S';

		for (int j=0; j<MAX_NUM_DECKCARDS/4; j++) {

			cards [(i*MAX_NUM_DECKCARDS/4)+j] = card(j+1, inSuit);
		}
	}
}

void deck::shuffle() {
    num_cards = MAX_NUM_DECKCARDS;
    cut_card = card();
    for (int i=MAX_NUM_DECKCARDS-1; i>0; --i) {
        int tmpInt = distrib(gen) % (i+1);
        card tmpCard = cards[i];
        cards[i] = cards[tmpInt];
        cards[tmpInt] = tmpCard;


    }
}

card* deck::deal_top() {
    if (num_cards <= 0) {
        cout << "out of cards" << endl;
        return nullptr;
    }
    num_cards--;
    return &cards[num_cards];
}

card* deck::cut(){
    if (cut_card.get_suit(false) == 'N') {
        cut_card = *deal_top();
    }
    return &cut_card;
}

void deck::print_deck(){
    for (int i=0; i<MAX_NUM_DECKCARDS; i++) {
        card tmpCard = cards[i];
        tmpCard.print_card();
    }
}


mock_deck::mock_deck() {
    top13[13];
    top13_set = false;
    deck::deck(0);
}

void mock_deck::shuffle() {
    num_cards = MAX_NUM_DECKCARDS;
    cut_card = card();
    if (top13_set) {
        set_top_13_cards(top13);
    } else {
        if (!top6_set) {
            cout << "cards not set, using default" << endl;
        }
        deck::shuffle();
    }
}

void mock_deck::set_top_13_cards(card* in_cards) {
    num_cards = MAX_NUM_DECKCARDS;
    for (int i = 0; i < 13; i++) {
        top13[i] = in_cards[i];
        cards[num_cards-i-1] = in_cards[i];
    }
    top13_set = true;
    
}

void mock_deck::set_top_6_cards(card* in_cards) {
    for (int i = 0; i < 6; i++) {
        top13[i] = in_cards[i];
    }
    top6_set = true;   
}

card* mock_deck::deal_top() {
    if (top6_set) {
        num_cards--;
        if (MAX_NUM_DECKCARDS - 6 <= num_cards) {
            return &top13[MAX_NUM_DECKCARDS-1-num_cards];
        } else {
            bool in_top_6 = false;
            for (int i=0; i<6; i++) {
                in_top_6 = in_top_6 || compare_cards(&top13[i], &cards[num_cards]);
            }
            if (in_top_6) {
                return deal_top();
            }
            return &cards[num_cards];
        }    
    } else {
        return deck::deal_top();
    }
}

internal_deck::internal_deck() {

}

card* internal_deck::get_cards(){
    return cards;
}