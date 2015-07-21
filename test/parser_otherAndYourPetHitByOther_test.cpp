#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "parser_param_test.h"



TEST_P(ParseTest, Damage) {
    const ExtendedLineInfo& eli = GetParam();
    // TODO: Need a way of selecting which function in parser to use depending
    // on what I want to test.
    LineInfo result = parser->otherAndYourPetHitByOther(eli.message);
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

INSTANTIATE_TEST_CASE_P(otherAndYourPetHitByOther, ParseTest,
    testing::Values(
        ExtendedLineInfo("Sheila Marlene hit Predator Rogue for 461 points of melee damage.",
                               "Sheila Marlene",
                               "Predator Rogue",
                               "damage",
                               "melee",
                               461),
        ExtendedLineInfo("Sgtcuddle hit Predator Rogue for 4302 points of Burst damage.",
                               "Sgtcuddle",
                               "Predator Rogue",
                               "damage",
                               "Burst",
                               4302),
        ExtendedLineInfo("Sgtcuddle hit Predator Rogue for 4302 points of Burst damage.",
                               "Sgtcuddle",
                               "Predator Rogue",
                               "damage",
                               "Burst",
                               4302),
        ExtendedLineInfo("Sgtcuddle hit Predator Rogue for 15000 points of Full Auto damage.",
                               "Sgtcuddle",
                               "Predator Rogue",
                               "damage",
                               "Full Auto",
                               15000),
        ExtendedLineInfo("Balas's reflect shield hit Junebop for 161 points of damage.",
                               "Balas",
                               "Junebop",
                               "damage",
                               "reflect shield",
                               161),
        ExtendedLineInfo("Letter hit Reet of Paradise for 1586 points of melee damage.Critical hit!",
                               "Letter",
                               "Reet of Paradise",
                               "damage",
                               "melee",
                               1586,
                               true),
        ExtendedLineInfo("Cratdat hit Ensign - Ilari'Ra for 801 points of projectile damage. Glancing hit.",
                               "Cratdat",
                               "Ensign - Ilari'Ra",
                               "damage",
                               "projectile",
                               801,
                               false,
                               true),
        ExtendedLineInfo("Someone absorbed 8198 points of energy damage.",
                               "Unknown",
                               "Someone",
                               "damage",
                               "energy",
                               8198),
        ExtendedLineInfo("Something hit Addicted2 for 49 points of damage by reflect shield.",
                               "Unknown",
                               "Addicted2",
                               "damage",
                               "reflect shield",
                               49),
        ExtendedLineInfo("Something hit Enfodruid for 1 points of damage by damage shield.",
                               "Unknown",
                               "Enfodruid",
                               "damage",
                               "damage shield",
                               1)));

