#pragma once
#include "abstraction.h"

void information_abstraction::setup_round() {

}

std::string information_abstraction::get_informationstate_string(simulator::cribbage* game, int player) {
    return "";
}

std::shared_ptr<information_abstraction> information_abstraction::clone() {
    return std::make_shared<information_abstraction>(static_cast<information_abstraction const &>(*this));
}

void action_abstraction::setup_round() {
    
}

bool action_abstraction::is_chance_node(simulator::cribbage* game) {
    return false;
}

std::vector<double> action_abstraction::chance_outcomes(simulator::cribbage* game) {
    throw std::runtime_error("action_abstraction: chance_outcomes: Not implemented!");
}

int action_abstraction::get_num_available_actions(simulator::cribbage* game) {
    return -1;
}

void action_abstraction::apply_action_from_list(simulator::cribbage* game, int index, bool from_internal) {

}

std::shared_ptr<action_abstraction> action_abstraction::clone() {
    return std::make_shared<action_abstraction>(static_cast<action_abstraction const &>(*this));
}

abstraction::abstraction() {
    throw std::runtime_error("abstraction initialized without arguments");
}

abstraction::abstraction(std::shared_ptr<simulator::cribbage> in_game, std::shared_ptr<information_abstraction> in_info_abstraction, std::shared_ptr<action_abstraction> in_act_abstraction, int in_seed, double in_scaling){
    game = in_game;
    info_abstraction = in_info_abstraction;
    act_abstraction = in_act_abstraction;
    max_num_children = 0;
    seed = in_seed;
    scaling = in_scaling;
}


int abstraction::get_current_player() {
    return game->get_current_player();
}


void abstraction::setup_round(bool skip_game) {
    if (!skip_game) {
        game->reset();
        game->setup_round();
    }
    for (int i=0; i<max_num_children; i++) {
        child_created[i] = false;
    }
    children.clear();
    
    info_abstraction->setup_round();
    act_abstraction->setup_round();
}

int abstraction::get_point_diff(int player) {
    return -(2*(player-1)-1) * (scaling * game->get_player1_score() - game->get_player2_score());
}

bool abstraction::is_terminal() {
    return game->is_playphase_done();
}

bool abstraction::is_chance_node() {
    return act_abstraction->is_chance_node(game.get());
}

std::vector<double> abstraction::chance_outcomes() {
    if (!is_chance_node()) {
        throw std::runtime_error("abstraction::chance_outcomes: is not a chance node");
    }
    return act_abstraction->chance_outcomes(game.get());
}

std::string abstraction::get_name() {
    return name;
}

void abstraction::set_name(std::string new_name) {
    name = new_name;
}

std::string abstraction::get_informationstate_string(int player) {
    return info_abstraction->get_informationstate_string(game.get(), player);
}

int abstraction::get_num_available_actions() {
    return act_abstraction->get_num_available_actions(game.get());
}

void abstraction::apply_action_from_list(int index, bool from_internal) {
    return act_abstraction->apply_action_from_list(game.get(), index, from_internal);
}


std::shared_ptr<information_abstraction> abstraction::get_information_abstraction() {
    return info_abstraction;
}

std::shared_ptr<action_abstraction> abstraction::get_action_abstraction() {
    return act_abstraction;
}

simulator::cribbage* abstraction::get_game() {
    return game.get();
}


template<class derived>
abstraction* base_abstraction<derived>::child(int action_index) {
    int num_available_actions = get_num_available_actions();

    if (max_num_children != num_available_actions) {
        child_created.clear();
        max_num_children = num_available_actions;
        if (max_num_children > 0) {
            for (int i = 0; i < max_num_children; i++) {
                child_created.push_back(false);
            }
        }
    } 

    if (action_index >= num_available_actions || action_index < 0) {
        throw std::runtime_error("action_index is not within actionspace");
    }

    while (children.size() <= action_index) {
        children.push_back(nullptr);
    }
    if (!child_created[action_index]) {
        children[action_index] = std::make_shared<derived>(std::shared_ptr<simulator::cribbage>(new simulator::cribbage( *(game.get()) )),
                                                info_abstraction->clone(),
                                                act_abstraction->clone(),
                                                seed,
                                                scaling);
        children[action_index]->apply_action_from_list(action_index);
        child_created[action_index] = true;
    }

    return children[action_index].get();
}

template<class derived>
abstraction* base_abstraction<derived>::new_initial_state() {
    abstraction* next_state = new derived(std::make_shared<simulator::cribbage>(*(game.get())),
                                              info_abstraction->clone(),
                                              act_abstraction->clone(),
                                              seed,
                                              scaling);
    next_state->setup_round();
    return next_state;
}

