#include ".././simulator/card.h"

bool compare_cards(card* c1, card* c2) {
    int c1_value = c1->get_value(true);
    int c2_value = c2->get_value(true);
    int s1 = c1->get_suit(true);
    int s2 = c2->get_suit(true);
    return c1_value == c2_value && s1 == s2;
}