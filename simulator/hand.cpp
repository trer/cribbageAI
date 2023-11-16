#pragma once
#include "./hand.h"
#include <iostream>
#include <string>

using namespace std;

#define MAX_NUM_CARDS 6
//might want to increase the max_number of cards to 7 if I want to brute force best hand

hand::hand() {
    init();
}

hand::hand(deck *in_deck) {
    draw_deck = in_deck; // Deck the hand draws cards from.
    init();
}

hand::hand(deck *in_deck, card* in_cards, int in_num_cards) {
    draw_deck = in_deck;
    num_cards = in_num_cards;
    for(int i=0; i<num_cards; i++) {
        cards[i] = in_cards[i];
    }
    init(in_num_cards);
}

void hand::init(int in_num_cards) {
    num_cards = in_num_cards; //The number of cards in the hand currently
    score = -1; //remember the score for the hand (should be set by the player or a scorer class)
    cut_drawn = 0; // counter/bool keeping checking if the cut card is available (will be in cards[num_cards] if it exists) 
}

void hand::set_deck(deck *in_deck) {
    draw_deck = in_deck;
}

void hand::draw(int num) {
    /*
     * Draws num number of cards to the hand.
     * Should not draw new cards after cut-card has been drawn.
    */
    for (int i=0; i<num; i++) {
        cards[num_cards+i] = *draw_deck->deal_top();
    }
    num_cards = num_cards + num;
}

void hand::remove_card(int pos) {
    /*
     * Removes the card at index pos in the cards array.
     * If consecutive cards should be removed, indexes has to be found again after removing a card.
    */
    if (pos == num_cards-1) {
        
    } else {
        card tmpCard = cards[pos];
        cards[pos] = cards[num_cards-1];
        cards[num_cards-1] = tmpCard;
    }
    num_cards--;
}

void hand::remove_2card(int pos1, int pos2) {
    /*
     * Choose two cards to remove without having to worry about positions changing inbetween removals.
    */
    if (pos1 == num_cards-2 || pos2 == num_cards-1) {
        remove_card(pos2);
        remove_card(pos1);
    } else {
        remove_card(pos1);
        remove_card(pos2);
    }
}


void hand::draw_cut(){
    /*
     * The cut is placed in the last position of the hand. 
     * When scoring the this should be acounted for.
     */
    if (!cut_drawn) {
        if (num_cards >= MAX_NUM_CARDS) {
            cout << "too many cards in hand, not drawing" << endl;
            return;
        }
        cards[num_cards] = *draw_deck->cut();
        cut_drawn = 1;
    }
}

card* hand::get_card(int pos) {
    return &cards[pos];
}

card* hand::get_cards() {
    return cards;
}

void hand::set_num_cards(int num) {
    num_cards = num;
}

int hand::get_num_cards() {
    return num_cards;
}

void hand::set_score(int new_score) {
    score = new_score;
}

int hand::get_score() {
    return score;
}

void hand::print_hand() {
    cout << "cards: ";
    for (int i=0; i<num_cards; i++) {
        cards[i].print_card();
    }
    cout << "score: " << score << endl;

}

/*
int main() {
    std::random_device rd;  // a seed source for the random number engine
    int s1 = rd();
    deck d1 = deck(s1);
    hand h1 = hand(&d1);
    h1.draw(6);
    cout << "top 6" << endl;
    h1.print_hand();
    h1.remove_2card(1, 5);
    h1.print_hand();
    h1.draw_cut();
    
    
    //remember that rd gives the same number every time, better to use seed
    //cannot shuffle while players are holding cards
}
*/