std::string hand_cards::get_informationstate_string(simulator::cribbage* game, int player) {
 /*
     * Returns a string representing the current information-state.
     * This will need to be updated as more information abstractions are used.
    */
    int n = game->get_player_hand_size(player);
    int* card_ranks = new int[n];
    for (int i = 0; i < n; i++) {
        card_ranks[i] = game->get_player_hand(player)->get_card(i)->get_value(true);
    }
    std::sort(card_ranks, card_ranks + n);

    std::string result = "";
    for (int i = 0; i < n; i++){
        result.append(std::to_string(card_ranks[i]) + "_");
    }
    
    delete[] card_ranks;    
    return result;
}

std::shared_ptr<information_abstraction> hand_cards::clone() {
    return std::make_shared<hand_cards>(static_cast<hand_cards const &>(*this));
}


std::string new_stack_and_hand_cards::get_informationstate_string(simulator::cribbage* game, int player) {
    /*
     * Returns a string representing the current information-state.
     * This will need to be updated as more information abstractions are used.
    */
    int n = game->get_player_hand_size(player);
    int* card_ranks = new int[n];
    for (int i = 0; i < n; i++) {
        card_ranks[i] = game->get_player_hand(player)->get_card(i)->get_value(true);
    }
    std::sort(card_ranks, card_ranks + n);

    std::string result = "";
    for (int i = 0; i < n; i++){
        result.append(std::to_string(card_ranks[i]) + "_");
    }
    
    result.append("|");

    for (int i = 0; i < game->get_num_cards_played(); i++) {
        result.append(std::to_string(game->get_cards_played()[i].get_value(true)) + "_");
    }

    delete[] card_ranks;    
    return result;
}

std::shared_ptr<information_abstraction> new_stack_and_hand_cards::clone() {
    return std::make_shared<new_stack_and_hand_cards>(static_cast<new_stack_and_hand_cards const &>(*this));
}

std::string hand_and_last_card::get_informationstate_string(simulator::cribbage* game, int player)  {
    int n = game->get_player_hand_size(player);
    int* card_ranks = new int[n];
    for (int i = 0; i < n; i++) {
        card_ranks[i] = game->get_player_hand(player)->get_card(i)->get_value(true);
    }
    std::sort(card_ranks, card_ranks + n);

    std::string result = "";
    result.append(std::to_string(game->get_num_available_actions()) + "_");
    for (int i = 0; i < n; i++){
        result.append(std::to_string(card_ranks[i]) + "_");
    }
    
    result.append("|");

    if (game->get_num_cards_played() > 0) {
        result.append(std::to_string(game->get_cards_played()[game->get_num_cards_played()-1].get_value(true)) + "_");
    }

    delete[] card_ranks;    
    return result;
}

std::shared_ptr<information_abstraction> hand_and_last_card::clone() {
    return std::make_shared<hand_and_last_card>(static_cast<hand_and_last_card const &>(*this));
}

std::string hand_and_last_card_and_sum::get_informationstate_string(simulator::cribbage* game, int player) {
    int n = game->get_player_hand_size(player);
    int* card_ranks = new int[n];
    int sum = 0;
    for (int i = 0; i < n; i++) {
        card_ranks[i] = game->get_player_hand(player)->get_card(i)->get_value(true);
    }
    std::sort(card_ranks, card_ranks + n);

    std::string result = "";
    for (int i = 0; i < n; i++){
        result.append(std::to_string(card_ranks[i]) + "_");
    }
    
    result.append("|");

    if (game->get_num_cards_played() > 0) {
        result.append(std::to_string(game->get_cards_played()[game->get_num_cards_played()-1].get_value(true)) + "_");
    }

    result.append(std::to_string(game->get_sum_cards_played()) + "_");

    delete[] card_ranks;    
    return result;
}

std::shared_ptr<information_abstraction> hand_and_last_card_and_sum::clone() {
    return std::make_shared<hand_and_last_card_and_sum>(static_cast<hand_and_last_card_and_sum const &>(*this));
}

std::string greedy_view::get_informationstate_string(simulator::cribbage* game, int player) {
    int n = game->get_player_hand_size(player);
    int* card_ranks = new int[n];
    for (int i = 0; i < n; i++) {
        card_ranks[i] = game->get_player_hand(player)->get_card(i)->get_value(true);
    }

    std::string result = "";
    for (int i = 0; i < n; i++){
        result.append(std::to_string(card_ranks[i]) + "_");
    }
    
    result.append("|");

    for (int i = 0; i < game->get_num_cards_played(); i++) {
        result.append(std::to_string(game->get_cards_played()[i].get_value(true)) + "_");
    }

    delete[] card_ranks;    
    return result;
}

std::shared_ptr<information_abstraction> greedy_view::clone() {
    return std::make_shared<greedy_view>(static_cast<greedy_view const &>(*this));
}


