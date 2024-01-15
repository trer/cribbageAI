import queue
import time
from tkinter import Button, Canvas, Frame, Label, OptionMenu, StringVar, Tk
from PIL import ImageTk, Image
import threading

from pyController import Game


class App(Tk):
    def __init__(self, *args, **kwargs):
        Tk.__init__(self, *args, **kwargs)
        self.game_object = object

        container = Frame(self)
        container.pack(side="top", fill="both", expand=True)
        container.grid_rowconfigure(0, weight=1)
        container.grid_columnconfigure(0, weight=1)

        self.frames = {}

        list_of_frames = [StartPage, GamePage]

        for F in list_of_frames:
            frame = F(container, self)
            self.frames[F] = frame
            frame.grid(row=0, column=0, sticky="nsew")

        self.fresh = True
        self.protocol("WM_DELETE_WINDOW", self.on_closing)
        self.show_frame(StartPage)

    def show_frame(self, context):
        frame = self.frames[context]
        print("waiting")
        if not self.fresh:
            time.sleep(0.1)
            frame.update(game_info_q.get())
        self.fresh = False
        frame.tkraise()
    
    def on_closing(self):
        global stop_threads
        stop_threads = True
        game_event.set()
        self.destroy()




class StartPage(Frame):
    def __init__(self, parent, controller):
        Frame.__init__(self, parent)

        self.player_types = {
            "Human player": 'h',
            "Random player": 'r'
        }


        height = 500
        width = 800
        canvas = Canvas(self, height=height, width=width, bg="light green")
        canvas.pack()

        left_frame = Frame(canvas, bg='green', bd=5)
        left_frame.place(relx=0, rely=0, relwidth=0.5, relheight=1, anchor='nw')
        name_frame = Frame(left_frame, bg="light green", bd=5)
        name_frame.place(relx=0.5, rely=0.17, relwidth=0.9, relheight=0.7, anchor="n")

        self.clicked_p1 = StringVar()
        self.clicked_p1.set(next(iter(self.player_types.keys())))
        self.drop_p1 = OptionMenu(name_frame , self.clicked_p1 , *self.player_types.keys()) 
        self.drop_p1.place(relwidth=0.5, relheight=0.2)

        self.clicked_p2 = StringVar()
        self.clicked_p2.set(next(iter(self.player_types.keys())))
        self.drop_p2 = OptionMenu(name_frame , self.clicked_p2 , *self.player_types.keys()) 
        self.drop_p2.place(relx=0.5, rely=0, relwidth=0.5, relheight=0.2)

        enter_player_label = Label(left_frame, text="Player Names:", font=("Courier", 12), bd=3)
        enter_player_label.place(relx=0.25, rely=0.07, relwidth=0.5, relheight=0.05)

        right_frame = Frame(canvas, bg='green', bd=5)
        right_frame.place(relx=1, rely=0, relwidth=0.5, relheight=1, anchor='ne')

        button = Button(right_frame, text="START", font=("Courier", 12),
                        command=lambda: self.button_click(self.clicked_p1.get(), self.clicked_p2.get(), controller))
        button.place(relx=0.5, rely=0.9, relwidth=0.3, relheight=0.1, anchor="n")

    def button_click(self, entry0, entry1, controller):

        player_entry_list = [entry0, entry1]
        player_entry_list = [self.player_types[i] for i in player_entry_list]
        if len(player_entry_list) < 2:
            print("not enough players")
            return

        setup = {
            "players": player_entry_list,
        }
        response_q.put(setup)
        game_event.set()
        controller.show_frame(GamePage)

