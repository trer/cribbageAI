#include "experiments.h"

policy* load_or_create_policy(string filename) {
    bool success = false;
    policy* p;
    if(filename == "greedypolicy") {
            p = new greedypolicy();
            success = true;
        } else if (filename == "randompolicy") {
            p = new randompolicy();
            success = true;
        } else if (filename == "determenisticpolicy"){
            p = new determenisticpolicy(0);
            success = true;
        } else {
            p = new policy();
            success = p->deserialize(filename);
            p->name = filename;
        }
        
        if (!success) {
            throw runtime_error("policy: " + filename + " could not be loaded");
        }
        return p;
}

int qualitative_hand_test(vector<string> filenames, vector<abstraction*> abs, int seed, ofstream* logger) {
    if (filenames.size() != abs.size()) {
        throw runtime_error("number of policies to load and number of abstractions do not match");
    }

    vector<policy*> policies = vector<policy*>();

    for (int i=0; i <filenames.size(); i++) {
        policies.push_back(load_or_create_policy(filenames[i]));
    }

    std::mt19937 gen(seed);
    mock_deck d = mock_deck();

    mockplayer p1 = mockplayer();
    mockplayer p2 = mockplayer();
    simulator::cribbage game = simulator::cribbage(seed, &p1, &p2, 1, &d);

    card player_and_opp_hand[13];

    *logger << "Test start hand with 5 in hand" << endl;
    //player hand
    player_and_opp_hand[0] = card(10, 'S'); //This will be discarded
    player_and_opp_hand[1] = card(11, 'S'); //This will be discarded
    player_and_opp_hand[2] = card(11, 'D');
    player_and_opp_hand[3] = card(3, 'D');
    player_and_opp_hand[4] = card(5, 'S');
    player_and_opp_hand[5] = card(10, 'C');
    //opp hand
    player_and_opp_hand[6] = card(11, 'H'); //This will be discarded
    player_and_opp_hand[7] = card(7, 'H'); //This will be discarded
    player_and_opp_hand[8] = card(5, 'H');
    player_and_opp_hand[9] = card(13, 'D');
    player_and_opp_hand[10] = card(11, 'S');
    player_and_opp_hand[11] = card(3, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');

    d.set_top_13_cards(player_and_opp_hand);

    game.setup_round();
    game.skip_to_play_phase();

    for (int i=0; i<policies.size(); i++) {
        string state_key = abs[i]->get_information_abstraction()->get_informationstate_string(&game, 2);
        vector<double> ac = policies[i]->action_probabilities(state_key, abs[i]->get_action_abstraction()->get_num_available_actions(&game));
        double sum = 0;
        for (double prob: ac) {
            sum += prob;
        }
        
        *logger << policies[i]->name << " with state key: " << state_key << "has the following probabilies: [";
        for (double prob: ac) {
            *logger << prob /sum << ", ";
        }
        *logger << "]" << endl;

    }
    logger->flush();

    *logger << endl;
    *logger << endl;

    
    *logger << "Test start hand with pair" << endl;
    //player hand
    player_and_opp_hand[0] = card(10, 'S'); //This will be discarded
    player_and_opp_hand[1] = card(11, 'S'); //This will be discarded
    player_and_opp_hand[2] = card(1, 'D');
    player_and_opp_hand[3] = card(3, 'D');
    player_and_opp_hand[4] = card(5, 'S');
    player_and_opp_hand[5] = card(10, 'C');
    //opp hand
    player_and_opp_hand[6] = card(11, 'H'); //This will be discarded
    player_and_opp_hand[7] = card(7, 'H'); //This will be discarded
    player_and_opp_hand[8] = card(1, 'H');
    player_and_opp_hand[9] = card(2, 'D');
    player_and_opp_hand[10] = card(7, 'S');
    player_and_opp_hand[11] = card(2, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');

    d.set_top_13_cards(player_and_opp_hand);
    game.reset(1);
    game.setup_round();
    game.skip_to_play_phase();

    for (int i=0; i<policies.size(); i++) {
        string state_key = abs[i]->get_information_abstraction()->get_informationstate_string(&game, 2);
        vector<double> ac = policies[i]->action_probabilities(state_key, abs[i]->get_action_abstraction()->get_num_available_actions(&game));
        double sum = 0;
        for (double prob: ac) {
            sum += prob;
        }
        
        *logger << policies[i]->name << " with state key: " << state_key << "has the following probabilies: [";
        for (double prob: ac) {
            *logger << prob /sum << ", ";
        }
        *logger << "]" << endl;

    }
    logger->flush();

    *logger << endl;
    *logger << endl;

    *logger << "Test start hand 4 on board" << endl;
    //player hand
    player_and_opp_hand[0] = card(10, 'S'); //This will be discarded
    player_and_opp_hand[1] = card(11, 'S'); //This will be discarded
    player_and_opp_hand[2] = card(1, 'D');
    player_and_opp_hand[3] = card(3, 'D');
    player_and_opp_hand[4] = card(2, 'S');
    player_and_opp_hand[5] = card(10, 'C');
    //opp hand
    player_and_opp_hand[6] = card(11, 'H'); //This will be discarded
    player_and_opp_hand[7] = card(7, 'H'); //This will be discarded
    player_and_opp_hand[8] = card(1, 'H');
    player_and_opp_hand[9] = card(2, 'D');
    player_and_opp_hand[10] = card(7, 'S');
    player_and_opp_hand[11] = card(2, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');

    d.set_top_13_cards(player_and_opp_hand);
    game.reset(1);
    game.setup_round();
    game.skip_to_play_phase();
    game.apply_action_from_list(2);

    for (int i=0; i<policies.size(); i++) {
        string state_key = abs[i]->get_information_abstraction()->get_informationstate_string(&game, 1);
        vector<double> ac = policies[i]->action_probabilities(state_key, abs[i]->get_action_abstraction()->get_num_available_actions(&game));
        double sum = 0;
        for (double prob: ac) {
            sum += prob;
        }
        
        *logger << policies[i]->name << " with state key: " << state_key << "has the following probabilies: [";
        for (double prob: ac) {
            *logger << prob /sum << ", ";
        }
        *logger << "]" << endl;

    }
    logger->flush();

    *logger << endl;
    *logger << endl;




    *logger << "Test run of 3" << endl;
    //player hand
    player_and_opp_hand[0] = card(10, 'S'); //This will be discarded
    player_and_opp_hand[1] = card(11, 'S'); //This will be discarded
    player_and_opp_hand[2] = card(1, 'D');
    player_and_opp_hand[3] = card(2, 'D');
    player_and_opp_hand[4] = card(4, 'S');
    player_and_opp_hand[5] = card(10, 'C');
    //opp hand
    player_and_opp_hand[6] = card(11, 'H'); //This will be discarded
    player_and_opp_hand[7] = card(7, 'H'); //This will be discarded
    player_and_opp_hand[8] = card(1, 'H');
    player_and_opp_hand[9] = card(3, 'D');
    player_and_opp_hand[10] = card(5, 'S');
    player_and_opp_hand[11] = card(3, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');

    d.set_top_13_cards(player_and_opp_hand);
    game.reset(1);
    game.setup_round();
    game.skip_to_play_phase();
    game.apply_action_from_list(1); //playing the 3 for player 2
    game.apply_action_from_list(2); // playing the 4 for player 1



    for (int i=0; i<policies.size(); i++) {
        string state_key = abs[i]->get_information_abstraction()->get_informationstate_string(&game, 2);
        vector<double> ac = policies[i]->action_probabilities(state_key, abs[i]->get_action_abstraction()->get_num_available_actions(&game));
        double sum = 0;
        for (double prob: ac) {
            sum += prob;
        }
        
        *logger << policies[i]->name << " with state key: " << state_key << "has the following probabilies: [";
        for (double prob: ac) {
            *logger << prob /sum << ", ";
        }
        *logger << "]" << endl;

    }
    logger->flush();

    *logger << endl;
    *logger << endl;


    *logger << "Test possible run of 3 (avoid possible run of three)" << endl;
    //player hand
    player_and_opp_hand[0] = card(10, 'S'); //This will be discarded
    player_and_opp_hand[1] = card(11, 'S'); //This will be discarded
    player_and_opp_hand[2] = card(1, 'D');
    player_and_opp_hand[3] = card(2, 'D');
    player_and_opp_hand[4] = card(4, 'S');
    player_and_opp_hand[5] = card(10, 'C');
    //opp hand
    player_and_opp_hand[6] = card(11, 'H'); //This will be discarded
    player_and_opp_hand[7] = card(7, 'H'); //This will be discarded
    player_and_opp_hand[8] = card(1, 'H');
    player_and_opp_hand[9] = card(3, 'D');
    player_and_opp_hand[10] = card(5, 'S');
    player_and_opp_hand[11] = card(3, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');

    d.set_top_13_cards(player_and_opp_hand);
    game.reset(1);
    game.setup_round();
    game.skip_to_play_phase();
    game.apply_action_from_list(1); //playing the 3 for player 2



    for (int i=0; i<policies.size(); i++) {
        string state_key = abs[i]->get_information_abstraction()->get_informationstate_string(&game, 1);
        vector<double> ac = policies[i]->action_probabilities(state_key, abs[i]->get_action_abstraction()->get_num_available_actions(&game));
        double sum = 0;
        for (double prob: ac) {
            sum += prob;
        }
        
        *logger << policies[i]->name << " with state key: " << state_key << "has the following probabilies: [";
        for (double prob: ac) {
            *logger << prob /sum << ", ";
        }
        *logger << "]" << endl;

    }
    logger->flush();

    *logger << endl;
    *logger << endl; 
        
    *logger << "Test 15 or run" << endl;
    //player hand
    player_and_opp_hand[0] = card(10, 'S'); //This will be discarded
    player_and_opp_hand[1] = card(11, 'S'); //This will be discarded
    player_and_opp_hand[2] = card(1, 'D');
    player_and_opp_hand[3] = card(6, 'D');
    player_and_opp_hand[4] = card(4, 'S');
    player_and_opp_hand[5] = card(10, 'C');
    //opp hand
    player_and_opp_hand[6] = card(11, 'H'); //This will be discarded
    player_and_opp_hand[7] = card(7, 'H'); //This will be discarded
    player_and_opp_hand[8] = card(2, 'H');
    player_and_opp_hand[9] = card(5, 'D');
    player_and_opp_hand[10] = card(8, 'S');
    player_and_opp_hand[11] = card(3, 'H');

    //cut
    player_and_opp_hand[12] = card(2, 'H');

    d.set_top_13_cards(player_and_opp_hand);
    game.reset(1);
    game.setup_round();
    game.skip_to_play_phase();
    game.apply_action_from_list(1); //playing the 3 for player 2
    game.apply_action_from_list(1); // playing the 4 for player 1



    for (int i=0; i<policies.size(); i++) {
        string state_key = abs[i]->get_information_abstraction()->get_informationstate_string(&game, 2);
        vector<double> ac = policies[i]->action_probabilities(state_key, abs[i]->get_action_abstraction()->get_num_available_actions(&game));
        double sum = 0;
        for (double prob: ac) {
            sum += prob;
        }
        
        *logger << policies[i]->name << " with state key: " << state_key << "has the following probabilies: [";
        for (double prob: ac) {
            *logger << prob /sum << ", ";
        }
        *logger << "]" << endl;

    }
    logger->flush();

    
    *logger << endl;
    *logger << endl;


    deck new_deck = deck();
    game.set_deck(&new_deck);
    for (int i = 0; i < 5; i++) {
        game.setup_round();
        game.skip_to_play_phase();

        for (int i=0; i<policies.size(); i++) {
            string state_key = abs[i]->get_information_abstraction()->get_informationstate_string(&game, 2);
            vector<double> ac = policies[i]->action_probabilities(state_key, abs[i]->get_action_abstraction()->get_num_available_actions(&game));
            double sum = 0;
            for (double prob: ac) {
                sum += prob;
            }
            
            *logger << policies[i]->name << " with state key: " << state_key << "has the following probabilies: [";
            for (double prob: ac) {
                *logger << prob /sum << ", ";
            }
            *logger << "]" << endl;

        }
        
        *logger << endl;
        *logger << endl;    
        logger->flush();

    }
    
    *logger << endl;
    *logger << endl;


    for (int i = 0; i < 5; i++) {
        game.setup_round();
        game.skip_to_play_phase();
        game.apply_action_from_list(0); //playing the 3 for player 2
        game.apply_action_from_list(0); // playing the 4 for player 1
        

        for (int i=0; i<policies.size(); i++) {
            string state_key = abs[i]->get_information_abstraction()->get_informationstate_string(&game, 2);
            vector<double> ac = policies[i]->action_probabilities(state_key, abs[i]->get_action_abstraction()->get_num_available_actions(&game));
            double sum = 0;
            for (double prob: ac) {
                sum += prob;
            }
            
            *logger << policies[i]->name << " with state key: " << state_key << "has the following probabilies: [";
            for (double prob: ac) {
                *logger << prob /sum << ", ";
            }
            *logger << "]" << endl;

        }
        
        *logger << endl;
        *logger << endl;
        logger->flush();

    }


    *logger << endl;
    *logger << endl;


    for (int i = 0; i < 5; i++) {
        game.setup_round();
        game.skip_to_play_phase();
        game.apply_action_from_list(0); //playing the 3 for player 2
        game.apply_action_from_list(0); // playing the 4 for player 1
        game.apply_action_from_list(0); //playing the 3 for player 2
        
        
        while(game.get_num_available_actions() == 0) {
            game.apply_action_from_list(-1);
        }
        game.apply_action_from_list(0); // playing the 4 for player 1
        
        while(game.get_num_available_actions() == 0) {
            game.apply_action_from_list(-1);
        }
        

        for (int i=0; i<policies.size(); i++) {
            string state_key = abs[i]->get_information_abstraction()->get_informationstate_string(&game, game.get_current_player());
            vector<double> ac = policies[i]->action_probabilities(state_key, abs[i]->get_action_abstraction()->get_num_available_actions(&game));
            double sum = 0;
            for (double prob: ac) {
                sum += prob;
            }
            
            *logger << policies[i]->name << " with state key: " << state_key << "has the following probabilies: [";
            for (double prob: ac) {
                *logger << prob /sum << ", ";
            }
            *logger << "]" << endl;

        }
        
        *logger << endl;
        *logger << endl;
        logger->flush();

    }
    
    *logger << endl;
    *logger << endl;

    logger->flush();
    logger->close();
    for (policy* p : policies) {
        delete p;
    }
    return 0;
}





int round_robin_test(int num_tests, vector<string> filenames, vector<abstraction*> abs, int seed, ofstream* logger) {
    if (filenames.size() != abs.size()) {
        throw runtime_error("number of policies to load and number of abstractions do not match");
    }

    vector<policy*> policies = vector<policy*>();
    bool success = false;
    for (int i=0; i <filenames.size(); i++) {
        policies.push_back(load_or_create_policy(filenames[i]));
    }

    std::mt19937 gen(seed);
    simulator::cribbage game = simulator::cribbage(seed);

    statistics stats = statistics();

    for(int i=0; i < policies.size(); i++) {
        for(int j=0; j < policies.size(); j++) {
            *logger << "Testing: " << filenames[i] << " vs. " << filenames[j] << endl;
            stats.head_to_head(&game, policies[i], policies[j], abs[i], abs[j], num_tests, &gen, logger, nullptr, 10);
        }   
    }
    for (policy* p : policies) {
        delete p;
    }
    
    return 0;
}

int train(int num_iter, int train_seed, abstraction* ab, string run_name, policy* opponent_pol, abstraction* opp_ab) {
    
    std::mt19937 gen(train_seed);
    policyplayer p1 = policyplayer(train_seed);
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(train_seed, &p1, &p2);

    // std::string run_name = "C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_108";
    string filename = run_name +".bin"; 
    string log_name = run_name +"_log.txt";

    ofstream logger(log_name, ios_base::app);
    
    if (!logger.is_open()) {
        throw runtime_error("logger has crashed");
    }
    policy* tmp_pol;


    if (std::filesystem::exists(filename)) {
        cout << "Policy found!" << endl;
        tmp_pol = new policy();
        tmp_pol->deserialize(filename); 
        cout << "Policy deserialized" << endl;
        logger << "Policy deserialized from " << filename << endl;
    } else {
        mccfrplayer mccfr = mccfrplayer(ab, 0.06, &gen);
        if (opp_ab != nullptr) {
            if (opponent_pol != nullptr) {
                mccfr.set_opp_policy(opponent_pol, opp_ab); //setting opp and locking
            } else {
                mccfr.set_opp_policy(opponent_pol, opp_ab, false); //setting opp_but not locking
                num_iter /= 2; //acounting for change in mccfr.iteration()
            }
        }

        
        int test_iter = num_iter / 1'00;
        cout << "starting training" << endl;
        logger << "starting training" << endl;
        auto training_start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_iter/test_iter; i++) {
            for (int j = 0; j < test_iter; j++) {
                mccfr.iteration();
            }
            cout << "Amount of regret after " << (i+1)*test_iter << " rounds is: " << mccfr.amount_of_avg_regret((i+1)*test_iter) << endl;
            logger << "Amount of regret after " << (i+1)*test_iter << " rounds is: " << mccfr.amount_of_avg_regret((i+1)*test_iter) << endl;
        }
        auto training_end = std::chrono::high_resolution_clock::now();
        cout << "finished training : " << std::chrono::duration_cast<std::chrono::milliseconds>(training_end - training_start).count() << " milliseconds." << endl;
        logger << "finished training : " << std::chrono::duration_cast<std::chrono::milliseconds>(training_end - training_start).count() << " milliseconds." << endl;

        tmp_pol = mccfr.average_policy();
        tmp_pol->serialize(filename);
        if (mccfr.opp_set && !mccfr.run_with_locked_opp) {
            policy* tmp_opp_pol = mccfr.average_policy_opp();
            tmp_opp_pol->serialize(run_name + "opp_pol.bin");
            delete tmp_opp_pol;
        }
        auto save_done = std::chrono::high_resolution_clock::now();
        cout << "saved policy: " << std::chrono::duration_cast<std::chrono::milliseconds>(save_done - training_end) << " milliseconds." << endl;
        logger << "saved policy: " << std::chrono::duration_cast<std::chrono::milliseconds>(save_done - training_end) << " milliseconds." << endl;
    }


    logger.flush();
    logger.close();
    delete tmp_pol;
    return 0;
}

int train_and_test(int num_iter, int train_seed, abstraction* ab, string run_name, policy* opponent_pol, abstraction* opp_ab) {

    std::mt19937 gen(train_seed);
    policyplayer p1 = policyplayer(train_seed);
    greedyplayer p2 = greedyplayer();
    simulator::cribbage game = simulator::cribbage(train_seed, &p1, &p2);


    abstraction* mccfr_ab = new new_stack_4_choose_1_abstraction(make_shared<simulator::cribbage>(game));
    abstraction* greedy_ab = new greedy_abstraction(make_shared<simulator::cribbage>(game));
    
    statistics stats = statistics();
    player* discard_policy = new randomplayer();

    // std::string run_name = "C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_108";
    string filename = run_name +".bin"; 
    string log_name = run_name +"_log.txt";

    ofstream logger(log_name, ios_base::app);
    
    if (!logger.is_open()) {
        throw runtime_error("logger has crashed");
    }

    
    policy* tmp_pol = new policy();
    policy* random_policy = new policy();
    policy* determenistic = new determenisticpolicy();
    policy* mccfr_policy = new policy();
    mccfr_policy->deserialize("C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_108.bin");
    mccfr_policy->name = "mccfr8";
    // mccfr_policy->deserialize("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_new_stack_4_choose_1_abstraction1.000000_scale1.000000_100000000.bin");
    // mccfr_policy->name = "mccfr9";
    policy* greed_pol = new greedypolicy();

    

    logger << "Testing " << p1.get_name() << " vs. " << p2.get_name() << endl;
    logger << "With " << p1.get_name() << " as player1." << endl;
    logger << "With " << p2.get_name() << " as player2." << endl;
    logger << "The abstraction used is " << ab->get_name() << endl;
    logger << "Deck has: " << MAX_NUM_DECKCARDS << " number of cards in the deck." << endl;


    int num_games = 1'000;
    int greedypoints = 0;
    int mccfrpoints = 0;

    cout << "Testing a random policy" << endl;
    stats.head_to_head(&game, tmp_pol, random_policy, ab, mccfr_ab, num_games, &gen, &logger, discard_policy);
    stats.head_to_head(&game, tmp_pol, mccfr_policy, ab, mccfr_ab, num_games, &gen, &logger, discard_policy);
    stats.head_to_head(&game, tmp_pol, greed_pol, ab, greedy_ab, num_games, &gen, &logger, discard_policy);
    // test_policy(game, num_games, p1_policy, ab, player_is_policy, gen, mccfrpoints, greedypoints, &logger);

    delete tmp_pol;


    if (std::filesystem::exists(filename)) {
        cout << "Policy found!" << endl;
        tmp_pol = new policy();
        tmp_pol->deserialize(filename); 
        cout << "Policy deserialized" << endl;
        logger << "Policy deserialized from " << filename << endl;
    } else {
        mccfrplayer mccfr = mccfrplayer(ab, 0.06, &gen);
        if (opp_ab != nullptr) {
            if (opponent_pol != nullptr) {
                mccfr.set_opp_policy(opponent_pol, opp_ab); //setting opp and locking
            } else {
                mccfr.set_opp_policy(opponent_pol, opp_ab, false); //setting opp_but not locking
                num_iter /= 2; //acounting for change in mccfr.iteration()
            }
        }

        
        int test_iter = num_iter / 1'00;
        cout << "starting training" << endl;
        logger << "starting training" << endl;
        auto training_start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_iter/test_iter; i++) {
            for (int j = 0; j < test_iter; j++) {
                mccfr.iteration();
            }
            cout << "Amount of regret after " << (i+1)*test_iter << " rounds is: " << mccfr.amount_of_avg_regret((i+1)*test_iter) << endl;
            logger << "Amount of regret after " << (i+1)*test_iter << " rounds is: " << mccfr.amount_of_avg_regret((i+1)*test_iter) << endl;
        }
        auto training_end = std::chrono::high_resolution_clock::now();
        cout << "finished training : " << std::chrono::duration_cast<std::chrono::milliseconds>(training_end - training_start).count() << " milliseconds." << endl;
        logger << "finished training : " << std::chrono::duration_cast<std::chrono::milliseconds>(training_end - training_start).count() << " milliseconds." << endl;

        tmp_pol = mccfr.average_policy();
        tmp_pol->serialize(filename);
        if (mccfr.opp_set && !mccfr.run_with_locked_opp) {
            policy* tmp_opp_pol = mccfr.average_policy_opp();
            tmp_opp_pol->serialize(run_name + "opp_pol.bin");
            delete tmp_opp_pol;
        }
        auto save_done = std::chrono::high_resolution_clock::now();
        cout << "saved policy: " << std::chrono::duration_cast<std::chrono::milliseconds>(save_done - training_end) << " milliseconds." << endl;
        logger << "saved policy: " << std::chrono::duration_cast<std::chrono::milliseconds>(save_done - training_end) << " milliseconds." << endl;
    }

    int test_seed = 1;
    std::mt19937 gen2(test_seed);
    policyplayer newp1 = policyplayer(filename, 1);

    simulator::cribbage game2 = simulator::cribbage(5, &newp1, &p2);
    logger << "Size of policy is: " << newp1.size_of_policy() << "." << endl;
    logger << "Testing " << newp1.get_name() << " vs. " << p2.get_name() << endl;

    cout << "Size of policy is: " << newp1.size_of_policy() << "." << endl;
    cout << "Testing " << newp1.get_name() << " vs. " << p2.get_name() << endl;
    
    num_games = 100'000;
    greedypoints = 0;
    mccfrpoints = 0;
    logger.flush();
    
    
    stats.head_to_head(&game2, tmp_pol, random_policy, ab, mccfr_ab, num_games, &gen, &logger, discard_policy);
    stats.head_to_head(&game2, tmp_pol, mccfr_policy, ab, mccfr_ab, num_games, &gen, &logger, discard_policy);
    stats.head_to_head(&game2, tmp_pol, greed_pol, ab, greedy_ab, num_games, &gen, &logger, discard_policy);


    logger.flush();
    logger.close();
    delete mccfr_ab;
    delete greedy_ab;
    delete discard_policy;
    delete tmp_pol;
    delete mccfr_policy;
    delete determenistic;
    return 0;
}

int test_restricted_policies() {
    int seed = 22;
    int status = 0;
    vector<string> filenames = vector<string>();
    vector<abstraction*> abs = vector<abstraction*>();

    simulator::cribbage tmp_game = simulator::cribbage();
    shared_ptr<simulator::cribbage> tmp_ptr = make_shared<simulator::cribbage>(tmp_game);

    int num_iter = 10'000'000;

    

    vector<policy*> opp_hand_pol = vector<policy*>();
    vector<abstraction*> opp_hand_abs = vector<abstraction*>();

    opp_hand_pol.push_back(new randompolicy());
    opp_hand_pol.push_back(new greedypolicy());
    opp_hand_pol.push_back(new determenisticpolicy(0));

    opp_hand_abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));
    opp_hand_abs.push_back(new greedy_abstraction(tmp_ptr));
    opp_hand_abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));

    
    for(int i=0; i<opp_hand_pol.size(); i++) {
        abstraction* ab = new hand_last_card_sum_abstraction(tmp_ptr);
        string run_name = std::to_string(MAX_NUM_DECKCARDS) + "_card_deck_mccfr_policy_LockedOpp" + ab->get_name() +"_vs_" + opp_hand_pol[i]->name + "_" + std::to_string(num_iter);

        status = train(num_iter, seed, ab, run_name, opp_hand_pol[i], opp_hand_abs[i]);
        filenames.push_back(run_name+ ".bin");
        abs.push_back(ab);
    }


    vector<string> opp_pol_names = vector<string>();
    vector<string> opp_pol_names_for_file = vector<string>();
    vector<abstraction*> opp_abs = vector<abstraction*>();

    opp_pol_names.push_back("C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_108.bin");
    opp_pol_names.push_back("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_last_card_sum_abstraction_10000000.bin");
    opp_pol_names.push_back("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_last_card_sum_abstraction_100000000.bin");

    opp_pol_names_for_file.push_back("mcccfr108");
    opp_pol_names_for_file.push_back("hand_card_sum_108");
    opp_pol_names_for_file.push_back("hand_card_sum_109");


    opp_abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));
    opp_abs.push_back(new hand_last_card_sum_abstraction(tmp_ptr));
    opp_abs.push_back(new hand_last_card_sum_abstraction(tmp_ptr));


    for(int i=0; i<opp_pol_names.size(); i++) {
        policy* opp_pol = new policy();
        opp_pol->deserialize(opp_pol_names[i]);
        abstraction* ab = new hand_last_card_sum_abstraction(tmp_ptr);
        string run_name = std::to_string(MAX_NUM_DECKCARDS) + "_card_deck_mccfr_policy_LockedOpp" + ab->get_name() +"_vs_" + opp_pol_names_for_file[i] + "_" + std::to_string(num_iter);

        status = train(num_iter, seed, ab, run_name, opp_pol, opp_abs[i]);
        
        filenames.push_back(run_name+ ".bin");
        abs.push_back(ab);
        delete opp_pol;
    }

    filenames.push_back("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_last_card_sum_abstraction_10000000.bin");
    filenames.push_back("C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_108.bin");
    filenames.push_back("greedypolicy");
    filenames.push_back("randompolicy");
    filenames.push_back("determenisticpolicy");
    
    abs.push_back(new hand_last_card_sum_abstraction(tmp_ptr));
    abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));
    abs.push_back(new greedy_abstraction(tmp_ptr));
    abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));
    abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));

    ofstream logger("locked_opp_hand_last_card_sum_robin_and_qualitative_analysis_v3.txt");
    
    if (!logger.is_open()) {
        throw runtime_error("logger has crashed");
    }

    status = round_robin_test(1'000'000, filenames, abs, seed, &logger);
    status = qualitative_hand_test(filenames, abs, seed, &logger);

    for(policy* p: opp_hand_pol) {
        delete p;
    }
    for(abstraction* a: opp_hand_abs) {
        delete a;
    }
    for(abstraction* a: abs) {
        delete a;
    }
    for(abstraction* a: opp_abs) {
        delete a;
    }
    

    return status;
}