int play_card_from_hand::get_num_available_actions(simulator::cribbage* game) {
    return game->get_num_available_actions();
}

void play_card_from_hand::apply_action_from_list(simulator::cribbage* game, int index, bool from_internal) {
    game->apply_action_from_list(index);
}

std::shared_ptr<action_abstraction> play_card_from_hand::clone() {
    return std::make_shared<play_card_from_hand>(static_cast<play_card_from_hand const &>(*this));
}

int play_permutation::get_num_available_actions(simulator::cribbage* game) {
    if (!game->is_discard_done()) {
        throw std::runtime_error("play_permutation: game is not in playphase");
    }
    return 24;
}

void play_permutation::apply_action_from_list(simulator::cribbage* game, int index, bool from_internal) {
    if (!game->is_discard_done() || game->is_playphase_done()) {
        throw std::runtime_error("play_permutation: game is not in playphase");
    }
    int current_player = game->get_current_player();

    int a;
    int b;
    int c;
    int d;
    
    map_24_to_4(&a, &b, &c, &d, index);
    
    if (current_player == 1) {
        if(!player1_set) {
            player1_set = true;
            
            p1_actions[0] = *game->get_player_hand(1)->get_card(game->get_available_action_indexes()[game->get_available_actions()[a][0]]);
            p1_actions[1] = *game->get_player_hand(1)->get_card(game->get_available_action_indexes()[game->get_available_actions()[b][0]]);
            p1_actions[2] = *game->get_player_hand(1)->get_card(game->get_available_action_indexes()[game->get_available_actions()[c][0]]);
            p1_actions[3] = *game->get_player_hand(1)->get_card(game->get_available_action_indexes()[game->get_available_actions()[d][0]]);
        }
    } else {
        if (!player2_set) {
            player2_set = true;
            p2_actions[0] = *game->get_player_hand(2)->get_card(game->get_available_action_indexes()[game->get_available_actions()[a][0]]);
            p2_actions[1] = *game->get_player_hand(2)->get_card(game->get_available_action_indexes()[game->get_available_actions()[b][0]]);
            p2_actions[2] = *game->get_player_hand(2)->get_card(game->get_available_action_indexes()[game->get_available_actions()[c][0]]);
            p2_actions[3] = *game->get_player_hand(2)->get_card(game->get_available_action_indexes()[game->get_available_actions()[d][0]]);
        }
    }
    

    card c1;
    action action_to_play;
    int legal = 0;
    // std::cout << "before playphase" << std::endl;
    while (!game->is_playphase_done()) {
            current_player = game->get_current_player();
            if(game->get_num_available_actions() == 0) {
                action_to_play = action();
            } else {
                if (current_player == 1) {
                    if (p1_actions_done > 3) {
                        throw std::runtime_error("p1_actions_done more than 3, this is most likely caused by forgetting to reset the abstraction after a round!");
                    }

                    c1 = p1_actions[p1_actions_done];
                    p1_actions_done++;
                } else {
                    if (p2_actions_done > 3) {
                        throw std::runtime_error("p2_actions_done more than 3, this is most likely caused by forgetting to reset the abstraction after a round!");
                    }

                    c1 = p2_actions[p2_actions_done];
                    p2_actions_done++;
                }

                action_to_play = action(&c1);
                // std::cout << "Current player: " << game->get_current_player() << " with hand: ";
                // for (int i=0; i<game->get_player_hand_size(game->get_current_player()); i++) {
                //     std::cout << game->get_player_hand(game->get_current_player())->get_card(i)->string_format() << " ";
                // }
                // std::cout << "Playing card: " << c1.string_format() << std::endl;
                if (!check_valid_move(false, game->get_cards_played(), game->get_num_cards_played(), game->get_sum_cards_played(), nullptr, 0, game->get_player_hand(current_player)->get_cards(), game->get_player_hand(current_player)->get_num_cards(), action_to_play)) {
                    if (current_player == 1) {
                        p1_random = true;
                    } else {
                        p2_random = true;
                    }
                }
            }

            if (p1_random && current_player == 1) {
                // std::cout << "polling random p1" << std::endl;
                action_to_play = r_player.poll_player(!game->is_discard_done(), game->get_player_hand(1), game->get_cards_played(), game->get_num_cards_played(), game->get_sum_cards_played(), game->get_player_hand_size(2), game->get_player1_score(), game->get_player2_score(), game->get_current_pone() == 2);
            }
            if (p2_random && current_player == 2) {
                // std::cout << "polling random p2" << std::endl;
                action_to_play = r_player.poll_player(!game->is_discard_done(), game->get_player_hand(2), game->get_cards_played(), game->get_num_cards_played(), game->get_sum_cards_played(), game->get_player_hand_size(1), game->get_player2_score(), game->get_player1_score(), game->get_current_pone() == 1);
            }

            game->set_play_action(action_to_play);
            legal = game->resolve_action();
            if (legal == 99) {
                std::cout << "24 hands illigal action bro" << std::endl;
            }
            if (!player1_set || !player2_set) {
                break;
            }       
        }
}

