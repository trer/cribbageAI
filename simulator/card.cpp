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
        return min(10, value);
    }
    return value;
}

char card::get_suit() {
    return suit;
}

void card::print_card() {
    bool isLetter = false;
    string charValue;

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
    
    if (isLetter) {
        std:: cout << value << get_suit() << endl;
    } else {
        std:: cout << get_value(false) << get_suit() << endl;
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