#include <gtest/gtest.h>
#include "parser_param_test.h"


TEST_P(ParseTest, Damage) {
    const ExtendedLineInfo& eli = GetParam();
    LineInfo result = parser->otherMisses(eli.message);
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

INSTANTIATE_TEST_CASE_P(otherMisses, ParseTest,
    testing::Values(
        ExtendedLineInfo("Predator Rogue tried to hit you, but missed!",
                         "Predator Rogue",
                         "You",
                         "damage",
                         "regular miss",
                         0,
                         false,
                         false,
                         true),
        ExtendedLineInfo("Keepone tries to attack you with FastAttack, but misses!",
                         "Keepone",
                         "You",
                         "damage",
                         "Fast Attack",
                         0,
                         false,
                         false,
                         true)));
