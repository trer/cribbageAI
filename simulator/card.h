#pragma once
#include <iostream>
#include <string>

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
        std::string string_format();
        bool operator<(card const & b);
        bool operator==(card const & b);

};