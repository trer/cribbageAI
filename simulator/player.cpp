#include "./player.h"
#include <iostream>
#include <string>
#include "player.h"

using namespace std;

#define CRIBBAGE_SUM_CEIL 31

player::player() {
    *player_hand; //the player hand
    for (int i=0; i<6; i++) {
        legal_moves[i] = 0;
    }
}

bool player::update_legal_moves(int sum_cards_played) {
    bool is_legal_move = false;
    int num_cards = player_hand->get_num_cards();
    card* cards = player_hand->get_cards();

    for (int i=0; i<num_cards; i++) {
        if (cards[i].get_value(false) + sum_cards_played <= CRIBBAGE_SUM_CEIL) {
            legal_moves[i] = 1;
            is_legal_move = true;
        }
    }
    return is_legal_move;
}

void player::set_hand(hand *new_hand) {
    player_hand = new_hand;
}

hand* player::get_hand() {
    return player_hand;
}

randomplayer::randomplayer() {

}

action randomplayer::discard_two_random_cards() {
    /*
     * Discards the two last cards in the hand.
     * As draw is random, this results in random discards.
    */
    int pos1 = player_hand->get_num_cards()-1;
    int pos2 = pos1 - 1;


    card* c1 = player_hand->get_card(pos1);
    card* c2 = player_hand->get_card(pos2);

    player_hand->remove_2card(pos1, pos2);

    return action(c1, c2);
}

action randomplayer::play_random_card() {
    /*
     * Plays first card which is legal.
     * As the hand is randomly drawn this will result in playing random cards.
     * Might have a slight bias towards choosing higher cards first in the second round.
    */
    int num_cards = player_hand->get_num_cards();
    for (int i=0; i<=num_cards; i++) {
        if (legal_moves[i]) {
            card* c1 = player_hand->get_card(i);
            player_hand->remove_card(i);
            return action(c1);
        }
    }
    return action();
}

action randomplayer::poll_player(bool discard_phase, card* cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp) {
    /*
     * Plays more or less randomly
    */
    if (discard_phase) {
        return discard_two_random_cards();
    } else if (!update_legal_moves(sum_cards)) {
        return action();
    } else {
        return play_random_card();
    }
    
}

action realplayer::discard_two_cards() {
    int pos1;
    int pos2;
    cout << "Give index of first card you want to discard";
    cin >> pos1;
    cout << "Give index of second card you want to discard";
    cin >> pos2;
    card c1 = *player_hand->get_card(pos1);
    card c2 = *player_hand->get_card(pos2);
    player_hand->remove_2card(pos1, pos2);

    return action(&c1, &c2);
}

action realplayer::play_a_card(int sum_cards_played) {
    if (update_legal_moves(sum_cards_played)) {
        int pos;
        cout << "Give index of the card you want to play";
        cin >> pos;
        card c1 = *player_hand->get_card(pos);
        player_hand->remove_card(pos);

        return action(&c1);
    }
    return action();
}

realplayer::realplayer()
{
}

action realplayer::poll_player(bool discard_phase, card *cards_played, int num_cards_played, int sum_cards, int opponent_num_cards, int score_self, int score_opp)
{
    cout << "Your score: " << score_self;
    cout << "Opponent score: " << score_opp;
    cout << "Discad Phase: " << discard_phase << endl;
    cout << "cards played: " << endl;
    for (int i = 0; i < num_cards_played; i++) {
        cards_played[i].print_card();
    }
    cout << "Your cards are: " << endl;
    for (int i = 0; i < player_hand->get_num_cards(); i++) {
        player_hand->get_card(i)->print_card();
    }

    if (discard_phase) {
        return discard_two_cards();
    } else {
        return play_a_card(sum_cards);
    }

}
