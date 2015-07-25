#include <gtest/gtest.h>
#include "parser_param_test.h"


TEST_P(ParseTest, Damage) {
    const ExtendedLineInfo& eli = GetParam();
    // TODO: Need a way of selecting which function in parser to use depending
    // on what I want to test.
    LineInfo result = parser->meGotHealth(eli.message);
    EXPECT_EQ(eli.dealer_name, result.dealer_name);
    EXPECT_EQ(eli.receiver_name, result.receiver_name);
    EXPECT_EQ(eli.type, result.type);
    EXPECT_EQ(eli.subtype, result.subtype);
    EXPECT_EQ(eli.amount, result.amount);
    EXPECT_EQ(eli.crit, result.crit);
    EXPECT_EQ(eli.deflect, result.deflect);
    EXPECT_EQ(eli.miss, result.miss);
    EXPECT_EQ(eli.nanobots, result.nanobots);
    EXPECT_EQ(eli.hasStats, result.hasStats);
}

INSTANTIATE_TEST_CASE_P(meGotHealth, ParseTest,
    testing::Values(
        ExtendedLineInfo("You were healed for 193 points.",
                         "",
                         "You",
                         "heal",
                         "actual",
                         193),
        ExtendedLineInfo("You got healed by Starphoenix for 3173 points of health.",
                         "Starphoenix",
                         "You",
                         "heal",
                         "potential",
                         3173),
        ExtendedLineInfo("You were healed for 1949 points.",
                         "",
                         "You",
                         "heal",
                         "actual",
                         1949)));

