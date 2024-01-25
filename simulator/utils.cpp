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

int update_legal_moves(int** available_actions, card* hand_cards, int num_hand_cards, int sum_cards_played, bool discard_done) {
    bool is_legal_move = false;
    int num_available_actions = 0;
    int offset = 0;
    if (!discard_done) {
        for (int i = 0; i < num_hand_cards; i++) {
            for (int j = i+1; j < num_hand_cards; j++) {
                available_actions[num_available_actions] = get_array(i, j);
                num_available_actions++;
            }
        }
        
    } else {
        for (int i=0; i<num_hand_cards; i++) {
            if (hand_cards[i].get_value(false) + sum_cards_played <= CRIBBAGE_SUM_CEIL) {
                available_actions[i-offset] = get_array(i);
                num_available_actions++;
            } else {
                offset++;
            }
        }
    }
    return num_available_actions;
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



int action0[1] = {0};
int action1[1] = {1};
int action2[1] = {2};
int action3[1] = {3};
int action01[2] = {0,1};
int action02[2] = {0,2};
int action03[2] = {0,3};
int action04[2] = {0,4};
int action05[2] = {0,5};
int action12[2] = {1,2};
int action13[2] = {1,3};
int action14[2] = {1,4};
int action15[2] = {1,5};
int action23[2] = {2,3};
int action24[2] = {2,4};
int action25[2] = {2,5};
int action34[2] = {3,4};
int action35[2] = {3,5};
int action45[2] = {4,5};

int* get_array(int i) {
    switch (i)
    {
    case 0:
        return action0;
    case 1:
        return action1;
    case 2:
        return action2;
    case 3:
        return action3;
    default:
        std::cout << "arrays not behaving properly, play" << std::endl;
        return action0;
    }
}


int* get_array(int i, int j) {
    switch (i)
    {
    case 0:
        switch (j)
        {
        case 1:
            return action01;
        case 2:
            return action02;
        case 3:
            return action03;
        case 4:
            return action04;
        case 5:
            return action05;
        }
        break;
    case 1:
        switch (j)
        {
        case 2:
            return action12;
        case 3:
            return action13;
        case 4:
            return action14;
        case 5:
            return action15;
        }
        break;
    case 2:
        switch (j)
        {
        case 3:
            return action23;
        case 4:
            return action24;
        case 5:
            return action25;
        }
        break;
    case 3:
        switch (j)
        {
        case 4:
            return action34;
        case 5:
            return action35;
        }
        break;
    case 4:
        return action45;
    }
    std::cout << "arrays not behaving properly, discard" << std::endl;
    return action0;
}