int test_abstractions() {
    int seed = 22;
    int status = 0;
    vector<string> filenames = vector<string>();
    vector<abstraction*> abs = vector<abstraction*>();

    simulator::cribbage tmp_game = simulator::cribbage();
    shared_ptr<simulator::cribbage> tmp_ptr = make_shared<simulator::cribbage>(tmp_game);

    int num_iter = 10'000'000;

    filenames.push_back("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_new_stack_4_choose_1_abstraction_1000000.bin");
    filenames.push_back("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_high_low_abstraction_1000000.bin");
    filenames.push_back("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_last_card_sum_abstraction_1000000.bin");
    filenames.push_back("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_last_card_abstraction_1000000.bin");
    filenames.push_back("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_cards_choose_24_1000000.bin");
    filenames.push_back("greedypolicy");
    filenames.push_back("randompolicy");
    filenames.push_back("determenisticpolicy");


    abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));
    abs.push_back(new high_low_abstraction(tmp_ptr));
    abs.push_back(new hand_last_card_sum_abstraction(tmp_ptr));
    abs.push_back(new hand_last_card_abstraction(tmp_ptr));
    abs.push_back(new hand_cards_choose_24(tmp_ptr));
    abs.push_back(new greedy_abstraction(tmp_ptr));
    abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));
    abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));


    // ofstream logger("abstraction106round_robbin_and_qualitative_analysis.txt");
    
    // if (!logger.is_open()) {
    //     throw runtime_error("logger has crashed");
    // }

    // status = round_robin_test(1'000'000, filenames, abs, seed, &logger);
    // status = qualitative_hand_test(filenames, abs, seed, &logger);

    // logger.flush();
    // logger.close();
    

    // filenames[0] = "C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_108.bin";
    // filenames[1] = "C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_high_low_108.bin";
    // filenames[2] = "C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_last_card_sum_abstraction_10000000.bin";
    // filenames[3] = "C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_last_card_abstraction_10000000.bin";
    // filenames[4] = "C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_cards_choose_24_10000000.bin";

    // abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));
    // abs.push_back(new high_low_abstraction(tmp_ptr));
    // abs.push_back(new hand_last_card_sum_abstraction(tmp_ptr));
    // abs.push_back(new hand_last_card_abstraction(tmp_ptr));
    // abs.push_back(new hand_cards_choose_24(tmp_ptr));

    for(int i=0; i<5; i++) {
        abstraction* ab = abs[i];
        string run_name = std::to_string(MAX_NUM_DECKCARDS) + "_card_deck_mccfr_policy_" + ab->get_name() + "_" + std::to_string(num_iter);

        status = train(num_iter, seed, ab, run_name);
        filenames[i] = run_name+ ".bin";
        // abs.push_back(ab);
    }




    // ofstream logger2("abstraction107round_robbin_and_qualitative_analysis_v2.txt");
    
    // if (!logger2.is_open()) {
    //     throw runtime_error("logger has crashed");
    // }

    // status = round_robin_test(1'000'000, filenames, abs, seed, &logger2);
    // status = qualitative_hand_test(filenames, abs, seed, &logger2);

    // logger2.flush();
    // logger2.close();

    
    num_iter = 100'000'000;

    abstraction* ab = abs[0];
    string run_name = std::to_string(MAX_NUM_DECKCARDS) + "_card_deck_mccfr_policy_" + ab->get_name() + "_" + std::to_string(num_iter);

    status = train(num_iter, seed, ab, run_name);
    filenames.push_back(run_name+ ".bin");
    abs.push_back(ab);

    ab = abs[2];
    run_name = std::to_string(MAX_NUM_DECKCARDS) + "_card_deck_mccfr_policy_" + ab->get_name() + "_" + std::to_string(num_iter);

    status = train(num_iter, seed, ab, run_name);
    filenames.push_back(run_name+ ".bin");
    abs.push_back(ab);

    return status;
}


