#include <gtest/gtest.h>
#include "parser_param_test.h"


TEST_P(ParseTest, Damage) {
    const ExtendedLineInfo& eli = GetParam();
    // TODO: Need a way of selecting which function in parser to use depending
    // on what I want to test.
    LineInfo result = parser->otherHitByNano(eli.message);
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

INSTANTIATE_TEST_CASE_P(otherHitByNano, ParseTest,
    testing::Values(
        ExtendedLineInfo("Predator Rogue was attacked with nanobots from Sgtcuddle for 1293 points of energy damage.",
                               "Sgtcuddle",
                               "Predator Rogue",
                               "damage",
                               "energy",
                               1293,
                               false,
                               false,
                               false,
                               true),
        ExtendedLineInfo("Frozen Spinetooth was attacked with nanobots for 445 points of unknown damage.",
                               "Unknown",
                               "Frozen Spinetooth",
                               "damage",
                               "unknown",
                               445,
                               false,
                               false,
                               false,
                               true)));
