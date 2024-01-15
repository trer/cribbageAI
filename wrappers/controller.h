#pragma once
#include "../simulator/cribbage.h"
#include "../simulator/player.h"
#include "../simulator/utils.h"
#include <string>

namespace controller {

    class game_controller {
        private:
            simulator::cribbage game;

            bool discard_set_p1;
            bool discard_set_p2;
            bool is_legal_move_next_turn;

            card c;
            card d1;
            card d2;
            card d3;
            card d4;
            
            player* player1;
            player* player2;

            randomplayer r1 = randomplayer();
            randomplayer r2 = randomplayer();
            realplayer h1 = realplayer();
            realplayer h2 = realplayer();

            int resolve_action();

        public:
            game_controller(int seed=0);
            void set_player(char player_type, int num);
            void set_seed(int seed);
            void set_firstdealer(int first_dealer);
            void setup_game(bool same_pone=false);
            void setup_round();
            void setup_play_phase();

            int play_next_step();
            int get_num_cards(int player);
            std::string get_cut_card();
            std::string get_card(int index, int player);
            void discard_action(int player, int index1, int index2);
            int play_action(int player, int index);
            int get_current_player();
            bool discard_set();
            int handle_discards();
            int get_sum_cards_played();
            bool has_legal_move(int player);
            bool has_called_go(int player);

            void matching_setup();
            int matching_score_pone();
            int matching_score_dealer();

            int get_points(int player);
            int get_current_pone();
            std::string poll_player(int player);
    };
};