int test_meta() {
    int seed = 22;
    int status = 0;
    vector<string> filenames = vector<string>();
    vector<abstraction*> abs = vector<abstraction*>();

    simulator::cribbage tmp_game = simulator::cribbage();
    shared_ptr<simulator::cribbage> tmp_ptr = make_shared<simulator::cribbage>(tmp_game);

    int num_iter = 10'000'000;

    policy* rand_pol = new randompolicy();
    policy* determ_low = new determenisticpolicy(0);
    policy* determ_high = new determenisticpolicy(3);
    policy* greedy = new greedypolicy();
    policy* mccfr_new_stack = new policy();
    mccfr_new_stack->deserialize("C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_108.bin");
    mccfr_new_stack->name="mccfr_policy_108";
    policy* mccfr_sum = new policy();
    mccfr_sum->deserialize("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_last_card_sum_abstraction_10000000.bin");
    mccfr_sum->name="52_card_deck_mccfr_policy_hand_last_card_sum_abstraction_10000000";
    policy* mccfr_last_card = new policy();
    mccfr_last_card->deserialize("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_last_card_abstraction_10000000.bin");
    mccfr_last_card->name="52_card_deck_mccfr_policy_hand_last_card_abstraction_10000000";
    
    abstraction* rand_ab = new new_stack_4_choose_1_abstraction(tmp_ptr); 
    abstraction* determ_low_ab = new new_stack_4_choose_1_abstraction(tmp_ptr); 
    abstraction* determ_high_ab = new new_stack_4_choose_1_abstraction(tmp_ptr); 
    abstraction* greedy_ab = new greedy_abstraction(tmp_ptr); 
    abstraction* mccfr_new_stack_ab = new new_stack_4_choose_1_abstraction(tmp_ptr);
    abstraction* mccfr_sum_ab = new hand_last_card_sum_abstraction(tmp_ptr);
    abstraction* mccfr_last_card_ab = new hand_last_card_abstraction(tmp_ptr);



    //test greedy and random policy
    meta_abstraction ab = meta_abstraction(tmp_ptr, seed);
    ab.add_policy(greedy, greedy_ab->get_information_abstraction(), greedy_ab->get_action_abstraction());
    ab.add_policy(rand_pol, rand_ab->get_information_abstraction(), rand_ab->get_action_abstraction());


    string run_name = std::to_string(MAX_NUM_DECKCARDS) + "_card_deck_mccfr_policy_" + ab.get_name() + "_" + std::to_string(num_iter);

    status = train(num_iter, seed, &ab, run_name);
    filenames.push_back(run_name+ ".bin");
    abs.push_back(&ab);

    //test greedy and mccfr policy
    meta_abstraction ab2 = meta_abstraction(tmp_ptr, seed);
    ab2.add_policy(greedy, greedy_ab->get_information_abstraction(), greedy_ab->get_action_abstraction());
    ab2.add_policy(mccfr_new_stack, mccfr_new_stack_ab->get_information_abstraction(), mccfr_new_stack_ab->get_action_abstraction());


    run_name = std::to_string(MAX_NUM_DECKCARDS) + "_card_deck_mccfr_policy_" + ab2.get_name() + "_" + std::to_string(num_iter);

    status = train(num_iter, seed, &ab2, run_name);
    filenames.push_back(run_name+ ".bin");
    abs.push_back(&ab2);


    //test random, greedy and mccfr policy
    meta_abstraction ab3 = meta_abstraction(tmp_ptr, seed);
    ab3.add_policy(greedy, greedy_ab->get_information_abstraction(), greedy_ab->get_action_abstraction());
    ab3.add_policy(mccfr_new_stack, mccfr_new_stack_ab->get_information_abstraction(), mccfr_new_stack_ab->get_action_abstraction());
    ab3.add_policy(rand_pol, rand_ab->get_information_abstraction(), rand_ab->get_action_abstraction());



    run_name = std::to_string(MAX_NUM_DECKCARDS) + "_card_deck_mccfr_policy_" + ab3.get_name() + "_" + std::to_string(num_iter);

    status = train(num_iter, seed, &ab3, run_name);
    filenames.push_back(run_name+ ".bin");
    abs.push_back(&ab3);


    //test random, deterministic, greedy and mccfr policy
    meta_abstraction ab4 = meta_abstraction(tmp_ptr, seed);
    ab4.add_policy(determ_low, determ_low_ab->get_information_abstraction(), determ_low_ab->get_action_abstraction());
    ab4.add_policy(greedy, greedy_ab->get_information_abstraction(), greedy_ab->get_action_abstraction());
    ab4.add_policy(mccfr_new_stack, mccfr_new_stack_ab->get_information_abstraction(), mccfr_new_stack_ab->get_action_abstraction());
    ab4.add_policy(rand_pol, rand_ab->get_information_abstraction(), rand_ab->get_action_abstraction());

    run_name = std::to_string(MAX_NUM_DECKCARDS) + "_card_deck_mccfr_policy_" + ab4.get_name() + "_" + std::to_string(num_iter);

    status = train(num_iter, seed, &ab4, run_name);
    filenames.push_back(run_name+ ".bin");
    abs.push_back(&ab4);


    //test mccfr policies
    meta_abstraction ab5 = meta_abstraction(tmp_ptr, seed);
    ab5.add_policy(mccfr_new_stack, mccfr_new_stack_ab->get_information_abstraction(), mccfr_new_stack_ab->get_action_abstraction());
    ab5.add_policy(mccfr_sum, mccfr_sum_ab->get_information_abstraction(), mccfr_sum_ab->get_action_abstraction());
    ab5.add_policy(mccfr_last_card, mccfr_last_card_ab->get_information_abstraction(), mccfr_last_card_ab->get_action_abstraction());


    run_name = std::to_string(MAX_NUM_DECKCARDS) + "_card_deck_mccfr_policy_" + ab5.get_name() + "_" + std::to_string(num_iter);

    status = train(num_iter, seed, &ab5, run_name);
    filenames.push_back(run_name+ ".bin");
    abs.push_back(&ab5);


    //test new_stack and sum policy
    meta_abstraction ab6 = meta_abstraction(tmp_ptr, seed);
    ab6.add_policy(mccfr_new_stack, mccfr_new_stack_ab->get_information_abstraction(), mccfr_new_stack_ab->get_action_abstraction());
    ab6.add_policy(mccfr_sum, mccfr_sum_ab->get_information_abstraction(), mccfr_sum_ab->get_action_abstraction());

    run_name = std::to_string(MAX_NUM_DECKCARDS) + "_card_deck_mccfr_policy_" + ab6.get_name() + "_" + std::to_string(num_iter);

    status = train(num_iter, seed, &ab6, run_name);
    filenames.push_back(run_name+ ".bin");
    abs.push_back(&ab6);


    filenames.push_back("C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_108.bin");
    filenames.push_back("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_last_card_abstraction_10000000.bin");
    filenames.push_back("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_last_card_sum_abstraction_10000000.bin");
    filenames.push_back("greedypolicy");
    filenames.push_back("randompolicy");
    filenames.push_back("determenisticpolicy");

    abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));
    abs.push_back(new hand_last_card_abstraction(tmp_ptr));
    abs.push_back(new hand_last_card_sum_abstraction(tmp_ptr));
    abs.push_back(new greedy_abstraction(tmp_ptr));
    abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));
    abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));

    ofstream logger("meta_round_robin_and_qualitative_analysis_fixed.txt");
    
    if (!logger.is_open()) {
        throw runtime_error("logger has crashed");
    }

    status = round_robin_test(1'000'000, filenames, abs, seed, &logger);
    status = qualitative_hand_test(filenames, abs, seed, &logger);

    delete rand_pol;
    delete determ_low;
    delete determ_high;
    delete greedy;
    delete mccfr_new_stack;
    delete mccfr_sum;
    delete mccfr_last_card;
    
    delete rand_ab;
    delete determ_low_ab;
    delete determ_high_ab;
    delete greedy_ab;
    delete mccfr_new_stack_ab;
    delete mccfr_sum_ab;
    delete mccfr_last_card_ab;

    return status;
}