void play_permutation::setup_round() {
    player1_set = false;
    player2_set = false;
    p1_random = false;
    p2_random = false;
    p1_actions_done = 0;
    p2_actions_done = 0;
}

std::shared_ptr<action_abstraction> play_permutation::clone() {
    return std::make_shared<play_permutation>(static_cast<play_permutation const &>(*this));
}

high_low_play::high_low_play() {
}

int high_low_play::get_num_available_actions(simulator::cribbage* game) {
    int num = play_card_from_hand::get_num_available_actions(game);
    return std::min(2, num);
}

void high_low_play::apply_action_from_list(simulator::cribbage* game, int index, bool from_internal) {
    /*
     * Index will always be 0 or 1
    */
    if (index == 1) {
        //the last available card is the card with the highest index
        play_card_from_hand::apply_action_from_list(game, play_card_from_hand::get_num_available_actions(game)-1, from_internal);
    } else {
        //if index is 0
        play_card_from_hand::apply_action_from_list(game, index, from_internal);
    }
}

std::shared_ptr<action_abstraction> high_low_play::clone() {
    return std::make_shared<high_low_play>(static_cast<high_low_play const &>(*this));
}

int greedy_play::get_num_available_actions(simulator::cribbage* game) {
    return game->get_player_hand(game->get_current_player())->get_num_cards();
}


void greedy_play::apply_action_from_list(simulator::cribbage* game, int index, bool from_internal) {
    int r_index = -1;
    for (int i = 0; i < get_num_available_actions(game); i++) {
        // if (!check_valid_move(!game->is_discard_done(), game->get_cards_played(), game->get_num_cards_played(), game->get_sum_cards_played(), nullptr, 0, game->get_player_hand(game->get_current_player())->get_cards(), game->get_player_hand_size(game->get_current_player()), action(game->get_player_hand(game->get_current_player())->get_card(game->get_available_action_indexes()[game->get_available_actions()[i][0]])))) {
        //     i++;
        // }
        if (game->get_available_action_indexes()[game->get_available_actions()[i][0]] == index) {
            r_index = i;
            break;
        }
    }
    game->apply_action_from_list(r_index);
}

std::shared_ptr<action_abstraction> greedy_play::clone() {
    return std::make_shared<greedy_play>(static_cast<greedy_play const &>(*this));
}


meta_play::meta_play() : gen(-1) {
    seed = -1;
}

meta_play::meta_play(int in_seed) : gen(in_seed) {
    seed=in_seed;
}

int meta_play::get_num_available_actions(simulator::cribbage* game) {
    return policies.size();
}

void meta_play::apply_action_from_list(simulator::cribbage* game, int index, bool from_internal) {
    // std::cout << "index " << index << " adjusted: " << info_index[index] << " " << action_index[index] << std::endl;
    if (game->is_playphase_done()) {
        throw std::runtime_error("Meta_play: Game is not in play phase");
    }
    // if the policy indexes are set it means that it should play the chosen index in the correct abstraction
    if (game->get_current_player() == 1 && p1_policy_index != -1) {
        actions[action_index[p1_policy_index]]->apply_action_from_list(game, index, from_internal);
        return;
    }
    if (game->get_current_player() == 2 && p2_policy_index != -1) {
        actions[action_index[p2_policy_index]]->apply_action_from_list(game, index, from_internal);
        return;
    }

    //If the policy indexes are not set this means that this is the action for choice of policy to follow.    
    if (index >= info_index.size()) {
        throw std::runtime_error("Index >= info_index.size()");
    }
    if (info_index[index] >= infos.size()) {
        throw std::runtime_error("info_index[index] >= infos.size()");
    }
    
    if (index != -1) {
        if (game->get_current_player() == 1) {
            p1_policy_index = index;
        } else {
            p2_policy_index = index;
        }
    }
    return;
    // std::string key = infos[info_index[index]]->get_informationstate_string(game, game->get_current_player());
    // std::vector<double> ac = policies[index]->action_probabilities(key, game->get_num_available_actions());
    

    // std::discrete_distribution<int> d(ac.begin(), ac.end());
    // int sampled_action_index = d(gen);
    // // std::cout << "sampled index " << sampled_action_index << std::endl;
    // actions[action_index[index]]->apply_action_from_list(game, sampled_action_index, from_internal);
    
    // int current_index = -1;
    // while (p1_policy_index > -1 && p2_policy_index > -1 && !game->is_playphase_done()){
    //     //follow policy for both players until the end of the game.
    //     if (game->get_current_player() == 1) {
    //         current_index = p1_policy_index;
    //     } else {
    //         current_index = p2_policy_index;
    //     }
    //     key = infos[info_index[current_index]]->get_informationstate_string(game, game->get_current_player());
    //     ac = policies[current_index]->action_probabilities(key, actions[action_index[current_index]]->get_num_available_actions(game));
    //     std::discrete_distribution<int> distrib(ac.begin(), ac.end());
    //     sampled_action_index = distrib(gen);
    //     actions[action_index[current_index]]->apply_action_from_list(game, sampled_action_index, from_internal);
    // }
}

