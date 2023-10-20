#pragma once
#include "./card.h"
#include "./deck.h"
#include "./hand.h"
#include "./scorer.h"

class round {
    private:
        deck game_deck;
        card * played_cards;
        hand hand1;
        hand hand2;
        card cut_card;
        int player1_score;
        int player2_score;

    
    public:
        round();

        //game(player player1, player player2, int player1_score, int_player2_score);
        void start_game();
        int get_score(hand* hand);
        int poll_player(hand* player_hand, int num_opponent_cards, card ** played_cards,
                        card cut_card, int player1_score, int player2_score, bool discard);
        

}
