#pragma once
#include "./card.h"
#include "./hand.h"
#include "./player.h"


int score_cards(card* cards, int num_cards, bool is_crib, card* cut_card);
int score_cards(hand * hand, bool is_crib);
int score_cards(player *player);

int score_played_card(card * card_history, int num_cards_played, card* card_played, int sum_cards_played);
int score_cut(card* cut_card);