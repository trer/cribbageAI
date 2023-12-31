#pragma once
#include "./action.h"
#include "./card.h"

bool part_of_hand(card* hand_cards, int num_hand_cards, card* card_to_check);
bool check_valid_move(bool discard_phase, card *cards_played, int num_cards_played, int sum_cards,
                      card* crib, card* player_hand, int num_cards_in_player_hand, action player_action);
bool compare_cards(card* c1, card* c2);
bool exsists_legal_move(card* hand_cards, int num_hand_cards, int sum_cards_played);