class GamePage(Frame):
    def __init__(self, parent, controller):
        Frame.__init__(self, parent)

        self.restart = True
        self.p1_cards = [0]*6 # max 6
        self.p2_cards = [0]*6 # max 6
        self.crib = [0]*4 # max 4
        self.played_cards = [0]*12 #max len 12

        

        height = 500
        width = 800
        canvas = Canvas(self, height=height, width=width, bg="light green")
        canvas.pack()

        # self.entry.bind("<Return>", lambda _: self.button_click(self.entry.get()))

        lower_frame = Frame(canvas, bg='green', bd=5)
        lower_frame.place(relx=1, rely=1, relwidth=1, relheight=0.2, anchor='se')

        self.p1_cards_frame = Frame(lower_frame, bd=2, relief="raised")
        self.p1_cards_frame.place(relx=0, rely=0, relwidth=1, relheight=1)

        self.p1_cards_labels = []
        len_per_card = 1 / len(self.p1_cards)
        for i in range(len(self.p1_cards)):
            tmp = Label(self.p1_cards_frame, bg="green", name="")
            tmp.name = "not_set"
            tmp.place(relx= (0 + len_per_card * i), relwidth=(len_per_card), relheight=1)
            card_d1 = ImageTk.PhotoImage(
            Image.open("GUI/card_images/default.png").resize((55, 85), Image.LANCZOS))
            tmp.image = card_d1
            tmp.configure(image=card_d1)
            self.p1_cards_labels.append(tmp)
        
        upper_frame = Frame(canvas, bg='green', bd=5)
        upper_frame.place(relx=1, rely=0, relwidth=1, relheight=0.2, anchor='ne')
        
        self.p2_cards_frame = Frame(upper_frame, bd=2, relief="raised")
        self.p2_cards_frame.place(relx=0, rely=0, relwidth=1, relheight=1)

        self.p2_cards_labels = []
        
        len_per_card = 1 / len(self.p2_cards)
        for i in range(len(self.p2_cards)):
            tmp = Label(self.p2_cards_frame, bg="green", name="")
            tmp.name = "not_set"
            tmp.place(relx= (0 + len_per_card * i), relwidth=(len_per_card), relheight=1)
            card_d1 = ImageTk.PhotoImage(
            Image.open("GUI/card_images/default.png").resize((55, 85), Image.LANCZOS))
            tmp.image = card_d1
            tmp.configure(image=card_d1)
            self.p2_cards_labels.append(tmp)

        self.played_cards_frame = Frame(canvas, bg="light green")
        self.played_cards_frame.place(relx=0.4, rely=0.5-0.1, relwidth=0.6, relheight=.2)
        self.played_card_labels = []

        len_per_card = 1 / len(self.played_cards)
        for i in range(len(self.played_cards)):
            tmp = Label(self.played_cards_frame, bg="light green", name="")
            tmp.place(relx= (0 + len_per_card * i/2), relwidth=55/480, relheight=1)
            self.played_card_labels.append(tmp)


        self.cut_card_frame = Frame(canvas, bg="light green")
        self.cut_card_frame.place(relx=0.3, rely=0.5-0.1, relwidth=55/800, relheight=.2)
        self.cut_card_label = Label(self.cut_card_frame, bg="light green", name="")
        self.cut_card_label.place(relx=0, relwidth=1, relheight=1)
        card_d1 = ImageTk.PhotoImage(
        Image.open("GUI/card_images/default.png").resize((55, 85), Image.LANCZOS))
        self.cut_card_label.image = card_d1
        self.cut_card_label.configure(image=card_d1)


        self.crib_frame = Frame(canvas, bg="light green")
        self.crib_frame.place(relx=0.05, rely=0.5-0.25, relwidth=3*55/800, relheight=.4, anchor="nw")
        self.crib_labels = []

        x_per_card = 2/3
        y_per_card = 3/5
        for i in range(len(self.crib)):
            tmp = Label(self.crib_frame, bg="light green", name="")
            tmp.place(relx=(0 + x_per_card *(i%2)), rely=(0+y_per_card*(i//2)), relwidth=1/3, relheight=2/5)
            card_d1 = ImageTk.PhotoImage(
            Image.open("GUI/card_images/default.png").resize((55, 85), Image.LANCZOS))
            tmp.image = card_d1
            tmp.configure(image=card_d1)
            self.crib_labels.append(tmp)
        
        self.crib_label = Label(self.crib_frame, text="CRIB", font=("Courier", 12), bd=3)
        self.crib_label.place(relx=self.crib_frame.winfo_width()/2, rely=self.crib_frame.winfo_height()/2, relwidth=.5, relheight=0.06, anchor="center")




        self.p1_score_label = Label(canvas, text="Points: 000", font=("Courier", 12), bd=3)
        self.p1_score_label.place(relx=0.75, rely=0.7, relwidth=.25, relheight=0.03)
        self.p1_status = Label(canvas, text="Dealer", font=("Courier", 12), bd=3)
        self.p1_status.place(relx=0.75, rely=0.73, relwidth=.25, relheight=0.03)

        self.p2_score_label = Label(canvas, text="Points: 000", font=("Courier", 12), bd=3)
        self.p2_score_label.place(relx=0.75, rely=0.3, relwidth=.25, relheight=0.03)
        self.p2_status = Label(canvas, text="Pone", font=("Courier", 12), bd=3)
        self.p2_status.place(relx=0.75, rely=0.27, relwidth=.25, relheight=0.03)

        self.next_round_label = Label(canvas, text="Click here for next round!", font=("Courier", 12), bd=3)
        self.next_round_label.text = "Click here for next round!"

        #self.event_update()

    def update(self, game):
        game_state = game.get_game_state()

        if self.restart:
            # set everything back to starting postitions
            for card in self.p1_cards_labels:
                tmp = ImageTk.PhotoImage(Image.open("GUI/card_images/default.png").resize((55, 85), Image.LANCZOS))
                card.image = tmp
                card.configure(image=tmp)
            for card in self.p2_cards_labels:
                tmp = ImageTk.PhotoImage(Image.open("GUI/card_images/default.png").resize((55, 85), Image.LANCZOS))
                card.image = tmp
                card.configure(image=tmp)
    
            for card in self.played_card_labels:
                card.configure(image="")
                #card.place_forget() #make invisible
            for card in self.crib_labels:
                card.configure(image="")
                #card.place_forget() #make invisible
            
            card_d1 = ImageTk.PhotoImage(
            Image.open("GUI/card_images/default.png").resize((55, 85), Image.LANCZOS))
            self.cut_card_label.image = card_d1
            self.cut_card_label.configure(image=card_d1)

            self.play_phase_already_setup = False
            self.discard_phase_already_setup = False
            self.matching_phase_already_setup = False
            self.num_placed_in_crib = 0
            self.num_placed_in_play = 0
            
            # Game restart
            game.p1_action_set = False
            game.p2_action_set = False
            self.restart = False
        
        
        if game_state == "discard_phase":
            #show human players cards
            self.setup_discard_phase(game)
        

        if game_state == "play_phase":
            self.setup_play_phase(game)
        if len(game.cards_to_place_in_play) > 0:
            self.place_card_in_play(game.cards_to_place_in_play.pop(0), game)

        if game_state == "matching_phase":
            self.setup_matching_phase(game)
            if game.dealer_has_matched_points: #last flag to be set
                self.next_round_label.place(relx=0.5, rely=0.5, relwidth=.25, relheight=0.03, anchor="center")
                self.next_round_label.lift()
                self.next_round_label.configure(text=self.next_round_label.text)
                self.next_round_label.bind("<Button-1>", lambda e: self.setup_next_round(game))
        
        if game.get_game_state() == "game_over":
            self.next_round_label.configure(text="Game is over! \n The winnner is: "+ str(game.winner) + " \n Click here to start next Game!")
            self.next_round_label.place(relx=0.5, rely=0.5, relwidth=.5, relheight=0.12, anchor="center")
            self.next_round_label.lift()
            self.next_round_label.bind("<Button-1>", lambda e: self.setup_next_game(game))
     

        self.p1_score_label.text = "Points: " + str(game.get_points(1))
        self.p1_score_label.configure(text = self.p1_score_label.text)
        self.p2_score_label.text = "Points: " + str(game.get_points(2))
        self.p2_score_label.configure(text = self.p2_score_label.text)   

        self.p1_status.text = game.get_player_status(1)
        self.p1_status.configure(text = self.p1_status.text)
        self.p2_status.text = game.get_player_status(2)
        self.p2_status.configure(text = self.p2_status.text)

        if not game_info_q.empty():
            self.after(20, self.update, game_info_q.get())
        else:
            self.check_do_update()

    def check_do_update(self):
        global stop_threads
        if do_update.is_set():
            do_update.clear()
            print("the queue is empty", game_info_q.empty())
            
            if not game_info_q.empty():
                self.after(20, self.update, game_info_q.get())
            else:
                self.after(200, self.check_do_update)
        elif not stop_threads:
            self.after(200, self.check_do_update)


    def setup_matching_phase(self, game):
        if self.matching_phase_already_setup:
            pass
        else:
            self.reveal_crib(game)
            self.reveal_player_cards(game)
            
            
            self.matching_phase_already_setup = True

    
    def setup_play_phase(self, game):
        if self.play_phase_already_setup:
            pass
        else:

            for card in game.cards_to_place_in_crib:
                self.place_card_in_crib(card)

            

            cut_card_name = game.get_cut_card()
            tmp = ImageTk.PhotoImage(Image.open("GUI/card_images/card_"+cut_card_name+".png").resize((55, 85), Image.LANCZOS))
            self.cut_card_label.image = tmp
            self.cut_card_label.configure(image=tmp)
            #potentially do some changes to bind

            self.play_phase_already_setup = True         

    def place_card_in_play(self, card_name, game):
        if card_name == None:
            self.num_placed_in_play += 2
            game.new_stack = False
        else:
            play_card = self.played_card_labels[self.num_placed_in_play]
            play_card.name = card_name
            tmp = ImageTk.PhotoImage(Image.open("GUI/card_images/card_"+card_name+".png").resize((55, 85), Image.LANCZOS))
            play_card.image = tmp
            play_card.configure(image=tmp)
            for card in self.p1_cards_labels + self.p2_cards_labels:
                if card.name == card_name:
                    card.configure(image="")
            self.num_placed_in_play += 1
                    
    def place_card_in_crib(self, card):
        # card.place_forget() #remove card from hand
        for c in self.p1_cards_labels + self.p2_cards_labels:
            if c.name == card:
                c.configure(image="")
        crib_card = self.crib_labels[self.num_placed_in_crib]
        tmp = ImageTk.PhotoImage(Image.open("GUI/card_images/default.png").resize((55, 85), Image.LANCZOS))
        crib_card.image = tmp
        crib_card.configure(image=tmp)
        self.num_placed_in_crib += 1

    def reveal_crib(self, game):
        crib_card_names = game.get_cards(-1)
        for name, card in zip(crib_card_names, self.crib_labels):
            card.name = name
            tmp = ImageTk.PhotoImage(Image.open("GUI/card_images/card_"+name+".png").resize((55, 85), Image.LANCZOS))
            card.image = tmp
            card.configure(image=tmp)
    
    def reveal_player_cards(self, game):
        player_card_names = game.get_cards(1)[:4] + game.get_cards(2)[:4]
        for name, card in zip(player_card_names, self.p1_cards_labels[:4]+self.p2_cards_labels[:4]):
                # if card.name not in [c.name for c in self.crib_labels] and card.name != "not_set":
                tmp = ImageTk.PhotoImage(Image.open("GUI/card_images/card_"+name+".png").resize((55, 85), Image.LANCZOS))
                card.image = tmp
                card.configure(image=card.image)

    def setup_discard_phase(self, game):
        if self.discard_phase_already_setup:
            return
        
        card_names = game.get_cards(1)
        for i in range(len(card_names)):
            card = self.p1_cards_labels[i]
            name = card_names[i]
            card.name=name #name of card set such that we can compare later
            if game.p1 == 'h':
                tmp = ImageTk.PhotoImage(Image.open("GUI/card_images/card_"+name+".png").resize((55, 85), Image.LANCZOS))
                card.image = tmp
                card.configure(image=tmp)

        if game.p1 == 'h':    
            #didn't work inside the loop have no idea why :shrug:
            self.p1_cards_labels[0].bind("<Button-1>", lambda e:self.card_chosen(self.p1_cards_labels[0], game))
            self.p1_cards_labels[1].bind("<Button-1>", lambda e:self.card_chosen(self.p1_cards_labels[1], game))
            self.p1_cards_labels[2].bind("<Button-1>", lambda e:self.card_chosen(self.p1_cards_labels[2], game))
            self.p1_cards_labels[3].bind("<Button-1>", lambda e:self.card_chosen(self.p1_cards_labels[3], game))
            self.p1_cards_labels[4].bind("<Button-1>", lambda e:self.card_chosen(self.p1_cards_labels[4], game))
            self.p1_cards_labels[5].bind("<Button-1>", lambda e:self.card_chosen(self.p1_cards_labels[5], game))


        card_names = game.get_cards(2)
        for card, name in zip(self.p2_cards_labels, card_names):
            card.name=name #name of card set such that we can compare later
            if game.p2 == 'h':      
                tmp = ImageTk.PhotoImage(Image.open("GUI/card_images/card_"+name+".png").resize((55, 85), Image.LANCZOS))
                card.image = tmp
                card.configure(image=tmp)

        if game.p2 == 'h':    
            #didn't work inside the loop have no idea why :shrug:
            self.p2_cards_labels[0].bind("<Button-1>", lambda e:self.card_chosen(self.p2_cards_labels[0], game))
            self.p2_cards_labels[1].bind("<Button-1>", lambda e:self.card_chosen(self.p2_cards_labels[1], game))
            self.p2_cards_labels[2].bind("<Button-1>", lambda e:self.card_chosen(self.p2_cards_labels[2], game))
            self.p2_cards_labels[3].bind("<Button-1>", lambda e:self.card_chosen(self.p2_cards_labels[3], game))
            self.p2_cards_labels[4].bind("<Button-1>", lambda e:self.card_chosen(self.p2_cards_labels[4], game))
            self.p2_cards_labels[5].bind("<Button-1>", lambda e:self.card_chosen(self.p2_cards_labels[5], game))
        
        self.discard_phase_already_setup = True


    def card_chosen(self, card_label, game):
        #add some checks that it is indeed the correct players turn
        if card_label in self.p1_cards_labels:
            game.add_action(1, card_label.name)
        elif card_label in self.p2_cards_labels:
            game.add_action(2, card_label.name)
        self.action_input("card_chosen")
    
    def setup_next_round(self, game):
        # game.finnish_round()
        game.setup_round()
        self.restart = True
        self.next_round_label.place_forget()
        self.action_input("starting new round")
    
    def setup_next_game(self, game):
        game.setup_game()
        self.setup_next_round(game)
            

    def action_input(self, entry0):

        response_q.put(entry0)
        game_event.set()
        # self.after(2000) #sleep(0.2)
        # if not game_info_q.empty():
        #     self.update(game_info_q.get())
    

def play(game):
    action = game.do_automatic_action()
    print(action)
    if action != "":
        game_info_q.put(game)
        do_update.set()
     
    time.sleep(0.01)


def run_app():
    app = App()
    app.mainloop()

def run_game_data():
    global stop_threads
    game0 = Game(game_info_q, response_q, game_event)
    game0.setup_round()
    game_info_q.put(game0)
    while not stop_threads:
        play(game0)

stop_threads = False
game_event = threading.Event()
response_q = queue.Queue()
game_info_q = queue.Queue()
do_update = threading.Event()
t1 = threading.Thread(target=run_app)
t1.start()
t2 = threading.Thread(target=run_game_data)
t2.start()

t1.join()
exit()

if __name__ == "__main__":
    print("running")