#pragma once
#include <chrono>
#include <cmath>
#include <forward_list>

#include "player.h"
#include "utils.h"
#include "cribbage.h"

class node {
    private:
        simulator::cribbage game_state;
        int action;
		double rewards[2];
		int visits=0;
		node *parent;
        int num_children;
		std::forward_list<node*> child_nodes;
		bool fully_expanded;

        void init();


    
    public:
        node();
        node(simulator::cribbage in_game, int action, node* parent);

        simulator::cribbage* node::get_state();
        void node::backup_visit(double* payoffs);
        node* node::get_parent();
        int node::get_action();
        node *node::add_child(node *child);
        int node::get_visits();
        double node::get_reward(int player);
        int get_num_children();
        std::forward_list<node*>* node::get_children();
        bool node::is_fully_expanded();
        void node::set_fully_expanded();

};

class cheatingutcplayer{
    private:
	    double cp;
	    long max_time = 0;
	    std::chrono::steady_clock::time_point start_time;
	    int max_nodes = 0;
	    int nodes_expanded = 0;
        node root_node;
        std::mt19937 *gen;

        double rewards[2];
        
        bool timeout();
        node* tree_policy(node* n);
        node* expand(node* n);
        double* default_policy(simulator::cribbage *state);
        void backup(node* n, double* payoffs);
        node* get_best_child(node* n, double c);
        void do_search(simulator::cribbage *state);

    
    public:
        cheatingutcplayer(double in_cp, long in_max_time, int in_max_nodes, std::mt19937 *in_gen);
        std::vector<int> search_and_return_visit_counts(simulator::cribbage *state); //we will probably need to know the length of the array
        int search(simulator::cribbage *state);
        int get_move(simulator::cribbage *state);



};