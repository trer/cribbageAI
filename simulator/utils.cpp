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

int update_legal_moves(int** available_actions, int* available_actions_indexes, card* hand_cards, int num_hand_cards, int sum_cards_played, bool discard_done) {
    /*
     * Iterates over all legal actions in a hand and enumerates them into a list
     * Counts number of legal moves.
     * Updates available_actions and available_actions_indexes inplace and returns num_available_actions
    */
    bool is_legal_move = false;
    int num_available_actions = 0;
    int offset = 0;
    card* hand_copy = new card[num_hand_cards];
    for (int i = 0; i < num_hand_cards; i++) {
        hand_copy[i] = hand_cards[i];
    }
    std::sort(hand_copy, hand_copy + num_hand_cards); // inplace sorting?
    //map the presented actions to hand
    for (int i = 0; i < num_hand_cards; i++) {
        for (int j = 0; j < num_hand_cards; j++) {
            if (hand_cards[i] == hand_copy[j]) {
                available_actions_indexes[j] = i;
            }
        }
        
    }

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


policy::policy() {

}

policy::policy(std::unordered_map<std::string, std::vector<double>*> in_states, int index) {

    for (auto it : in_states) {
        
        // int length = it.second->size();
        
        infostates[it.first] = std::vector<double>(it.second[index]);
        // for (int i = 0; i < length; i++) {
        //     infostates[it.first][i] = it.second[index][i];
        // }
        
    }
    
}

policy::policy(std::unordered_map<std::string, std::vector<double>> in_infostates) {
    infostates = in_infostates;
}

bool policy::part_of_policy(std::string key) {
    return !(infostates.find(key) == infostates.end());
}

std::vector<double> policy::action_probabilities(std::string key) {
    return infostates[key];
}


std::vector<double> policy::action_probabilities(std::string info_state_key, int num_available_actions) {

    if (num_available_actions == 0) {
        std::cout << "ERROR (probably): action probabilities called from a state that does not have any available actions" << std::endl;
    }

    //if the infostate is not present in the strategy return a random move
    if (!part_of_policy(info_state_key)) {
        infostates[info_state_key] = std::vector<double>();
        std::generate_n(std::back_inserter(infostates[info_state_key]), num_available_actions, [] {return 1.0;});
        for (int i = 0; i < num_available_actions; i++) {
            infostates[info_state_key][i] = 1.0/num_available_actions;
        }   
    }
    
    return infostates[info_state_key];
}

bool policy::serialize(std::string filepath) {
    std::ofstream file;
    file.open(filepath, std::ios::binary | std::ios::out);
    if (file.is_open()) {
        size_t length = infostates.size();
        size_t str_length;
        size_t vec_length;
        file.write(reinterpret_cast<char*>(&length), sizeof(length));
        for(auto it : infostates) {
            str_length = it.first.length();
            file.write(reinterpret_cast<char*>(&str_length), sizeof(str_length));
            file.write( it.first.c_str(), str_length);
            vec_length = it.second.size();
            file.write(reinterpret_cast<char*>(&vec_length), sizeof(vec_length));
            file.write(reinterpret_cast<char*>(&it.second[0]), vec_length * sizeof(double));
        }
        
        file.close();
        return true;
    }
    return false;
}

bool policy::deserialize(std::string filepath) {
    std::ifstream file;
    file.open(filepath, std::ios::binary | std::ios::out);
    if (file.is_open()) {
        size_t length;
        
        

        file.read(reinterpret_cast<char*>(&length), sizeof length);

        for (int i = 0; i < length; i++) {
            size_t str_length;
            size_t vec_length;
            char* tmp_key;
            std::string tmp_key2;
            file.read(reinterpret_cast<char*>(&str_length), sizeof(str_length));
            tmp_key = new char[str_length+1];
            file.read(reinterpret_cast<char*>(&tmp_key[0]), str_length);
            tmp_key[str_length] = '\0';
            tmp_key2 = tmp_key;
            file.read(reinterpret_cast<char*>(&vec_length), sizeof(vec_length));
            infostates[tmp_key2] = std::vector<double>(vec_length);
            file.read(reinterpret_cast<char*>( &infostates[tmp_key2][0]), vec_length * sizeof(double));
        }
        
        file.close();
        return true;
    } else {
        std::cout << "BAD READ nothing loaded" << std::endl;
        return false;
    }
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
