#include "cheatingutcplayer.h"


node::node() {
    init();
}

node::node(simulator::cribbage in_game, int in_action, node* in_parent) {
    game_state = in_game;
    action = in_action;
    parent = in_parent;
    init();
}

void node::init() {
    rewards[0]=0;
    rewards[1]=0;
}

simulator::cribbage* node::get_state() {
    return &game_state;
}

void node::backup_visit(double* payoffs) {
    visits++;
    for (int i = 0; i < 2; i++) {
        rewards[i] += payoffs[i];
    }
    
}

node* node::get_parent() {
    return parent;
}

int node::get_action() {
    return action;
}

node* node::add_child(node *child) {
    num_children++;
    child_nodes.push_front(child);
    return child;
}

int node::get_visits() {
    return visits;
}

double node::get_reward(int player) {
    // add check that player is 0 or 1?
    if (player == 0 || player == 1) {
        return rewards[player];
    }
    std::cout << "Node get_reward expects player to be 0 or 1, change if that is not the case" << std::endl;
    return rewards[player]; //better to have the game crash
}

int node::get_num_children() {
    return num_children;
}

std::forward_list<node*>* node::get_children() {
    return &child_nodes;
}

bool node::is_fully_expanded() {
    return fully_expanded;
}

void node::set_fully_expanded() {
    fully_expanded = true;
}


cheatingutcplayer::cheatingutcplayer(double in_cp, long in_max_time, int in_max_nodes, std::mt19937 *in_gen) {
    cp = in_cp;
    max_time = in_max_time;
    max_nodes = in_max_nodes;
    gen = in_gen;
}

bool cheatingutcplayer::timeout() {
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time);
    if (max_time > 0 && duration.count() > max_time) {
        return true;
    }
    if (max_nodes > 0 && nodes_expanded > max_nodes) {
        return true;
    }
    return false;
}


node* cheatingutcplayer::tree_policy(node* n) {
    // while round is not over
    while (!n->get_state()->is_round_done()) {
        if (n->is_fully_expanded()) {
            n = get_best_child(n, cp);
        }
        else {
            return expand(n);
        }
    }
	return n;
}

node *cheatingutcplayer::expand(node *n) {
    int action = n->get_num_children(); 
    node* new_node = n->add_child(new node(simulator::cribbage(*n->get_state()), action, n));
    int win = new_node->get_state()->apply_action_from_list(action);
    if (win == -99) {
        std::cout << "did an illigal move" << std::endl;
    }
    //check for fully expanded
    if (n->get_num_children() == n->get_state()->get_num_available_actions()) {
        n->set_fully_expanded();
    }
    
    return new_node;
}


double* cheatingutcplayer::default_policy(simulator::cribbage *state) {
    int* moves;
    int num_available_actions;
    int index;
    int win = 0;
    while (!state->is_round_done() && win != -99) {
        num_available_actions = state->get_num_available_actions();
        if (num_available_actions > 1) {
            std::uniform_int_distribution<int> distrib(0, state->get_num_available_actions()-1);
            index = distrib(*gen);
        } else {
            index = 0;
        }
        win = state->apply_action_from_list(index);
        if (win == -99) {
        std::cout << "did an illigal move: default policy" << std::endl;
        }

    }
    rewards[0] = state->get_point_diff(1);
    rewards[1] = state->get_point_diff(2);
    return rewards;
}

void cheatingutcplayer::backup(node* n, double* payoffs) {
    while (n != nullptr) {
        n->backup_visit(payoffs);
        n = n->get_parent();
    }  
}

node* cheatingutcplayer::get_best_child(node* n, double c) {
    int player = n->get_state()->get_current_player();
    node *max_child;
    double max = -INFINITY;
    for (node* child : *n->get_children()) {
        //careful with this function when available moves are not the same each rollout
        double val = child->get_reward(player) / child->get_visits() + c * sqrt(2 * log(n->get_visits()) / child->get_visits());
        if (val > max) {
            max = val;
            max_child = child;
        }
    }
    return max_child;
}

void cheatingutcplayer::do_search(simulator::cribbage *state) {
    start_time = std::chrono::high_resolution_clock::now();
    nodes_expanded = 0;
    
    //root node
    root_node = node(*state, -1, nullptr);
    
    while (!timeout()) {
        node *new_node = tree_policy(&root_node);
        double *payoffs = default_policy(new_node->get_state());
        backup(new_node, payoffs);
        nodes_expanded++;
    }
}

std::vector<int> cheatingutcplayer::search_and_return_visit_counts(simulator::cribbage *state) {
    do_search(state);
    std::vector<int> visits(root_node.get_num_children());
    int i = 0;
    for (node *n : *root_node.get_children()) {
        visits[i] = n->get_visits();
        i++;
    }
    return visits;
}

int cheatingutcplayer::search(simulator::cribbage *state) {
    do_search(state);
    return get_best_child(&root_node, 0)->get_action();
}


int cheatingutcplayer::get_move(simulator::cribbage *state) {
    int num_moves = state->get_num_available_actions();
    int* moves = state->get_available_actions();
    if (num_moves == 1) {
        return moves[0];
    }
    else {
        return search(state);
    }
}