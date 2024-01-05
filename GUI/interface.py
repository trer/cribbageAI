import os
import queue
import time
from tkinter import Button, Canvas, Entry, Frame, Label, Tk
from PIL import ImageTk, Image
import threading

class Game():

    def __init__(self):
        pass



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
        self.show_frame(StartPage)

    def show_frame(self, context):
        frame = self.frames[context]
        print("waiting")
        if not self.fresh:
            time.sleep(0.1)
            frame.update(game_info_q.get())
        self.fresh = False
        frame.tkraise()

class StartPage(Frame):
    def __init__(self, parent, controller):
        Frame.__init__(self, parent)

        height = 500
        width = 800
        canvas = Canvas(self, height=height, width=width, bg="light green")
        canvas.pack()

        left_frame = Frame(canvas, bg='green', bd=5)
        left_frame.place(relx=0, rely=0, relwidth=0.5, relheight=1, anchor='nw')
        name_frame = Frame(left_frame, bg="light green", bd=5)
        name_frame.place(relx=0.5, rely=0.17, relwidth=0.9, relheight=0.7, anchor="n")
        self.entry_p0 = Entry(name_frame, font=("Courier", 12), bd=3)
        self.entry_p0.place(relwidth=0.5, relheight=0.2)
        self.entry_p1 = Entry(name_frame, font=("Courier", 12), bd=3)
        self.entry_p1.place(relx=0.5, rely=0, relwidth=0.5, relheight=0.2)

        enter_player_label = Label(left_frame, text="Player Names:", font=("Courier", 12), bd=3)
        enter_player_label.place(relx=0.25, rely=0.07, relwidth=0.5, relheight=0.05)
        # self.entry.bind("<Return>", lambda _: self.button_click(self.entry.get()))

        right_frame = Frame(canvas, bg='green', bd=5)
        right_frame.place(relx=1, rely=0, relwidth=0.5, relheight=1, anchor='ne')

        button = Button(right_frame, text="START", font=("Courier", 12),
                        command=lambda: self.button_click(self.entry_p0.get(), self.entry_p1.get(), controller))
        button.place(relx=0.5, rely=0.9, relwidth=0.3, relheight=0.1, anchor="n")

    def button_click(self, entry0, entry1, controller):
        entry_list = [entry0, entry1]
        player_entry_list = [entry0, entry1]
        print(player_entry_list)
        player_entry_list = list(set(player_entry_list))
        for player in player_entry_list:
            if player == "":
                player_entry_list.remove(player)
        print(player_entry_list)
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

        self.restart = False
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
            tmp = Label(self.p1_cards_frame, bg="green")
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
            tmp = Label(self.p2_cards_frame, bg="green")
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
            tmp = Label(self.played_cards_frame, bg="light green")
            tmp.place(relx= (0 + len_per_card * i/2), relwidth=55/480, relheight=1)
            card_d1 = ImageTk.PhotoImage(
            Image.open("GUI/card_images/card_ST.png").resize((55, 85), Image.LANCZOS))
            tmp.image = card_d1
            tmp.configure(image=card_d1)
            self.played_cards.append(tmp)


        self.cut_card_frame = Frame(canvas, bg="light green")
        self.cut_card_frame.place(relx=0.3, rely=0.5-0.1, relwidth=55/800, relheight=.2)
        self.cut_card_label = Label(self.cut_card_frame, bg="light green")
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
            tmp = Label(self.crib_frame, bg="light green")
            tmp.place(relx=(0 + x_per_card *(i%2)), rely=(0+y_per_card*(i//2)), relwidth=1/3, relheight=2/5)
            card_d1 = ImageTk.PhotoImage(
            Image.open("GUI/card_images/card_ST.png").resize((55, 85), Image.LANCZOS))
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


    def update(self, game):

        if self.restart:
            card1 = ImageTk.PhotoImage(Image.open(str("GUI/cards/default.png")).resize((55, 85), Image.LANCZOS))
            self.cc_1.image = card1
            self.cc_1.configure(image=card1)

            card1 = ImageTk.PhotoImage(Image.open(str("GUI/cards/default.png")).resize((55, 85), Image.LANCZOS))
            self.cc_2.image = card1
            self.cc_2.configure(image=card1)

            card1 = ImageTk.PhotoImage(Image.open(str("GUI/cards/default.png")).resize((55, 85), Image.LANCZOS))
            self.cc_3.image = card1
            self.cc_3.configure(image=card1)

            card1 = ImageTk.PhotoImage(Image.open(str("GUI/cards/default.png")).resize((55, 85), Image.LANCZOS))
            self.cc_4.image = card1
            self.cc_4.configure(image=card1)

            card1 = ImageTk.PhotoImage(Image.open(str("GUI/cards/default.png")).resize((55, 85), Image.LANCZOS))
            self.cc_5.image = card1
            self.cc_5.configure(image=card1)
            self.restart = False
        """
        if game.round_ended:
            time.sleep(0.3)
            self.new_round_label.lift(self.action_cover_label)
            self.button_y.lift(self.action_cover_label)
            self.button_n.lift(self.action_cover_label)
            winners = []
            scores = []
            for player in game.list_of_players_not_out:
                if player.win:
                    winners.append(player)
                    scores.append(player.score)
            print(f"gui thinks winners are: {winners}")
            print(f"and thinks scores are: {scores}")
            if scores == [[]]:
                self.winner_label["text"] = "Winner: " + str(winners)
            else:
                try:
                    for player in game.list_of_players_not_out:
                        if player.win:
                            if player.score == max(scores):
                                self.winner_label["text"] = "Winner: " + str(winners) + "\n" + score_interpreter(player)
                except IndexError:
                    pass
            self.winner_label.lift(self.action_cover_label)

            self.restart = True

            return
        """
        """
        if game.need_raise_info:
            self.raise_entry.lift(self.action_cover_label)
            self.raise_button.lift(self.action_cover_label)
        """
        """
        try:
            card1 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.cards[0]) + ".png").resize((55, 85), Image.LANCZOS))
            self.cc_1.image = card1
            self.cc_1.configure(image=card1)

            card1 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.cards[1]) + ".png").resize((55, 85), Image.LANCZOS))
            self.cc_2.image = card1
            self.cc_2.configure(image=card1)

            card1 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.cards[2]) + ".png").resize((55, 85), Image.LANCZOS))
            self.cc_3.image = card1
            self.cc_3.configure(image=card1)

            card1 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.cards[3]) + ".png").resize((55, 85), Image.LANCZOS))
            self.cc_4.image = card1
            self.cc_4.configure(image=card1)

            card1 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.cards[4]) + ".png").resize((55, 85), Image.LANCZOS))
            self.cc_5.image = card1
            self.cc_5.configure(image=card1)
        except IndexError:
            pass
        try:
            self.name_label_p0["text"] = game.list_of_players[0]
            self.name_label_p1["text"] = game.list_of_players[1]
            self.name_label_p2["text"] = game.list_of_players[2]
            self.name_label_p3["text"] = game.list_of_players[3]
            self.name_label_p4["text"] = game.list_of_players[4]
            self.name_label_p5["text"] = game.list_of_players[5]
            self.name_label_p6["text"] = game.list_of_players[6]
            self.name_label_p7["text"] = game.list_of_players[7]
            self.name_label_p8["text"] = game.list_of_players[8]
            self.name_label_p9["text"] = game.list_of_players[9]
        except IndexError:
            pass
        try:
            self.chips_label_p0["text"] = "Chips:\n" + str(game.list_of_players[0].chips)
            self.chips_label_p1["text"] = "Chips:\n" + str(game.list_of_players[1].chips)
            self.chips_label_p2["text"] = "Chips:\n" + str(game.list_of_players[2].chips)
            self.chips_label_p3["text"] = "Chips:\n" + str(game.list_of_players[3].chips)
            self.chips_label_p4["text"] = "Chips:\n" + str(game.list_of_players[4].chips)
            self.chips_label_p5["text"] = "Chips:\n" + str(game.list_of_players[5].chips)
            self.chips_label_p6["text"] = "Chips:\n" + str(game.list_of_players[6].chips)
            self.chips_label_p7["text"] = "Chips:\n" + str(game.list_of_players[7].chips)
            self.chips_label_p8["text"] = "Chips:\n" + str(game.list_of_players[8].chips)
            self.chips_label_p9["text"] = "Chips:\n" + str(game.list_of_players[9].chips)
        except IndexError:
            pass
        try:
            card1 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.list_of_players[0].cards[0]) + ".png").resize((55, 85), Image.LANCZOS))
            self.card1_p0.image = card1
            self.card1_p0.configure(image=card1)

            card1 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.list_of_players[1].cards[0]) + ".png").resize((55, 85), Image.LANCZOS))
            self.card1_p1.image = card1
            self.card1_p1.configure(image=card1)

            card1 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.list_of_players[2].cards[0]) + ".png").resize((55, 85), Image.LANCZOS))
            self.card1_p2.image = card1
            self.card1_p2.configure(image=card1)

            card1 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.list_of_players[3].cards[0]) + ".png").resize((55, 85), Image.LANCZOS))
            self.card1_p3.image = card1
            self.card1_p3.configure(image=card1)

            card1 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.list_of_players[4].cards[0]) + ".png").resize((55, 85), Image.LANCZOS))
            self.card1_p4.image = card1
            self.card1_p4.configure(image=card1)

            card1 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.list_of_players[5].cards[0]) + ".png").resize((55, 85), Image.LANCZOS))
            self.card1_p5.image = card1
            self.card1_p5.configure(image=card1)

            card1 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.list_of_players[6].cards[0]) + ".png").resize((55, 85), Image.LANCZOS))
            self.card1_p6.image = card1
            self.card1_p6.configure(image=card1)

            card1 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.list_of_players[7].cards[0]) + ".png").resize((55, 85), Image.LANCZOS))
            self.card1_p7.image = card1
            self.card1_p7.configure(image=card1)

            card1 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.list_of_players[8].cards[0]) + ".png").resize((55, 85), Image.LANCZOS))
            self.card1_p8.image = card1
            self.card1_p8.configure(image=card1)

            card1 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.list_of_players[9].cards[0]) + ".png").resize((55, 85), Image.LANCZOS))
            self.card1_p9.image = card1
            self.card1_p9.configure(image=card1)
        except IndexError:
            pass
        try:
            card2 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.list_of_players[0].cards[1]) + ".png").resize((55, 85), Image.LANCZOS))
            self.card2_p0.image = card2
            self.card2_p0.configure(image=card2)

            card2 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.list_of_players[1].cards[1]) + ".png").resize((55, 85), Image.LANCZOS))
            self.card2_p1.image = card2
            self.card2_p1.configure(image=card2)

            card2 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.list_of_players[2].cards[1]) + ".png").resize((55, 85), Image.LANCZOS))
            self.card2_p2.image = card2
            self.card2_p2.configure(image=card2)

            card2 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.list_of_players[3].cards[1]) + ".png").resize((55, 85), Image.LANCZOS))
            self.card2_p3.image = card2
            self.card2_p3.configure(image=card2)

            card2 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.list_of_players[4].cards[1]) + ".png").resize((55, 85), Image.LANCZOS))
            self.card2_p4.image = card2
            self.card2_p4.configure(image=card2)

            card2 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.list_of_players[5].cards[1]) + ".png").resize((55, 85), Image.LANCZOS))
            self.card2_p5.image = card2
            self.card2_p5.configure(image=card2)

            card2 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.list_of_players[6].cards[1]) + ".png").resize((55, 85), Image.LANCZOS))
            self.card2_p6.image = card2
            self.card2_p6.configure(image=card2)

            card2 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.list_of_players[7].cards[1]) + ".png").resize((55, 85), Image.LANCZOS))
            self.card2_p7.image = card2
            self.card2_p7.configure(image=card2)

            card2 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.list_of_players[8].cards[1]) + ".png").resize((55, 85), Image.LANCZOS))
            self.card2_p8.image = card2
            self.card2_p8.configure(image=card2)

            card2 = ImageTk.PhotoImage(
                Image.open("cards\\" + str(game.list_of_players[9].cards[1]) + ".png").resize((55, 85), Image.LANCZOS))
            self.card2_p9.image = card2
            self.card2_p9.configure(image=card2)
        except IndexError:
            pass
        try:
            self.stake_label_p0["text"] = "Stake: " + str(game.list_of_players[0].stake)
            self.stake_label_p1["text"] = "Stake: " + str(game.list_of_players[1].stake)
            self.stake_label_p2["text"] = "Stake: " + str(game.list_of_players[2].stake)
            self.stake_label_p3["text"] = "Stake: " + str(game.list_of_players[3].stake)
            self.stake_label_p4["text"] = "Stake: " + str(game.list_of_players[4].stake)
            self.stake_label_p5["text"] = "Stake: " + str(game.list_of_players[5].stake)
            self.stake_label_p6["text"] = "Stake: " + str(game.list_of_players[6].stake)
            self.stake_label_p7["text"] = "Stake: " + str(game.list_of_players[7].stake)
            self.stake_label_p8["text"] = "Stake: " + str(game.list_of_players[8].stake)
            self.stake_label_p9["text"] = "Stake: " + str(game.list_of_players[9].stake)
        except IndexError:
            pass
        self.pot_label["text"] = "Pot: " + str(game.pot)
        if game.game_over:
            self.actor_label["text"] = "Winner!: " + str(game.winner.name)
            return
        print(f"round ended {game.round_ended}")

        self.actor_label["text"] = str(game.acting_player.name)

        variable = StringVar(self.action_frame)
        variable.initialize("ACTION")
        w = OptionMenu(self.action_frame, variable, *game.possible_responses)
        w.place(relx=0, rely=0.05, relheight=0.1, relwidth=0.3)
        button_go = Button(self.action_frame, text="GO", font=("Courier", 10), command=lambda: self.action_input(variable.get()))
        button_go.place(relx=1, rely=1, relheight=0.3, relwidth=0.3, anchor="se")
    """

    def action_input(self, entry0):

        response_q.put(entry0)
        game_event.set()
        time.sleep(0.1)
        if not game_info_q.empty():
            self.update(game_info_q.get())



def ask_app(question, game=""):
    print("asking...")
    print(question)
    answer = ""
    if game != "":
        game_info_q.put(game)
    game_event.wait()
    if not response_q.empty():
        answer = response_q.get()
    game_event.clear()

    return answer

def update_gui(game1):
    print("updating gui...")
    print(game1)

def play(game):
    game_info_q.put(game)
    update_gui(game)
    #play game
    time.sleep(1)
    game_info_q.put(game)


def run_app():
    app = App()
    app.mainloop()

def run_game_data():
    game0 = Game()
    while True:
        play(game0)

game_event = threading.Event()
response_q = queue.Queue()
game_info_q = queue.Queue()
end_update = threading.Event()
t1 = threading.Thread(target=run_app)
t1.start()
t2 = threading.Thread(target=run_game_data())
t2.start()

if __name__ == "__main__":
    print("running")