#include "parser_param_test.h"

#include <gtest/gtest.h>


INSTANTIATE_TEST_CASE_P(meHitByMonster, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Me hit by monster",
                              "Predator Rogue hit you for 306 points of melee damage."),
            ExtendedLineInfo("Predator Rogue",
                             "You",
                             LineType::damage,
                             "melee",
                             306)),
        std::make_tuple(
            FormattedLineStub("Me hit by monster",
                              "Predator Rogue hit you for 717 points of melee damage.Critical hit!"),
            ExtendedLineInfo("Predator Rogue",
                             "You",
                             LineType::damage,
                             "melee",
                             717,
                             true)),
        std::make_tuple(
            FormattedLineStub("Me hit by monster",
                              "Someone's reflect shield hit you for 121 points of damage."),
            ExtendedLineInfo("Unknown",
                             "You",
                             LineType::damage,
                             "reflect shield",
                             121)),
        std::make_tuple(
            FormattedLineStub("Me hit by monster",
                              "Someone's damage shield hit you for 132 points of damage."),
            ExtendedLineInfo("Unknown",
                             "You",
                             LineType::damage,
                             "damage shield",
                             132)),
        std::make_tuple(
            FormattedLineStub("Me hit by monster",
                              "Assembler Leader hit you for 1966 points of melee damage. Glancing hit."),
            ExtendedLineInfo("Assembler Leader",
                             "You",
                             LineType::damage,
                             "melee",
                             1966,
                             false,
                             true))));

INSTANTIATE_TEST_CASE_P(meHitByEnvironment, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Me hit by environment",
                              "You were damaged by a toxic substance for 123 points of damage."),
            ExtendedLineInfo("Environment",
                             "You",
                             LineType::damage,
                             "a toxic substance",
                             123))));

INSTANTIATE_TEST_CASE_P(meHitByNano, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Me hit by nano",
                              "You were attacked with nanobots from Predator Rogue for 875 points of poison damage."),
            ExtendedLineInfo("Predator Rogue",
                             "You",
                             LineType::damage,
                             "poison",
                             875,
                             false,
                             false,
                             false,
                             true)),
        std::make_tuple(
            FormattedLineStub("Me hit by nano",
                             "You were attacked with nanobots for 9 points of unknown damage."),
            ExtendedLineInfo("Unknown",
                             "You",
                             LineType::damage,
                             "unknown",
                             9,
                             false,
                             false,
                             false,
                             true))));

INSTANTIATE_TEST_CASE_P(meHitByPlayer, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Me hit by player",
                              "Player Balas hit you for 854 points of projectile damage."),
            ExtendedLineInfo("Balas",
                             "You",
                             LineType::damage,
                             "projectile",
                             854)),
        std::make_tuple(
            FormattedLineStub("Me hit by player",
                              "Balas hit you for 949 points of Aimed Shot damage."),
            ExtendedLineInfo("Balas",
                             "You",
                             LineType::damage,
                             "Aimed Shot",
                             949))));
