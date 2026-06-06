#pragma once
#include "simulator/player.h"
#include "simulator/greedyplayer.h"
#include "simulator/cribbage.h"
#include "simulator/mccfrplayer.h"
#include "simulator/statistics.h"
#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>

using namespace std;


policy* load_or_create_policy(string filename);
int qualitative_hand_test(vector<string> filenames, vector<abstraction*> abs, int seed, ofstream* logger);
int round_robin_test(int num_tests, vector<string> filenames, vector<abstraction*> abs, int seed, ofstream* logger);
int train(int num_iter, int train_seed, abstraction* ab, string run_name, policy* opponent_pol=nullptr, abstraction* opp_ab=nullptr);
int train_and_test(int num_iter, int train_seed, abstraction* ab, string run_name, policy* opponent_pol=nullptr, abstraction* opp_ab=nullptr);
int test_restricted_policies();
int test_abstractions();
int test_meta();
int test_meta_opp();
int test_growing_generating_policies(policy* start_pol, abstraction* start_ab);
int test_growing_best_response(policy* start_pol, abstraction* start_ab);
int test_different_starting_points();
