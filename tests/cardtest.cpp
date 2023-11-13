#pragma once
#include <gtest/gtest.h>
#include ".././simulator/card.h"

TEST(card, cardtest) {
    card c1 = card();
    char non_char = 'N';
    GTEST_ASSERT_EQ((int) c1.get_suit(false), (int) non_char) << "An uninitialized card should be N";
    GTEST_ASSERT_EQ(c1.get_suit(true), -1) << "The nummeric value of the suit Non should be -1";
    GTEST_ASSERT_EQ(c1.get_value(false), 0) << "An uninitialized card should have the value of 0";

    char H_char = 'H';
    card c2 = card(13, H_char);
    GTEST_ASSERT_EQ((int) c2.get_suit(false),(int) H_char) << "Suit should be H";
    GTEST_ASSERT_EQ(c2.get_suit(true), 2) << "The nummeric value of the suit of Hearts should be 2";
    GTEST_ASSERT_EQ(c2.get_value(false), 10) << "The counting value of a king card should be 10";
    GTEST_ASSERT_EQ(c2.get_value(true), 13) << "The face value of a king should be 13";

    //Currently no logic to ensure that cards get initialized within the right values.
}
