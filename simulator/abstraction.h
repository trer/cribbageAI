#pragma once
#include <algorithm>
#include <assert.h>
#include <memory>
#include <string>
#include <unordered_map>


#include "card.h"
#include "cribbage.h"


class information_abstraction {
    public:
        virtual std::string get_informationstate_string(simulator::cribbage* game, int player);
        virtual void setup_round();
        
        virtual std::shared_ptr<information_abstraction> clone();
};

class action_abstraction {
    public:
        virtual int get_num_available_actions(simulator::cribbage* game);
        virtual void apply_action_from_list(simulator::cribbage* game, int index, bool from_internal=true);
        virtual void setup_round();
        virtual bool is_chance_node(simulator::cribbage* game);
        virtual std::vector<double> chance_outcomes(simulator::cribbage* game);
        virtual std::shared_ptr<action_abstraction> clone();
};

class abstraction {
    protected:
        std::shared_ptr<simulator::cribbage> game;
        std::string name = "base_abstraction";

        std::shared_ptr<information_abstraction> info_abstraction;
        std::shared_ptr<action_abstraction> act_abstraction;

        std::vector<std::shared_ptr<abstraction>> children;
        std::vector<bool> child_created;
        int max_num_children;

        bool act_abstraction_set = false;
        bool info_abstraction_set = false;

        int seed;
        double scaling;

    public:
        abstraction();
        abstraction(std::shared_ptr<simulator::cribbage> in_game, std::shared_ptr<information_abstraction> in_info_abstraction, std::shared_ptr<action_abstraction> in_act_abstraction, int in_seed=0, double in_scaling=1.0);

        int get_current_player();
        virtual void setup_round(bool skip_game=false);
        int get_point_diff(int player);
        bool is_terminal();
        virtual bool is_chance_node();
        virtual std::vector<double> chance_outcomes();
        virtual abstraction* child(int action_index) {return nullptr;};
        virtual abstraction* new_initial_state() {return nullptr;};

        virtual std::string get_informationstate_string(int player);
        virtual int get_num_available_actions();
        virtual void apply_action_from_list(int index, bool from_internal=true);

        std::string get_name();
        void set_name(std::string new_name);

        std::shared_ptr<information_abstraction> get_information_abstraction();
        std::shared_ptr<action_abstraction> get_action_abstraction();
        simulator::cribbage* get_game();

};

template<class derived>
class base_abstraction : public abstraction {
    protected:

    public:
        base_abstraction() : abstraction() {};
        base_abstraction(std::shared_ptr<simulator::cribbage> in_game, std::shared_ptr<information_abstraction> in_info_abstraction, std::shared_ptr<action_abstraction> in_act_abstraction, int in_seed=0, double in_scaling=1.0) : abstraction(in_game, in_info_abstraction, in_act_abstraction, in_seed, in_scaling) {};
        abstraction* child(int action_index) override;
        abstraction* new_initial_state() override;
};


class hand_cards : public information_abstraction {
    public:
        std::string get_informationstate_string(simulator::cribbage* game, int player) override;
        std::shared_ptr<information_abstraction> clone() override;
};

class new_stack_and_hand_cards : public information_abstraction {
    public:
        std::string get_informationstate_string(simulator::cribbage* game, int player) override;
        std::shared_ptr<information_abstraction> clone() override;
};

class hand_and_last_card : public information_abstraction {
    public: 
        std::string get_informationstate_string(simulator::cribbage* game, int player) override;
        std::shared_ptr<information_abstraction> clone() override;
};

class hand_and_last_card_and_sum : public information_abstraction {
    public: 
        std::string get_informationstate_string(simulator::cribbage* game, int player) override;
        std::shared_ptr<information_abstraction> clone() override;
};

class greedy_view : public information_abstraction {
    public: 
        std::string get_informationstate_string(simulator::cribbage* game, int player) override;
        std::shared_ptr<information_abstraction> clone() override;
};

class play_card_from_hand : public action_abstraction {
    public:
        virtual int get_num_available_actions(simulator::cribbage* game);
        virtual void apply_action_from_list(simulator::cribbage* game, int index, bool from_internal=true);
        virtual std::shared_ptr<action_abstraction> clone();
};

class play_permutation : public action_abstraction {
    protected:
        bool player1_set = false;
        bool player2_set = false;
        bool p1_random = false;
        bool p2_random = false;

        card p1_actions[4];
        card p2_actions[4];

        int p1_actions_done = 0;
        int p2_actions_done = 0;

        randomplayer r_player = randomplayer();

    public:
        int get_num_available_actions(simulator::cribbage* game) override;
        void apply_action_from_list(simulator::cribbage* game, int index, bool from_internal=true) override;
        void setup_round() override;
        std::shared_ptr<action_abstraction> clone() override;

};

class high_low_play : public play_card_from_hand {

    private:

    public:
        high_low_play();        
        int get_num_available_actions(simulator::cribbage* game) override;
        void apply_action_from_list(simulator::cribbage* game, int index, bool from_internal=true) override;
        std::shared_ptr<action_abstraction> clone() override;
}; 

class greedy_play : public play_card_from_hand {
    public:
        int get_num_available_actions(simulator::cribbage* game) override;
        void apply_action_from_list(simulator::cribbage* game, int index, bool from_internal=true) override;
        std::shared_ptr<action_abstraction> clone() override;
};


class meta_play : public action_abstraction {
    private:
        std::vector<policy*> policies = std::vector<policy*>();
        std::vector<int> info_index = std::vector<int>();
        std::vector<int> action_index = std::vector<int>();
        std::vector<std::shared_ptr<information_abstraction>> infos;
        std::vector<std::shared_ptr<action_abstraction>> actions;

