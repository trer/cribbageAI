
import gui_bind

class Game():

    def __init__(self, game_info_q, response_q, game_event):
        self.acting_player = 1
        self.cribbage_simulator = gui_bind.controller()
        self.set_queues_and_events(game_info_q, response_q, game_event)
        self.setup = self.ask_app("Start Game?")

        self.p1 = self.setup["players"][0]
        self.p2 = self.setup["players"][1]
        self.cribbage_simulator.set_player(self.p1, 1)
        self.cribbage_simulator.set_player(self.p2, 2)

        self.setup_game()



    def get_game_state(self):
        return self.phase
    
    def do_automatic_action(self):
        answer = ""
        question = ""
        win = 0
        if self.phase == "discard_phase":
            if not self.p1_discard_done:
                if len(self.p1_action) == 2:
                    self.discard_cards(1, self.p1_action)
                    answer = "discard cards for p1"
                elif self.p1 != 'h':
                    self.p1_discard_done = True # not needed i think
                    self.cribbage_simulator.poll_player(1)
                    self.handle_discards() 
                    answer = "discard for p1 automaticly set"
                else:
                    question = "Player 1 must choose 2 cards to discard"
            if not self.p2_discard_done:
                if len(self.p2_action) == 2:
                    self.discard_cards(2, self.p2_action)
                    answer = "discard cards for p2"
                elif self.p2 != 'h':
                    self.p2_discard_done = True # not needed i think
                    self.cribbage_simulator.poll_player(2)
                    self.handle_discards() 
                    answer = "discard for p2 automaticly set"
                else:
                    if question == "":
                        question = "Player 2 must choose 2 cards to discard"

        elif self.phase == "play_phase":
            if self.cribbage_simulator.get_num_cards(1) == 0 and self.cribbage_simulator.get_num_cards(2) == 0:
                self.clear_actions() #remove discard actions
                self.phase = "matching_phase"
                answer = "moving to matching phase"
            elif not self.cribbage_simulator.has_called_go(1) and not self.cribbage_simulator.has_called_go(2) and self.go_called:
                self.new_stack = True
                self.cards_to_place_in_play.append(None)
                self.go_called = False
                answer = "new stack"

            elif self.cribbage_simulator.get_current_player() == 1:
                if not self.cribbage_simulator.has_legal_move(1):
                    self.call_go_action(1)
                    self.play_card(1, self.p1_action) #empty list
                    answer = "p1_call_go"
                elif len(self.p1_action) == 1:
                    self.cards_to_place_in_play.append(self.p1_action[0])
                    self.play_card(1, self.p1_action)
                    answer = "player 1 playing card"
                elif self.p1 != 'h':
                    out_string = self.cribbage_simulator.poll_player(1)
                    out_string = out_string.split(" ")
                    win = int(out_string[-1])
                    self.cards_to_place_in_play.append(out_string[0])
                    self.clear_actions()
                    self.p1_action_done = True
                    answer = "player 1 automatic play"
                else:
                    question = "Player 1 must choose 1 card to play"
                

            elif self.cribbage_simulator.get_current_player() == 2:
                if not self.cribbage_simulator.has_legal_move(2):
                    self.call_go_action(2)
                    self.play_card(2, self.p2_action) #empty list
                    answer = "p2_call_go"
                elif len(self.p2_action) == 1:
                    self.cards_to_place_in_play.append(self.p2_action[0])
                    self.play_card(2, self.p2_action)
                    answer = "player 2 playing card"
                elif self.p2 != 'h':
                    out_string = self.cribbage_simulator.poll_player(2)
                    out_string = out_string.split(" ")
                    win = int(out_string[-1])
                    self.cards_to_place_in_play.append(out_string[0])
                    self.clear_actions()
                    self.p2_action_done = True
                    answer = "player 2 automatic play"
                else:
                    question = "Player 2 must choose 1 card to play"

        elif self.phase == "matching_phase":
            if not self.pone_has_matched_points:
                self.cribbage_simulator.matching_setup()
                win = self.cribbage_simulator.matching_score_pone()
                answer = "points given to pone"
                self.pone_has_matched_points = True
                
            elif not self.dealer_has_matched_points:
                win = self.cribbage_simulator.matching_score_dealer()
                answer = "points given to dealer"
                self.dealer_has_matched_points = True
                self.ready_for_next_round = True
        
        if answer == "":
            if question == "":
                question = "waiting on player input"
            answer = self.ask_app(question, game=self)

        if win != 0:
            self.game_cleanup(win)
        return answer

    
    def get_cards(self, player):
        cards = []
        num_cards = self.cribbage_simulator.get_num_cards(player)
        for i in range(num_cards):
            cards.append(self.cribbage_simulator.get_card(i, player))
        return cards

    def get_cut_card(self):
        return self.cribbage_simulator.get_cut_card()

    def clear_actions(self):
        self.p1_action = []
        self.p2_action = []
        self.p1_action_set = False
        self.p2_action_set = False
        self.p1_action_done = False
        self.p2_action_done = False

    def call_go_action(self, player):
        self.go_called = True
        if player == 1:
            self.p1_action_set = True
        elif player == 2:
            self.p2_action_set = True

    def add_action(self, player, card_name):
        if player == 1:
            if card_name in self.p1_action:
                self.p1_action.remove(card_name)
            else:
                self.p1_action.append(card_name)
                self.p1_action_set = True
        elif player == 2:
            if card_name in self.p2_action:
                self.p2_action.remove(card_name)
            else:
                self.p2_action.append(card_name)
                self.p2_action_set = True

    def discard_cards(self, player, action):
        cards = self.get_cards(player)
        card_indexes = [cards.index(card_name) for card_name in action]
        self.discard_cards_internal(player, card_indexes[0], card_indexes[1])
    
    def discard_cards_internal(self, player, index0, index1):
        self.cribbage_simulator.discard_action(player, index0, index1)

        handled_discards = self.handle_discards() 
                
        if player == 1:
            self.p1_discard_done = True
        elif player == 2:
            self.p2_discard_done = True
        return handled_discards
    
    def handle_discards(self):
        if self.cribbage_simulator.discard_set():
            win = self.cribbage_simulator.handle_discards()
            self.cards_to_place_in_crib = self.get_cards(-1)
            
            if win != 0:
                self.game_cleanup(win)
            else:
                self.phase = "play_phase"
                self.cribbage_simulator.setup_play_phase()
                self.clear_actions()
            return True
        return False

    def play_card(self, player, action):
        cards = self.get_cards(player)
        if len(action) > 0:
            card_indexes = [cards.index(card_name) for card_name in action]
        else:
            card_indexes = [-1]
        
        win = self.cribbage_simulator.play_action(player, card_indexes[0])
        self.clear_actions()
        if win != 0:
            self.game_cleanup(win)
        
        

    def game_cleanup(self, winner):
        self.phase = "game_over"
        self.winner = winner
        print("winner is player", winner)


    def setup_game(self):
        self.winner = 0
        self.restart = True
        self.phase = "discard_phase"
        self.p1_discard_done = False
        self.p2_discard_done = False
        self.p1_action_set = False
        self.p2_action_set = False
        self.p1_action_done = False
        self.p2_action_done = False
        self.p1_action = []
        self.p2_action = []
        self.new_stack = False
        self.cribbage_simulator.setup_game(same_pone=True)
        self.setup_round()
    
    def setup_round(self):
        self.cribbage_simulator.setup_round()
        self.clear_actions()
        self.cards_to_place_in_play = []
        self.cards_to_place_in_crib = []
        self.p1_discard_done = False
        self.p2_discard_done = False
        self.go_called = False
        self.ready_for_next_round = False
        self.dealer_has_matched_points = False
        self.pone_has_matched_points = False
        self.phase = "discard_phase"


    def get_current_player(self):
        return self.cribbage_simulator.get_current_player()
    
    def get_points(self, player):
        return self.cribbage_simulator.get_points(player)
    
    def get_player_status(self, player):
        current_pone = self.cribbage_simulator.get_current_pone()
        if current_pone == player:
            return "Pone"
        else:
            return "Dealer"
        
    def set_queues_and_events(self, game_info_q, response_q, game_event):
        self.game_info_q = game_info_q
        self.response_q = response_q
        self.game_event = game_event  
        
    def ask_app(self, question, game=""):
        print("asking...")
        print(question)
        answer = ""
        if game != "":
            self.game_info_q.put(game)
        self.game_event.wait()
        if not self.response_q.empty():
            answer = self.response_q.get()
        self.game_event.clear()

        return answer