#pragma once
#include "experiments.h"




int main(int argc, char const *argv[]) {

    int status = 0;


    policy lcs_policy = policy();
    policy ns_policy = policy();

    lcs_policy.deserialize("C:/Users/tor-d/git/cribbageAI/precomputed_policies/52_card_deck_mccfr_policy_hand_last_card_sum_abstraction_100000000.bin");
    ns_policy.deserialize("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_new_stack_4_choose_1_abstraction_100000000.bin");
    
    
    cout << "LCS size is: " << lcs_policy.size() << endl;
    cout << "NS size is: " << ns_policy.size() << endl;
    
    ofstream logger("BIG_POLICY_SIZES.txt", ios_base::app);

    logger << "LCS size is: " << lcs_policy.size() << endl;
    logger << "NS size is: " << ns_policy.size() << endl;
    
    logger.flush();
    logger.close();


    // simulator::cribbage game = simulator::cribbage(0);
    // shared_ptr<simulator::cribbage> game_ptr = make_shared<simulator::cribbage>(game);

    // meta_abstraction meta = meta_abstraction(game_ptr, 0);
    // greedy_abstraction greedy_ab = greedy_abstraction(game_ptr);
    // meta.add_policy(new greedypolicy(), greedy_ab.get_information_abstraction(), greedy_ab.get_action_abstraction());

    // hand_last_card_sum_abstraction ab = hand_last_card_sum_abstraction(game_ptr);

    // train_and_test(10'000'000, 0, &ab, "TEST_last_card_sum_vs_meta_only_greedy", nullptr, &meta);


    try {
        // status = test_abstractions();
    } catch (runtime_error e) {
        cout << e.what() << endl;
        cout << "test abstractions failed" << endl;
    }


    try {
        status = test_restricted_policies();
    } catch (runtime_error e) {
        cout << e.what() << endl;
        cout << "restricted policies failed" << endl;
    }

    

    try {
        // status = test_meta();
    } catch (runtime_error e) {
        cout << e.what() << endl;
        cout << "test meta failed" << endl;
    }

    try {
        // status = test_meta_opp();
    } catch (runtime_error e) {
        cout << e.what() << endl;
        cout << "test meta_opp failed" << endl;
    }

    try {
        // status = test_different_starting_points();
    } catch (runtime_error e) {
        cout << e.what() << endl;
        cout << "test generating policies failed" << endl;
    }


    return status;
}