bool meta_play::add_policy(policy* p, std::shared_ptr<information_abstraction> info_ab, std::shared_ptr<action_abstraction> act_ab) {
    /*
     * adds the policy to the list.
     * Adds the informations abstraction to the set of informations abstractions if it is not present and points the policy to that abstraction in the set
     * Adds the action abstraction to the set of action abstractions if it is not already present, adds a pointer to the correct action abstraction.
    */
    
    policies.push_back(p);
    int i_idx = -1;
    int a_idx = -1;
    for (int i=0; i<infos.size(); i++) {
        // std::cout << typeid(*info_ab).name() << " " << typeid(*infos[i]).name()<< std::endl;
        if (typeid(*infos[i]) == typeid(*info_ab)) {
            i_idx = i;
        }
    }
    for (int i=0; i<actions.size(); i++) {
        // std::cout << typeid(*act_ab).name() << " " << typeid(*actions[i]).name()<< std::endl;
        if (typeid(*actions[i]) == typeid(*act_ab)) {
            a_idx = i;
        }
    }

    if (i_idx == -1) {
        infos.push_back(info_ab);
        info_index.push_back(infos.size()-1);
    } else {
        info_index.push_back(i_idx);
    }
    if (a_idx == -1) {
        actions.push_back(act_ab);
        action_index.push_back(actions.size()-1);
    } else {
        action_index.push_back(a_idx);
    }
    
    return true;
}

void meta_play::setup_round() {
    if (policies.size() == 0) {
        throw std::runtime_error("Trying to train meta abstraction with no policies set!");
    }
    for (auto info : infos) {
        info->setup_round();
    }
    for (auto act : actions) {
        act->setup_round();
    }
    p1_policy_index = -1;
    p2_policy_index = -1;
}

bool meta_play::is_chance_node(simulator::cribbage* game) {
    if (game->get_current_player() == 1 && p1_policy_index != -1 && !game->is_playphase_done()) {
        return true;
    }
    if (game->get_current_player() == 2 && p2_policy_index != -1 && !game->is_playphase_done()) {
        return true;
    }
    return false;
}

std::vector<double> meta_play::chance_outcomes(simulator::cribbage* game) {
    std::string key;
    std::vector<double> ac;
    if (game->get_current_player() == 1 && p1_policy_index != -1) {
        key = infos[info_index[p1_policy_index]]->get_informationstate_string(game, game->get_current_player());
        ac = policies[p1_policy_index]->action_probabilities(key, actions[action_index[p1_policy_index]]->get_num_available_actions(game));
    } else if (game->get_current_player() == 2 && p2_policy_index != -1) {
        key = infos[info_index[p2_policy_index]]->get_informationstate_string(game, game->get_current_player());
        ac = policies[p2_policy_index]->action_probabilities(key, actions[action_index[p2_policy_index]]->get_num_available_actions(game));
    } else {
        throw std::runtime_error("meta_play::chance_outcomes - most likely chance outcome called without is_chancenode called first");
    }
    return ac;
}

std::shared_ptr<action_abstraction> meta_play::clone() {
    // std::shared_ptr<meta_play> tmp = std::make_shared<meta_play>(this);

    std::shared_ptr<meta_play> m = std::make_shared<meta_play>(this->seed);
    m->gen = gen;
    for (int i=0; i< actions.size(); i++) {
        m->actions.push_back(actions[i]->clone());
    }
    
    for (int i=0; i< infos.size(); i++) {
        m->infos.push_back(infos[i]->clone());
    }
    m->policies = policies;
    m->action_index = action_index;
    m->info_index = info_index;
    m->p1_policy_index = p1_policy_index;
    m->p2_policy_index = p2_policy_index;

    for (int i=0; i<actions.size(); i++) {
        if (actions[i] == m->actions[i] )
        {
            throw std::runtime_error("Meta_play: The action_abstraction in the copy is the same as the one in the original");
        }
        
    }
    return m;
}


meta_abstraction::meta_abstraction(std::shared_ptr<simulator::cribbage> in_game, int in_seed) :  base_abstraction(in_game, std::make_shared<hand_cards>(), std::make_shared<meta_play>(in_seed), in_seed) {
    name="meta_abstraction";
}