int test_meta_opp() {

    ofstream logger("meta_opp_round_robin_and_qualitative_analysis_v3.txt");
    
    if (!logger.is_open()) {
        throw runtime_error("logger has crashed");
    }


    int seed = 22;
    int status = 0;
    string run_name;
    vector<string> filenames = vector<string>();
    vector<abstraction*> abs = vector<abstraction*>();

    simulator::cribbage tmp_game = simulator::cribbage();
    shared_ptr<simulator::cribbage> tmp_ptr = make_shared<simulator::cribbage>(tmp_game);

    int num_iter = 10'000'000;


    policy* rand_pol = new randompolicy();
    policy* determ_low = new determenisticpolicy(0);
    policy* determ_high = new determenisticpolicy(3);
    policy* greedy_pol = new greedypolicy();
    policy* mccfr_new_stack = new policy();
    mccfr_new_stack->deserialize("C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_108.bin");
    mccfr_new_stack->name="mccfr_policy_108";
    policy* mccfr_sum = new policy();
    mccfr_sum->deserialize("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_last_card_sum_abstraction_10000000.bin");
    mccfr_sum->name="52_card_deck_mccfr_policy_hand_last_card_sum_abstraction_10000000";
    policy* mccfr_last_card = new policy();
    mccfr_last_card->deserialize("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_last_card_abstraction_10000000.bin");
    mccfr_last_card->name="52_card_deck_mccfr_policy_hand_last_card_abstraction_10000000";
    
    abstraction* rand_ab = new new_stack_4_choose_1_abstraction(tmp_ptr); 
    abstraction* determ_low_ab = new new_stack_4_choose_1_abstraction(tmp_ptr); 
    abstraction* determ_high_ab = new new_stack_4_choose_1_abstraction(tmp_ptr); 
    abstraction* greedy_ab = new greedy_abstraction(tmp_ptr); 
    abstraction* mccfr_new_stack_ab = new new_stack_4_choose_1_abstraction(tmp_ptr);
    abstraction* mccfr_sum_ab = new hand_last_card_sum_abstraction(tmp_ptr);
    abstraction* mccfr_last_card_ab = new hand_last_card_abstraction(tmp_ptr);


    meta_abstraction meta_gr = meta_abstraction(tmp_ptr, seed);
    meta_gr.add_policy(greedy_pol, greedy_ab->get_information_abstraction(), greedy_ab->get_action_abstraction());
    meta_gr.add_policy(rand_pol, rand_ab->get_information_abstraction(), rand_ab->get_action_abstraction());

    run_name = "v2" + std::to_string(MAX_NUM_DECKCARDS) + "_card_deck_" + mccfr_sum_ab->get_name() + "_vs_" + meta_gr.get_name() + std::to_string(num_iter);

    status = train(num_iter, seed, mccfr_sum_ab, run_name, nullptr, &meta_gr);
    filenames.push_back(run_name+ ".bin");
    abs.push_back(mccfr_sum_ab);

    meta_abstraction meta_gm = meta_abstraction(tmp_ptr, seed);
    meta_gm.add_policy(greedy_pol, greedy_ab->get_information_abstraction(), greedy_ab->get_action_abstraction());
    meta_gm.add_policy(mccfr_new_stack, mccfr_new_stack_ab->get_information_abstraction(), mccfr_new_stack_ab->get_action_abstraction());

    run_name = "v2" + std::to_string(MAX_NUM_DECKCARDS) + "_card_deck_" + mccfr_sum_ab->get_name() + "_vs_" + meta_gm.get_name() + std::to_string(num_iter);

    status = train(num_iter, seed, mccfr_sum_ab, run_name, nullptr, &meta_gm);
    filenames.push_back(run_name+ ".bin");
    abs.push_back(mccfr_sum_ab);

    meta_abstraction meta_gmr = meta_abstraction(tmp_ptr, seed);
    meta_gmr.add_policy(greedy_pol, greedy_ab->get_information_abstraction(), greedy_ab->get_action_abstraction());
    meta_gmr.add_policy(mccfr_new_stack, mccfr_new_stack_ab->get_information_abstraction(), mccfr_new_stack_ab->get_action_abstraction());
    meta_gmr.add_policy(rand_pol, rand_ab->get_information_abstraction(), rand_ab->get_action_abstraction());

    run_name = "v2" + std::to_string(MAX_NUM_DECKCARDS) + "_card_deck_" + mccfr_sum_ab->get_name() + "_vs_" + meta_gmr.get_name() + std::to_string(num_iter);

    status = train(num_iter, seed, mccfr_sum_ab, run_name, nullptr, &meta_gmr);
    filenames.push_back(run_name+ ".bin");
    abs.push_back(mccfr_sum_ab);

    meta_abstraction meta_dgmr = meta_abstraction(tmp_ptr, seed);
    meta_dgmr.add_policy(determ_low, determ_low_ab->get_information_abstraction(), determ_low_ab->get_action_abstraction());
    meta_dgmr.add_policy(greedy_pol, greedy_ab->get_information_abstraction(), greedy_ab->get_action_abstraction());
    meta_dgmr.add_policy(mccfr_new_stack, mccfr_new_stack_ab->get_information_abstraction(), mccfr_new_stack_ab->get_action_abstraction());
    meta_dgmr.add_policy(rand_pol, rand_ab->get_information_abstraction(), rand_ab->get_action_abstraction());

    run_name = "v2" + std::to_string(MAX_NUM_DECKCARDS) + "_card_deck_" + mccfr_sum_ab->get_name() + "_vs_" + meta_dgmr.get_name() + std::to_string(num_iter);

    status = train(num_iter, seed, mccfr_sum_ab, run_name, nullptr, &meta_dgmr);
    filenames.push_back(run_name+ ".bin");
    abs.push_back(mccfr_sum_ab);

    meta_abstraction meta_m3 = meta_abstraction(tmp_ptr, seed);
    meta_m3.add_policy(mccfr_new_stack, mccfr_new_stack_ab->get_information_abstraction(), mccfr_new_stack_ab->get_action_abstraction());
    meta_m3.add_policy(mccfr_sum, mccfr_sum_ab->get_information_abstraction(), mccfr_sum_ab->get_action_abstraction());
    meta_m3.add_policy(mccfr_last_card, mccfr_last_card_ab->get_information_abstraction(), mccfr_last_card_ab->get_action_abstraction());
    meta_m3.set_name("meta_abstraction_mmm_107");
    run_name = "v2" + std::to_string(MAX_NUM_DECKCARDS) + "_card_deck_" + mccfr_sum_ab->get_name() + "_vs_" + meta_m3.get_name() + "_" + std::to_string(num_iter);

    status = train(num_iter, seed, mccfr_sum_ab, run_name, nullptr, &meta_m3);
    filenames.push_back(run_name+ ".bin");
    abs.push_back(mccfr_sum_ab);

    meta_abstraction meta_m2 = meta_abstraction(tmp_ptr, seed);
    meta_m2.add_policy(mccfr_new_stack, mccfr_new_stack_ab->get_information_abstraction(), mccfr_new_stack_ab->get_action_abstraction());
    meta_m2.add_policy(mccfr_sum, mccfr_sum_ab->get_information_abstraction(), mccfr_sum_ab->get_action_abstraction());

    run_name = "v2" + std::to_string(MAX_NUM_DECKCARDS) + "_card_deck_" + mccfr_sum_ab->get_name() + "_vs_" + meta_m2.get_name() + std::to_string(num_iter);

    status = train(num_iter, seed, mccfr_sum_ab, run_name, nullptr, &meta_m2);
    filenames.push_back(run_name+ ".bin");
    abs.push_back(mccfr_sum_ab);

    filenames.push_back("C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_108.bin");
    filenames.push_back("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_last_card_abstraction_10000000.bin");
    filenames.push_back("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_last_card_sum_abstraction_10000000.bin");
    filenames.push_back("greedypolicy");
    filenames.push_back("randompolicy");
    filenames.push_back("determenisticpolicy");

    abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));
    abs.push_back(new hand_last_card_abstraction(tmp_ptr));
    abs.push_back(new hand_last_card_sum_abstraction(tmp_ptr));
    abs.push_back(new greedy_abstraction(tmp_ptr));
    abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));
    abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));


    if (!logger.is_open()) {
        throw runtime_error("logger has crashed");
    }

    status = round_robin_test(1'000'000, filenames, abs, seed, &logger);
    status = qualitative_hand_test(filenames, abs, seed, &logger);

    delete rand_pol;
    delete determ_low;
    delete determ_high;
    delete greedy_pol;
    delete mccfr_new_stack;
    delete mccfr_sum;
    delete mccfr_last_card;
    
    delete rand_ab;
    delete determ_low_ab;
    delete determ_high_ab;
    delete greedy_ab;
    delete mccfr_new_stack_ab;
    delete mccfr_sum_ab;
    delete mccfr_last_card_ab;

    return status;

}



