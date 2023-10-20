#include "./deck.h"
#include <iostream>
#include <string>
#include <random>

using namespace std;

deck::deck() {
    
    std::random_device rd;  // a seed source for the random number engine
    seed = rd();

    cout << seed << endl;
    num_cards = 52;
    init();
}

deck::deck(int inSeed) {
    
    seed = inSeed;
    num_cards = 52;
    init();
}

void deck::init() {
    
    char inSuit = 'H';

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
    std::mt19937 gen(seed); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0, 51);
    for (int i=51; i>0; --i) {
        int tmpInt = distrib(gen) % (i+1);
        card tmpCard = cards[i];
        cards[i] = cards[tmpInt];
        cards[tmpInt] = tmpCard;


    }
}

void deck::print_deck(){
    for (int i=0; i<52; i++) {
        card tmpCard = cards[i];
        tmpCard.print_card();
    }
}

int main() {
    std::random_device rd;  // a seed source for the random number engine
    int s1 = rd();
    int s2 = rd();
    deck d1 = deck(s1);
    deck d2 = deck(s2);
    d1.shuffle();
    d1.print_deck();
    cout << "next deck" << endl;
    d2.shuffle();
    d2.print_deck();
    //remember that rd gives the same number every time, better to use seed
}