meta_abstraction::meta_abstraction(std::shared_ptr<simulator::cribbage> in_game, std::shared_ptr<information_abstraction> in_info_abstraction, std::shared_ptr<action_abstraction> in_act_abstraction, int in_seed, double in_scaling) : base_abstraction(in_game, in_info_abstraction, in_act_abstraction, in_seed, in_scaling) {
    meta_play* valid_cast = dynamic_cast<meta_play*>(in_act_abstraction.get());
    if (typeid(*valid_cast) != typeid(meta_play)) {
        throw std::runtime_error("Meta abstraction is only compatible with meta_play abstraction");
    }
    name="meta_abstraction";
}



void meta_abstraction::setup_round(bool skip_game) {
    abstraction::setup_round(skip_game);
    if (!skip_game) {
        game->skip_to_play_phase();
    }
}

bool meta_abstraction::add_policy(policy* p, std::shared_ptr<information_abstraction> info_ab, std::shared_ptr<action_abstraction> act_ab) {
    /*
     * adds the policy to the list.
     * Adds the informations abstraction to the set of informations abstractions if it is not present and points the policy to that abstraction in the set
     * Adds the action abstraction to the set of action abstractions if it is not already present, adds a pointer to the correct action abstraction.
    */
    name += p->name;
    return dynamic_cast<meta_play*>(act_abstraction.get())->add_policy(p, info_ab, act_ab);
}



new_stack_4_choose_1_abstraction::new_stack_4_choose_1_abstraction(std::shared_ptr<simulator::cribbage> in_game, double in_scaling) : base_abstraction(in_game, std::make_shared<new_stack_and_hand_cards>(), std::make_shared<play_card_from_hand>(), 0, in_scaling) {
    name = "new_stack_4_choose_1_abstraction";
}

new_stack_4_choose_1_abstraction::new_stack_4_choose_1_abstraction(std::shared_ptr<simulator::cribbage> in_game, std::shared_ptr<information_abstraction> in_info_abstraction, std::shared_ptr<action_abstraction> in_act_abstraction, int in_seed, double in_scaling) : base_abstraction(in_game, in_info_abstraction, in_act_abstraction, in_seed, in_scaling) {
    name = "new_stack_4_choose_1_abstraction";
}

void new_stack_4_choose_1_abstraction::setup_round(bool skip_game) {
    abstraction::setup_round(skip_game);
    if (!skip_game) {
        game->skip_to_play_phase();
    }
}


hand_cards_choose_24::hand_cards_choose_24(std::shared_ptr<simulator::cribbage> in_game) : base_abstraction(in_game, std::make_shared<hand_cards>(), std::make_shared<play_permutation>(), 0) {
    name = "hand_cards_choose_24";
}

hand_cards_choose_24::hand_cards_choose_24(std::shared_ptr<simulator::cribbage> in_game, std::shared_ptr<information_abstraction> in_info_abstraction, std::shared_ptr<action_abstraction> in_act_abstraction, int in_seed, double in_scaling) : base_abstraction(in_game, in_info_abstraction, in_act_abstraction, in_seed, in_scaling) {
    name = "hand_cards_choose_24";
}

void hand_cards_choose_24::setup_round(bool skip_game) {
    abstraction::setup_round(skip_game);
    if (!skip_game) {
        game->skip_to_play_phase();
    }
}


high_low_abstraction::high_low_abstraction(std::shared_ptr<simulator::cribbage> in_game) : base_abstraction(in_game, std::make_shared<new_stack_and_hand_cards>(), std::make_shared<high_low_play>(), 0){
    name = "high_low_abstraction";
}

high_low_abstraction::high_low_abstraction(std::shared_ptr<simulator::cribbage> in_game, std::shared_ptr<information_abstraction> in_info_abstraction, std::shared_ptr<action_abstraction> in_act_abstraction, int in_seed, double in_scaling) : base_abstraction(in_game, in_info_abstraction, in_act_abstraction, in_seed, in_scaling) {
    name = "high_low_abstraction";
}


void high_low_abstraction::setup_round(bool skip_game) {
    abstraction::setup_round(skip_game);
    if (!skip_game) {
        game->skip_to_play_phase();
    }
}

hand_last_card_abstraction::hand_last_card_abstraction(std::shared_ptr<simulator::cribbage> in_game) : base_abstraction(in_game, std::make_shared<hand_and_last_card>(), std::make_shared<play_card_from_hand>(), 0){
    name = "hand_last_card_abstraction";
}

hand_last_card_abstraction::hand_last_card_abstraction(std::shared_ptr<simulator::cribbage> in_game, std::shared_ptr<information_abstraction> in_info_abstraction, std::shared_ptr<action_abstraction> in_act_abstraction, int in_seed, double in_scaling) : base_abstraction(in_game, in_info_abstraction, in_act_abstraction, in_seed, in_scaling) {
    name = "hand_last_card_abstraction";
}