int test_growing_generating_policies(policy* start_pol, abstraction* start_ab) {
    int seed = 22;
    int status = 0;
    string run_name;
    vector<string> filenames = vector<string>();
    vector<abstraction*> abs = vector<abstraction*>();
    vector<policy*> component_policies = vector<policy*>();

    simulator::cribbage tmp_game = simulator::cribbage();
    shared_ptr<simulator::cribbage> tmp_ptr = make_shared<simulator::cribbage>(tmp_game);

    int num_iter = 10'000'000;
    const int n = 5;

    hand_last_card_sum_abstraction agent_ab = hand_last_card_sum_abstraction(tmp_ptr);

    meta_abstraction meta = meta_abstraction(tmp_ptr, 1);

    component_policies.push_back(start_pol);
    meta.add_policy(start_pol, start_ab->get_information_abstraction(), start_ab->get_action_abstraction());

    for(int i=1; i<n; i++) {
        run_name = std::to_string(MAX_NUM_DECKCARDS) + "_card_deck_start_"+ start_pol->name + "_" + agent_ab.get_name() + "_vs_" + "meta_num" + std::to_string(i) + "_iter" + std::to_string(num_iter) + "fixed";

        status = train(num_iter, seed, &agent_ab, run_name, nullptr, &meta);
        filenames.push_back(run_name+ ".bin");
        abs.push_back(&agent_ab);

        policy* trained_policy = new policy();
        trained_policy->deserialize(run_name+".bin");

        meta.add_policy(trained_policy, agent_ab.get_information_abstraction(), agent_ab.get_action_abstraction());
        component_policies.push_back(trained_policy);
    }

    filenames.push_back("C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_108.bin");
    filenames.push_back("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_last_card_sum_abstraction_10000000.bin");
    filenames.push_back("greedypolicy");
    filenames.push_back("randompolicy");
    filenames.push_back("determenisticpolicy");

    abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));
    abs.push_back(new hand_last_card_sum_abstraction(tmp_ptr));
    abs.push_back(new greedy_abstraction(tmp_ptr));
    abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));
    abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));

    ofstream logger("growing_generated_polies_round_robin_and_qualitative_hand_analysis"+ start_pol->name + "v2.txt");
    
    if (!logger.is_open()) {
        throw runtime_error("logger has crashed");
    }

    status = round_robin_test(1'000'000, filenames, abs, seed, &logger);
    status = qualitative_hand_test(filenames, abs, seed, &logger);

    for(int i=1; i<n; i++) {
        delete component_policies[i];
    }

    for(int i=abs.size()-5; i<abs.size(); i++) {
        delete abs[i];
    }

    return 0;
}