        bool locked = false;
        int seed;
        std::mt19937 gen;

        int p1_policy_index = -1;
        int p2_policy_index = -1;

    public:
        meta_play();
        meta_play(int in_seed);
        int get_num_available_actions(simulator::cribbage* game) override;
        void apply_action_from_list(simulator::cribbage* game, int index, bool from_internal=true) override;
        void setup_round() override;
        bool is_chance_node(simulator::cribbage* game) override;
        std::vector<double> chance_outcomes(simulator::cribbage* game) override;

        bool add_policy(policy* p, std::shared_ptr<information_abstraction> info_ab, std::shared_ptr<action_abstraction> act_ab);

        std::shared_ptr<action_abstraction> clone() override;
};

class new_stack_4_choose_1_abstraction : public base_abstraction<new_stack_4_choose_1_abstraction> {
    
    public:
        new_stack_4_choose_1_abstraction(std::shared_ptr<simulator::cribbage> in_game, double scale_score = 1.0);
        new_stack_4_choose_1_abstraction(std::shared_ptr<simulator::cribbage> in_game, std::shared_ptr<information_abstraction> in_info_abstraction, std::shared_ptr<action_abstraction> in_act_abstraction, int in_seed, double in_scaling=1.0);        
        void setup_round(bool skip_game=false) override;
};

class hand_cards_choose_24 : public base_abstraction<hand_cards_choose_24> {

    public:
        hand_cards_choose_24(std::shared_ptr<simulator::cribbage> in_game);
        hand_cards_choose_24(std::shared_ptr<simulator::cribbage> in_game, std::shared_ptr<information_abstraction> in_info_abstraction, std::shared_ptr<action_abstraction> in_act_abstraction, int in_seed, double in_scaling);

        void setup_round(bool skip_game=false) override;
};

class high_low_abstraction : public base_abstraction<high_low_abstraction> {
    public:
        high_low_abstraction(std::shared_ptr<simulator::cribbage> in_game);
        high_low_abstraction(std::shared_ptr<simulator::cribbage> in_game, std::shared_ptr<information_abstraction> in_info_abstraction, std::shared_ptr<action_abstraction> in_act_abstraction, int in_seed, double in_scaling);

        void setup_round(bool skip_game=false) override;
};

class hand_last_card_abstraction : public base_abstraction<hand_last_card_abstraction> {
    public:
        hand_last_card_abstraction(std::shared_ptr<simulator::cribbage> in_game);
        hand_last_card_abstraction(std::shared_ptr<simulator::cribbage> in_game, std::shared_ptr<information_abstraction> in_info_abstraction, std::shared_ptr<action_abstraction> in_act_abstraction, int in_seed, double in_scaling);

        void setup_round(bool skip_game=false) override;
};


class hand_last_card_sum_abstraction : public base_abstraction<hand_last_card_sum_abstraction> {
    public:
        hand_last_card_sum_abstraction(std::shared_ptr<simulator::cribbage> in_game);
        hand_last_card_sum_abstraction(std::shared_ptr<simulator::cribbage> in_game, std::shared_ptr<information_abstraction> in_info_abstraction, std::shared_ptr<action_abstraction> in_act_abstraction, int in_seed, double in_scaling);

        void setup_round(bool skip_game=false) override;
};

class greedy_abstraction : public base_abstraction<greedy_abstraction> {
    public:
        greedy_abstraction(std::shared_ptr<simulator::cribbage> in_game);
        greedy_abstraction(std::shared_ptr<simulator::cribbage> in_game, std::shared_ptr<information_abstraction> in_info_abstraction, std::shared_ptr<action_abstraction> in_act_abstraction, int in_seed, double in_scaling);

        void setup_round(bool skip_game=false) override;
};

class meta_abstraction : public base_abstraction<meta_abstraction> {

    private:
        
        

    public:
        meta_abstraction(std::shared_ptr<simulator::cribbage> in_game, int in_seed);
        meta_abstraction(std::shared_ptr<simulator::cribbage> in_game, std::shared_ptr<information_abstraction> in_info_abstraction, std::shared_ptr<action_abstraction> in_act_abstraction, int in_seed, double in_scaling);

        
        void setup_round(bool skip_game=false) override;
        // std::string get_informationstate_string(int player);
        
        // bool add_policy(policy* p, information_abstraction* info_ab, action_abstraction* act_ab);
        bool add_policy(policy* p, std::shared_ptr<information_abstraction> info_ab, std::shared_ptr<action_abstraction> act_ab);

};


class br_abstraction : public base_abstraction<br_abstraction> {
    private:

        bool all_opp_visited = false;
        
        std::unordered_map<int, std::vector<int>> hand_map = std::unordered_map<int, std::vector<int>>();

        void increment_ranks(int index=3);
        // bool all_opp_visited(int num_opp_cards);
        bool increment_opp(int index=3);

    public:

        int self_card_ranks[4];
        int opp_card_ranks[4];
        int seen_cards[8];
        
        br_abstraction(std::shared_ptr<simulator::cribbage> in_game, abstraction* opp_ab);
        br_abstraction(std::shared_ptr<simulator::cribbage> in_game, std::shared_ptr<information_abstraction> in_info_abstraction, std::shared_ptr<action_abstraction> in_act_abstraction, int in_seed=0, double in_scaling=1.0);

        
        void setup_round(bool skip_game=false) override;
        bool all_visited();
        void reset(int next_dealer);
        int get_num_opp_hands(int update_player);
        void set_opp_hand(int hand_number, int update_player);
        void set_self_hand();

        br_abstraction* new_initial_state() override;
        br_abstraction* child(int action_index) override;
};