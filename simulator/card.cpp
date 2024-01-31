#pragma once
#include "./card.h"


card::card() {
    value = 0;
    suit = 'N';
}

card::card(int v, char s) {
    value = v;
    suit = s;
}


int card::get_value(bool distinctFace) {
    if (distinctFace) {
        return value; 
    }
    return std::min(10, value);
}

char card::get_suit(bool numeric) {
    if (numeric) {
        switch (suit)
        {
        case 'D':
            return 0;
        
        case 'C':
            return 1;
        case 'H':
            return 2;
        case 'S':
            return 3;
        default:
            return -1;
            break;
        }
    }
    return suit;
}

void card::print_card() {
    std::cout << string_format() << std::endl;
}
    

std::string card::string_format() {
    bool isLetter = false;
    std::string charValue;
    std::string s;
    switch (value)
    {
    case 13:
        isLetter = true;
        charValue = "K";
        break;
    case 12:
        isLetter = true;
        charValue = "Q";
        break;
    case 11:
        isLetter = true;
        charValue = "J";
        break;
    case 10:
        isLetter = true;
        charValue = "T";
        break;
    case 1:
        isLetter = true;
        charValue = "A";
        break;
    default:
        break;
    }

    switch (suit)
    {
    case 'D':
        //s = "♢";
        break;
    case 'C':
        //s="♣";
        break;
    case 'H':
        //s="♡";
        break;
    case 'S':
        //s="♠";
        break;
    default:
        s="N";
        break;
    }
    
    if (isLetter) {
        return  get_suit(false) + charValue;
    } else {
        return get_suit(false) + std::to_string(get_value(false));
    }
    
}

bool card::operator<(card const & b) {
    if (value == b.value) {
        return suit < b.suit;
    }
    return value < b.value;
}

bool card::operator==(card const & b) {
    return value == b.value && suit == b.suit;
}