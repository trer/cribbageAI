#pragma once
#include "./card.h"



class action {
    public:
        action(card *play_card); //play card
        action(card *discard1, card *discard2); //discard cards
        action(); //pass
        card * card1;
        card * card2;
        
};