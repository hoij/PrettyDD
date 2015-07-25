#include <gtest/gtest.h>
#include "parser_param_test.h"


TEST_P(ParseTest, Damage) {
    const ExtendedLineInfo& eli = GetParam();
    // TODO: Need a way of selecting which function in parser to use depending
    // on what I want to test.
    LineInfo result = parser->meHitByMonster(eli.message);
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

INSTANTIATE_TEST_CASE_P(meHitByMonster, ParseTest,
    testing::Values(
        ExtendedLineInfo("Predator Rogue hit you for 306 points of melee damage.",
                         "Predator Rogue",
                         "You",
                         "damage",
                         "melee",
                         306),
        ExtendedLineInfo("Predator Rogue hit you for 717 points of melee damage.Critical hit!",
                         "Predator Rogue",
                         "You",
                         "damage",
                         "melee",
                         717,
                         true),
        ExtendedLineInfo("Someone's reflect shield hit you for 121 points of damage.",
                         "Unknown",
                         "You",
                         "damage",
                         "reflect shield",
                         121),
        ExtendedLineInfo("Someone's damage shield hit you for 132 points of damage.",
                         "Unknown",
                         "You",
                         "damage",
                         "damage shield",
                         132),
        ExtendedLineInfo("Assembler Leader hit you for 1966 points of melee damage. Glancing hit.",
                         "Assembler Leader",
                         "You",
                         "damage",
                         "melee",
                         1966,
                         false,
                         true)));