void hand_last_card_abstraction::setup_round(bool skip_game) {
    abstraction::setup_round(skip_game);
    if (!skip_game) {
        game->skip_to_play_phase();
    }
}

hand_last_card_sum_abstraction::hand_last_card_sum_abstraction(std::shared_ptr<simulator::cribbage> in_game) : base_abstraction(in_game, std::make_shared<hand_and_last_card_and_sum>(), std::make_shared<play_card_from_hand>(), 0){
    name = "hand_last_card_sum_abstraction";
}


hand_last_card_sum_abstraction::hand_last_card_sum_abstraction(std::shared_ptr<simulator::cribbage> in_game, std::shared_ptr<information_abstraction> in_info_abstraction, std::shared_ptr<action_abstraction> in_act_abstraction, int in_seed, double in_scaling) : base_abstraction(in_game, in_info_abstraction, in_act_abstraction, in_seed, in_scaling) {
    name = "hand_last_card_sum_abstraction";
}


void hand_last_card_sum_abstraction::setup_round(bool skip_game) {
    abstraction::setup_round(skip_game);
    if (!skip_game) {
        game->skip_to_play_phase();
    }
}


greedy_abstraction::greedy_abstraction(std::shared_ptr<simulator::cribbage> in_game) : base_abstraction(in_game, std::make_shared<greedy_view>(), std::make_shared<greedy_play>(), 0) {
    name="greedy_abstaction";
}
greedy_abstraction::greedy_abstraction(std::shared_ptr<simulator::cribbage> in_game, std::shared_ptr<information_abstraction> in_info_abstraction, std::shared_ptr<action_abstraction> in_act_abstraction, int in_seed, double in_scaling) : base_abstraction(in_game, in_info_abstraction, in_act_abstraction, in_seed, in_scaling) {
    name = "greedy_abstraction";
}

void greedy_abstraction::setup_round(bool skip_game) {
    abstraction::setup_round(skip_game);
    if (!skip_game) {
        game->skip_to_play_phase();
    }
}

br_abstraction::br_abstraction(std::shared_ptr<simulator::cribbage> in_game, abstraction* opp_ab) : base_abstraction(in_game, opp_ab->get_information_abstraction(), opp_ab->get_action_abstraction()) {
    for(int i=0; i<4; i++) {
        self_card_ranks[i] = 0;
    }
    for(int i=0; i<4; i++) {
        opp_card_ranks[i] = 1;
    }
    name = "br_abstraction";
}

br_abstraction::br_abstraction(std::shared_ptr<simulator::cribbage> in_game, std::shared_ptr<information_abstraction> in_info_abstraction, std::shared_ptr<action_abstraction> in_act_abstraction, int in_seed, double in_scaling) : base_abstraction(in_game, in_info_abstraction, in_act_abstraction, in_seed, in_scaling) {
    name = "br_abstraction";
}

void br_abstraction::setup_round(bool skip_game) {
    abstraction::setup_round(skip_game);
    if (!skip_game) {
        game->skip_to_play_phase();
        set_self_hand();
    }
}

void br_abstraction::set_self_hand() {
    for (int i=0; i<4; i++) {
        seen_cards[i] = self_card_ranks[i];
    }
    hand* player_hand = game->get_player_hand(1);
    card* cards = new card[4];
    for (int i = 0; i < 4; i++){
        cards[i] = card(self_card_ranks[i] + 1, (char) i);
    }
    
    player_hand->set_cards(cards, 4);
    delete cards;
}


void br_abstraction::increment_ranks(int index) {
    int rank = (self_card_ranks[index] + 1)%(MAX_NUM_DECKCARDS/4);
    self_card_ranks[index] = rank;
    if (rank == 0) {
        increment_ranks(index-1);
    }
}

bool br_abstraction::all_visited() {
    /*
     * The last infostate visited is player1: 13, 13, 13, 13 - player2: 12, 12, 12, 12
    */
    for(int i=0; i<4; i++) {
        if (self_card_ranks[i] != MAX_NUM_DECKCARDS/4) {
            return false;
        }
    }
    return true;
}

void br_abstraction::reset(int next_dealer) {
    game->reset(next_dealer);
}

// bool br_abstraction::all_opp_visited(int num_opp_cards) {
//     /*
//      * The last infostate visited is player1: 13, 13, 13, 13 - player2: 12, 12, 12, 12
//     */
//     for(int i=num_opp_cards-1; i>=0; i--) {
//         if (opp_card_ranks[i] != 0) {
//             return false;
//         }
//     }
//     return true;
// }

