#pragma once
#include "controller.h"

namespace controller {

    game_controller::game_controller(int seed) {
        game = simulator::cribbage(seed);


    }

    void game_controller::set_player(char player_type, int num) {
        if (num == 1) {
            switch (player_type)
            {
            case 'h':
                player1 = &h1;
                break;
            case 'r':
                player1 = &r1;
                break;
            case 'g':
                player1 = &g1;
                break;
            default:
                break;
            }
            game.set_player(player1, num);
        } else {
            switch (player_type)
            {
            case 'h':
                player2 = &h2;
                break;
            case 'r':
                player2 = &r2;
                break;
            case 'g':
                player2 = &g2;
                break;
            default:
                break;
            }
            game.set_player(player2, num);
        }
    }

    void game_controller::set_seed(int seed) {

    }

    void game_controller::set_firstdealer(int first_dealer) {

    }

    void game_controller::setup_game(bool same_pone) {
        game.reset();
        if (same_pone) {
            game.reset();
        }
    }

    void game_controller::setup_round() {
        discard_set_p1 = false;
        discard_set_p2 = false;
        game.setup_round();
        game.set_current_player();
    }

    void game_controller::setup_play_phase() {
        game.setup_play_phase();
        game.set_current_player(); //setup for first turn
    }

    int game_controller::play_next_step()
    {
        return 0;
    }

    int game_controller::get_current_player() {
        player* cp = game.current_player;
        if (cp == game.player1) {
            return 1;
        }
        if (cp == game.player2) {
            return 2;
        }
        return 0;
    }

    int game_controller::get_num_cards(int player) {
        if (player == 1) {
            return game.player1->get_hand()->get_num_cards();
        } else if (player == 2) {
            return game.player2->get_hand()->get_num_cards();
        } else if (player == -1) {
            return game.crib.get_num_cards();
        }
        
        return -1;
    }

    std::string game_controller::get_cut_card() {
        card* c = game.game_deck->cut();
        std::string cut_name = c->string_format();
        return cut_name;
    }

    std::string game_controller::get_card(int index, int player) {
        card* tmp;
        if (player == -1) {
            tmp = game.crib.get_card(index);
        } else {
            tmp = game.get_player_hand(player)->get_card(index);
        }
        std::string card_name = tmp->string_format();
        return card_name;
    }

    
    void game_controller::discard_action(int player, int index1, int index2) {
        card* tmp = game.get_player_hand(player)->get_cards(); 
        if (player == 1) {
            d1 = tmp[index1];
            d2 = tmp[index2];
            game.player1->get_hand()->remove_2card(index1, index2);
            discard_set_p1 = true;
            game.set_discard(action(&d1, &d2), player);
        } else if (player == 2) {
            d3 = tmp[index1];
            d4 = tmp[index2];
            game.player2->get_hand()->remove_2card(index1, index2); //Does random automatically do this?
            discard_set_p2 = true;
            game.set_discard(action(&d3, &d4), player);
        }
    }

    int game_controller::play_action(int player, int index) {
        
        int current_player = get_current_player();
        if (current_player == player) {
            if (index == -1) {
                game.set_play_action(action());
            } else {
                hand* tmp = game.get_player_hand(player);
                c = *tmp->get_card(index);
                tmp->remove_card(index);
                game.set_play_action(action(&c));
                
            }
        }
        return resolve_action();
    }

    int game_controller::resolve_action() {
        game.resolve_action();
        if (game.get_player_hand_size(1) == 0 && game.get_player_hand_size(2) == 0) {
            game.last_card_played();
        }
        int win = game.check_win();
        
        //setup next turn
        game.set_current_player();
        
        return win;
    }

    bool game_controller::discard_set() {
        return discard_set_p1 && discard_set_p2;
    }

    int game_controller::handle_discards() {
        return game.handle_discards();
    }

    int game_controller::get_sum_cards_played() {
        return game.sum_cards;
    }

    bool game_controller::has_legal_move(int player) {
        if (player == 1) {
            return exsists_legal_move(player1->get_hand()->get_cards(), player1->get_hand()->get_num_cards(), get_sum_cards_played());
        } else if (player == 2)
        {
            return exsists_legal_move(player2->get_hand()->get_cards(), player2->get_hand()->get_num_cards(), get_sum_cards_played());
        }
        return false;   
    }

    bool game_controller::has_called_go(int player) {
        if (player == 1) {
            if (game.player1 == game.pone) {
                return game.pone_go;
            } else {
                return game.dealer_go;
            }
        } else if (player == 2) {
            if (game.player2 == game.pone) {
                return game.pone_go;
            } else {
                return game.dealer_go;
            }
        }
        return false;
    }

    void game_controller::matching_setup() {
            game.matching_setup();
    }

    int game_controller::matching_score_pone() {
        int win = game.matching_score_pone();
        return win;
    }

    int game_controller::matching_score_dealer() {
        int win  = game.matching_score_dealer();
        return win;
    }

    int game_controller::get_points(int player) {
        if (player == 1) {
            return game.player1_score;
        } else if (player == 2) {
            return game.player2_score;
        }
        return -1;
    }

    int game_controller::get_current_pone() {
        if (game.player1 == game.pone) {
            return 1;
        } else if (game.player2 == game.pone) {
            return 2;
        }
        return -1;
    }

    std::string game_controller::poll_player(int p) {
        std::string out_string;
        player* player_to_poll = nullptr;
        bool discard_phase;
        int opp_num_cards;
        int score_self;
        int score_opp;
        int win=0;
        if (p == 1) {
            player_to_poll = player1;

            if (discard_set_p1) {
                discard_phase = false;
            } else {
                discard_phase = true;
                discard_set_p1 = true;
            }
            opp_num_cards = player2->get_hand()->get_num_cards();
            score_self = game.player1_score;
            score_opp = game.player2_score;

        } else if (p == 2) {
            player_to_poll = player2; 
            if (discard_set_p2) {
                discard_phase = false;
            } else {
                discard_phase = true;
                discard_set_p2 = true;
            }
            opp_num_cards = player1->get_hand()->get_num_cards();
            score_self = game.player2_score;
            score_opp = game.player1_score;
        }

        if (player_to_poll == nullptr) {
            std::cout << "NO PLAYER FOUND, returning" << std::endl;
            return "";
        }

        action a = player_to_poll->poll_player(discard_phase, game.cards_played, game.num_cards_played, game.sum_cards, opp_num_cards, score_self, score_opp, !game.pone_to_play);
        out_string = a.card1->string_format();
        if (discard_phase) {
            game.set_discard(a, p);
            out_string += " " + a.card2->string_format();
        } else {
            game.set_play_action(a);
            win = resolve_action();
        }
        out_string += " " + std::to_string(win);
        return out_string;
    }
}