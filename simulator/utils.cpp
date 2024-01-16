#pragma once
#include "./utils.h"

#define CRIBBAGE_SUM_CEIL 31

bool compare_cards(card* c1, card* c2) {
    int c1_value = c1->get_value(true);
    int c2_value = c2->get_value(true);
    int s1 = c1->get_suit(true);
    int s2 = c2->get_suit(true);
    return c1_value == c2_value && s1 == s2;
}

bool part_of_hand(card* hand_cards, int num_hand_cards, card* card_to_check) {
    /*
     * Checks if the card_to_check is part of the player hand
    */
    bool part_of_hand = false;
    for (int i = 0; i < num_hand_cards; i++) {
        if (compare_cards(&hand_cards[i], card_to_check)) {
            part_of_hand = true;
            break;
        }
    }
    if (!part_of_hand) {
        return false;
    }
    return true;
}

bool exsists_legal_move(card* hand_cards, int num_hand_cards, int sum_cards_played) {
    bool is_legal_move = false;

    for (int i=0; i<num_hand_cards; i++) {
        if (hand_cards[i].get_value(false) + sum_cards_played <= CRIBBAGE_SUM_CEIL) {
            is_legal_move = true;
            break;
        }
    }
    return is_legal_move;
}

bool check_valid_move(bool discard_phase, card *cards_played, int num_cards_played, 
                    int sum_cards, card* crib, int num_cards_in_crib, card* player_hand, int num_cards_in_player_hand, 
                    action player_action) {
    /*
     * Checks the validity of a move. Returns true if the move is allowed to play.
     * Makes sure:
     * That cards aren't played several times.
     * That the correct action is played.
     * That the sum of cards don't exceed 31 after the card is played.
     * 
     * Not meant to be a safe guard against cheating, just checks for common mistakes
     */

   
    //If it is the discard phase and a discard action has been selected return true
    if (discard_phase) {
        //Check that action contains 2 cards
        if (player_action.card2 == 0) {
            return false;
        }
        if (player_action.card1 == 0) {
            return false;
        }
        //check that both cards are part of the players hand
        //We have to acount for the fact that the player has removed the card(s) they try to play, in this case 2
        if (!part_of_hand(player_hand, num_cards_in_player_hand+2, player_action.card1)) {
            return false;
        }
        if (!part_of_hand(player_hand, num_cards_in_player_hand+2, player_action.card2)) {
            return false;
        }
        if (compare_cards(player_action.card1, player_action.card2)) {
            //tries to discard same card twice
            return false;
        }
        //Otherwise legal action
        return true;

    } else {
        //If it is not the discard phase we check that the second card is not defined
        if (player_action.card2 != 0) {
            return false;
        }
    }
    //player tries to play a card
    if (player_action.card1 != 0) {
        card* played_card = player_action.card1;
        //Check that sum of cards wont exceed 31
        if (played_card->get_value(false) + sum_cards > CRIBBAGE_SUM_CEIL) {
            return false;
        }

        //If the card is part of previous cards played
        for (int i = 0; i < num_cards_played; i++) {
            if (compare_cards(&cards_played[i], played_card)) {
                return false;
            }
        }
        //If the card is part of the crib or cut
        for (int i = 0; i < num_cards_in_crib; i++) {
            if (compare_cards(&crib[i], played_card)) {
                return false;
            }
        }
        //We have to acount for the fact that the player has removed the card(s) they try to play, in this case 1
        if (!part_of_hand(player_hand, num_cards_in_player_hand+1, played_card)) {
            return false;
        }
        
        return true;
    }
    //If player tries to say go
    return !exsists_legal_move(player_hand, num_cards_in_player_hand, sum_cards);
}
