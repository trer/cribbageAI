#pragma once
#include "./deck.h"
#include <iostream>
#include <string>
#include <random>

using namespace std;

deck::deck() : gen(seed), distrib(0, 51){
    
    std::random_device rd;  // a seed source for the random number engine
    seed = rd();

    num_cards = 52;
    init();
}

deck::deck(int inSeed): gen(inSeed), distrib(0, 51) {
    
    seed = inSeed;
    num_cards = 52;
    init();
}

deck::deck(const deck &source) {
    seed = source.seed;
    num_cards = source.num_cards;
    for (int i = 0; i < 52; i++)
    {
        cards[i] = source.cards[i];
    }
    
    cut_card = source.cut_card;
    gen = source.gen;
    distrib = source.distrib;
}

void deck::set_seed(int inSeed){
    seed = inSeed;
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

		for (int j=0; j<13; j++) {

			cards [(i*13)+j] = card(j+1, inSuit);
		}
	}
}

void deck::shuffle() {
    num_cards = 52;
    cut_card = card();
    for (int i=51; i>0; --i) {
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
    for (int i=0; i<52; i++) {
        card tmpCard = cards[i];
        tmpCard.print_card();
    }
}


mock_deck::mock_deck() {
    top13[13];
    top13_set = false;

}

void mock_deck::shuffle() {
    num_cards = 52;
    cut_card = card();
    if (top13_set) {
        set_top_13_cards(top13);
    } else {
        cout << "cards not set, using default" << endl;
        deck::shuffle();
    }
}

void mock_deck::set_top_13_cards(card* in_cards) {
    for (int i = 0; i < 13; i++) {
        top13[i] = in_cards[i];
        cards[num_cards-i-1] = in_cards[i];
    }
    top13_set = true;
    
}

internal_deck::internal_deck() {

}

card* internal_deck::get_cards(){
    return cards;
}