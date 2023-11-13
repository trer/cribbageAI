#pragma once

class card {
    private:
        int value;
        char suit;
    public:
        card();
        card(int v, char s);
        int get_value(bool distinct_face);
        char get_suit(bool numeric);
        void print_card();

};