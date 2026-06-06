#pragma once

#include "abstraction.h"
#include "utils.h"

#include <chrono>


class statistics {
    private:
    public:
        int set_dealer = -1;

        statistics() {};
        double head_to_head(simulator::cribbage* game, policy* p1, policy* p2, abstraction* ab1, abstraction* ab2, int num_games, std::mt19937* gen, std::ofstream* logger=nullptr, player* discard_policy=nullptr, int num_repeats=1);
};

