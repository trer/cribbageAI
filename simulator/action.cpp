#pragma once
#include "./action.h"

action::action() {
    card1 = 0;
    card2 = 0;
}

action::action(card *play_card) {
    card1 = play_card;
    card2 = 0;
}

action::action(card *discard1, card *discard2) {
    card1 = discard1;
    card2 = discard2;
}