int test_growing_best_response(policy* start_pol, abstraction* start_ab) {
    int seed = 22;
    int status = 0;
    string run_name;
    vector<string> filenames = vector<string>();
    vector<abstraction*> abs = vector<abstraction*>();
    vector<policy*> component_policies = vector<policy*>();

    simulator::cribbage tmp_game = simulator::cribbage();
    shared_ptr<simulator::cribbage> tmp_ptr = make_shared<simulator::cribbage>(tmp_game);

    int num_iter = 10'000'000;
    const int n = 5;

    hand_last_card_sum_abstraction agent_ab = hand_last_card_sum_abstraction(tmp_ptr);



    meta_abstraction meta = meta_abstraction(tmp_ptr, 1);

    component_policies.push_back(start_pol);
    meta.add_policy(start_pol, start_ab->get_information_abstraction(), start_ab->get_action_abstraction());

    policy* last_pol = start_pol;
    abstraction* last_ab = start_ab;

    for(int i=1; i<n; i++) {
        run_name = "BR" + std::to_string(MAX_NUM_DECKCARDS) + "_card_deck_start_" + start_pol->name + "_" + agent_ab.get_name() + "_vs_" + "previous_policy" + std::to_string(i) + "_iter" + std::to_string(num_iter);

        status = train(num_iter, seed, &agent_ab, run_name, last_pol, last_ab);
        filenames.push_back(run_name+ ".bin");
        abs.push_back(&agent_ab);

        policy* trained_policy = new policy();
        trained_policy->deserialize(run_name+".bin");

        meta.add_policy(trained_policy, agent_ab.get_information_abstraction(), agent_ab.get_action_abstraction());
        component_policies.push_back(trained_policy);
        last_pol = trained_policy;
        last_ab = &agent_ab;

        run_name = "BR_v2" + std::to_string(MAX_NUM_DECKCARDS) + "_card_deck_start_" + start_pol->name + "_" + agent_ab.get_name() + "_vs_" + "meta_snapshot" + std::to_string(i) + "_iter" + std::to_string(num_iter);

        status = train(num_iter, seed, &agent_ab, run_name, nullptr, &meta);
        filenames.push_back(run_name+ ".bin");
        abs.push_back(&agent_ab);
    }


    filenames.push_back("C:/Users/tor-d/git/cribbageAI/precomputed_policies/mccfr_policy_108.bin");
    filenames.push_back("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_last_card_sum_abstraction_10000000.bin");
    filenames.push_back("greedypolicy");
    filenames.push_back("randompolicy");
    filenames.push_back("determenisticpolicy");

    abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));
    abs.push_back(new hand_last_card_sum_abstraction(tmp_ptr));
    abs.push_back(new greedy_abstraction(tmp_ptr));
    abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));
    abs.push_back(new new_stack_4_choose_1_abstraction(tmp_ptr));

    ofstream logger("BR_polies_round_robin_and_qualitative_hand_analysis"+ start_pol->name +"v2.txt");
    
    if (!logger.is_open()) {
        throw runtime_error("logger has crashed");
    }

    status = round_robin_test(1'000'000, filenames, abs, seed, &logger);
    status = qualitative_hand_test(filenames, abs, seed, &logger);

    for(int i=1; i<n; i++) {
        delete component_policies[i];
    }

    for(int i=abs.size()-5; i<abs.size(); i++) {
        delete abs[i];
    }

    return 0;
}

