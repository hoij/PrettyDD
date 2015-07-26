#include <gtest/gtest.h>
#include "parser_param_test.h"


INSTANTIATE_TEST_CASE_P(meHitByMonster, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Me hit by monster",
                              "Predator Rogue hit you for 306 points of melee damage."),
            ExtendedLineInfo("Predator Rogue",
                             "You",
                             "damage",
                             "melee",
                             306)),
        std::make_tuple(
            FormattedLineStub("Me hit by monster",
                              "Predator Rogue hit you for 717 points of melee damage.Critical hit!"),
            ExtendedLineInfo("Predator Rogue",
                             "You",
                             "damage",
                             "melee",
                             717,
                             true)),
        std::make_tuple(
            FormattedLineStub("Me hit by monster",
                              "Someone's reflect shield hit you for 121 points of damage."),
            ExtendedLineInfo("Unknown",
                             "You",
                             "damage",
                             "reflect shield",
                             121)),
        std::make_tuple(
            FormattedLineStub("Me hit by monster",
                              "Someone's damage shield hit you for 132 points of damage."),
            ExtendedLineInfo("Unknown",
                             "You",
                             "damage",
                             "damage shield",
                             132)),
        std::make_tuple(
            FormattedLineStub("Me hit by monster",
                              "Assembler Leader hit you for 1966 points of melee damage. Glancing hit."),
            ExtendedLineInfo("Assembler Leader",
                             "You",
                             "damage",
                             "melee",
                             1966,
                             false,
                             true))));

