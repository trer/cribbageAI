#pragma once
#include "cribbage.h"
#include "player.h"
#include "deck.h"
#include "scorer.h"
#include "utils.h"

#include <sstream>




class greedyplayer : public player {

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

class greedypolicy : public greedyplayer, public policy{

    private:
        std::vector<double> poll_player_and_return_probabilities(bool discard_phase, hand* player_hand, card* cards_played, int num_cards_played, int sum_cards_played);
    public:
        greedypolicy();

        bool part_of_policy(std::string key);
        std::vector<double> action_probabilities(std::string key) override;
        std::vector<double> action_probabilities(std::string info_state_key, int num_available_actions) override;
        std::vector<double> action_probabilities(simulator::cribbage* game);

        ~greedypolicy() {};
};