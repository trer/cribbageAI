#pragma once
#include "player.h"
#include "deck.h"
#include "scorer.h"

class greedyplayer : public player{

    private:
        internal_deck player_deck = internal_deck();
        bool card_in_hand(card* hand_cards, int num_cards, card* card_to_check);
        action discard_two_cards(bool is_dealer);
        action play_a_card(card* cards_played, int num_cards_played, int sum_played_cards);
        int best_cards_indexes[2];
        card cards_in_crib[2];

    public:
        greedyplayer();

        int* get_best_two_cards(card* hand_cards, int num_cards, bool is_dealer);
        int get_best_card(card* hand_cards, int num_cards, card* cards_played, int num_cards_played, int sum_cards);

        action poll_player(bool discard_phase, hand* p_hand, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp, bool is_dealer);     

};