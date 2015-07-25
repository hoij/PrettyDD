#include <gtest/gtest.h>
#include "parser_param_test.h"


TEST_P(ParseTest, Damage) {
    const ExtendedLineInfo& eli = GetParam();
    // TODO: Need a way of selecting which function in parser to use depending
    // on what I want to test.
    LineInfo result = parser->youHitOther(eli.message);
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
        ExtendedLineInfo("You hit Predator Rogue for 2357 points of poison damage.",
                               "You",
                               "Predator Rogue",
                               "damage",
                               "poison",
                               2357),
        ExtendedLineInfo("You hit Predator Rogue for 2329 points of chemical damage.",
                               "You",
                               "Predator Rogue",
                               "damage",
                               "chemical",
                               2329),
        ExtendedLineInfo("You hit Predator Rogue for 2357 points of Fast Attack damage.",
                               "You",
                               "Predator Rogue",
                               "damage",
                               "Fast Attack",
                               2357),
        ExtendedLineInfo("You hit Predator Rogue for 1942 points of Brawling damage.",
                               "You",
                               "Predator Rogue",
                               "damage",
                               "Brawling",
                               1942),
        ExtendedLineInfo("You hit Kyr'Ozch Hive Technician for 3017 points of Fling Shot damage.",
                               "You",
                               "Kyr'Ozch Hive Technician",
                               "damage",
                               "Fling Shot",
                               3017),
        ExtendedLineInfo("You hit Predator Rogue for 3911 points of poison damage.Critical hit!",
                               "You",
                               "Predator Rogue",
                               "damage",
                               "poison",
                               3911,
                               true),
        ExtendedLineInfo("Your reflect shield hit Predator Rogue for 49 points of damage.",
                               "You",
                               "Predator Rogue",
                               "damage",
                               "reflect shield",
                               49),
        ExtendedLineInfo("Your damage shield hit Predator Rogue for 163 points of damage.",
                               "You",
                               "Predator Rogue",
                               "damage",
                               "damage shield",
                               163)));