int test_different_starting_points() {
    
    simulator::cribbage tmp_game = simulator::cribbage();
    shared_ptr<simulator::cribbage> tmp_ptr = make_shared<simulator::cribbage>(tmp_game);
    
    greedy_abstraction greedy_ab = greedy_abstraction(tmp_ptr);
    greedypolicy greedy_pol = greedypolicy();

    policy mccfr_pol = policy();
    mccfr_pol.deserialize("C:/Users/tor-d/git/cribbageAI/build/Debug/52_card_deck_mccfr_policy_hand_last_card_sum_abstraction_10000000.bin");
    mccfr_pol.name = "52_card_deck_mccfr_policy_hand_last_card_sum_abstraction_10000000";
    hand_last_card_sum_abstraction mccfr_ab = hand_last_card_sum_abstraction(tmp_ptr);

    randompolicy random_pol = randompolicy();
    new_stack_4_choose_1_abstraction random_ab = new_stack_4_choose_1_abstraction(tmp_ptr);


    test_growing_best_response(&greedy_pol, &greedy_ab);
    test_growing_best_response(&mccfr_pol, &mccfr_ab);
    test_growing_best_response(&random_pol, &random_ab);

    test_growing_generating_policies(&greedy_pol, &greedy_ab);
    test_growing_generating_policies(&mccfr_pol, &mccfr_ab);
    test_growing_generating_policies(&random_pol, &random_ab);

    return 0;
}