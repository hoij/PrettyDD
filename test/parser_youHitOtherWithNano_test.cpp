#include <gtest/gtest.h>
#include "parser_param_test.h"


TEST_P(ParseTest, Damage) {
    const ExtendedLineInfo& eli = GetParam();
    // TODO: Need a way of selecting which function in parser to use depending
    // on what I want to test.
    LineInfo result = parser->youHitOtherWithNano(eli.message);
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
        ExtendedLineInfo("You hit Kyr'Ozch Hive Medic with nanobots for 798 points of projectile damage.",
                               "You",
                               "Kyr'Ozch Hive Medic",
                               "damage",
                               "projectile",
                               798,
                               false,
                               false,
                               false,
                               true)));

