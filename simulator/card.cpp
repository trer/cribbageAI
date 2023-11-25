#pragma once
#include "./card.h"
#include <iostream>
#include <string>

using namespace std;

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
    return min(10, value);
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
    bool isLetter = false;
    string charValue;
    string s;
    switch (value)
    {
    case 13:
        isLetter = true;
        charValue = "K-";
        break;
    case 12:
        isLetter = true;
        charValue = "Q-";
        break;
    case 11:
        isLetter = true;
        charValue = "J-";
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
        std:: cout << charValue << get_suit(false) << endl;
    } else {
        std:: cout << get_value(false) << "-"<< get_suit(false) << endl;
    }
    
}


// int main() {
//     card c1 = card(4, 'H');
//     cout << c1.get_suit() << endl;
//     cout << c1.get_value(true) << endl;
//     c1.print_card();
//     card c2 = card();
//     c2.print_card();
// }