bool br_abstraction::increment_opp(int index) {
    
    int rank = opp_card_ranks[index] + 1;
    if (rank == MAX_NUM_DECKCARDS/4) {
        if (index == 0) {
            all_opp_visited = true;
            return true;
        }
        if (increment_opp(index-1)) {
            return true;
        }
        rank = opp_card_ranks[index-1];
    }
    opp_card_ranks[index] = rank;
    seen_cards[index] = rank;
    int num_of_rank = 0;
    for(int i=0; i<8; i++) {
        if (rank == seen_cards[i]) {
            num_of_rank++;
        }
    }
    if (num_of_rank > 4) {
        return increment_opp(index);
    }
    return false;
}


int br_abstraction::get_num_opp_hands(int update_player) {
    int opp_player = 3 - update_player;
    int num_cards_in_hand = game->get_player_hand_size(update_player);
    card* player_hand = game->get_player_hand(update_player)->get_cards();
    int num_cards_played = game->get_num_cards_played();
    card* cards_played = game->get_cards_played();

    int num_opp_cards = game->get_player_hand_size(opp_player);
    bool done = false;
    int counter = 0;
    int num_rank_0 = 0;


    for (int i = num_opp_cards; i < num_opp_cards+num_cards_in_hand; i++) {
        seen_cards[i] = player_hand[i-num_opp_cards].get_value(true)-1;
    }
    for (int i = num_opp_cards+num_cards_in_hand; i < num_opp_cards+num_cards_in_hand+num_cards_played; i++) {
        seen_cards[i] = cards_played[i-num_opp_cards-num_cards_in_hand].get_value(true)-1;
    }
    for (int i = num_opp_cards; i < num_opp_cards+num_cards_in_hand+num_cards_played; i++) {
        if (seen_cards[i] == 0) {
            num_rank_0++;
        } 
    }
    
    for (int i = 0; i < num_opp_cards; i++) {
        if (num_rank_0 < 4) {
            opp_card_ranks[i] = 0;
            num_rank_0++;
        } else {
            opp_card_ranks[i] = 1;
        }
        
    }
    opp_card_ranks[num_opp_cards-1] = -1;
    
    while (!increment_opp(num_opp_cards-1)) {
        hand_map[counter] = std::vector<int>();
        for (int i = 0; i < num_opp_cards; i++) {
            hand_map[counter].push_back(opp_card_ranks[i]);
        }
        counter++;
    }
    
    return counter;
}

void br_abstraction::set_opp_hand(int hand_number, int update_player) {
    int opp_player = 3 - update_player;
    std::vector<int> ranks = hand_map[hand_number];
    hand* player_hand = game->get_player_hand(opp_player);
    card* cards = new card[ranks.size()];
    for (int i = 0; i < ranks.size(); i++){
        cards[i] = card(ranks[i] + 1, (char) i+(4*player_hand->get_num_cards()));
    }
    
    player_hand->set_cards(cards, ranks.size());
    delete cards;
}

br_abstraction* br_abstraction::new_initial_state() {
    br_abstraction* next_state = new br_abstraction(std::make_shared<simulator::cribbage>(*(game.get())),
                                              info_abstraction->clone(),
                                              act_abstraction->clone(),
                                              seed
                                              );
    for(int i=0; i<4; i++) {
        next_state->self_card_ranks[i] = self_card_ranks[i];
    }
    for(int i=0; i<4; i++) {
        next_state->opp_card_ranks[i] = 0;
    }

    next_state->setup_round();
    increment_ranks();
    return next_state;
}

br_abstraction* br_abstraction::child(int action_index) {
    int num_available_actions = get_num_available_actions();

    if (max_num_children != num_available_actions) {
        child_created.clear();
        max_num_children = num_available_actions;
        if (max_num_children > 0) {
            for (int i = 0; i < max_num_children; i++) {
                child_created.push_back(false);
            }
        }
    }

    while (children.size() <= num_available_actions) {
        children.push_back(nullptr);
    }

    if (action_index >= num_available_actions || action_index < -1) {
        throw std::runtime_error("action_index is not within actionspace");
    }
    if (action_index == -1) {
        children[0] = std::make_shared<br_abstraction>(std::shared_ptr<simulator::cribbage>(new simulator::cribbage( *(game.get()) )),
                                                info_abstraction->clone(),
                                                act_abstraction->clone(),
                                                seed);
        children[0]->apply_action_from_list(-1);
        child_created[0] = true;
        action_index = 0;
    }
    if (!child_created[action_index]) {
        children[action_index] = std::make_shared<br_abstraction>(std::shared_ptr<simulator::cribbage>(new simulator::cribbage( *(game.get()) )),
                                                info_abstraction->clone(),
                                                act_abstraction->clone(),
                                                seed);
        children[action_index]->apply_action_from_list(action_index);
        child_created[action_index] = true;
    }

    return static_cast<br_abstraction*>(children[action_index].get());
}
