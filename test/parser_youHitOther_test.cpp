#include <gtest/gtest.h>
#include "parser_param_test.h"


INSTANTIATE_TEST_CASE_P(youHitOther, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("You hit other",
                              "You hit Predator Rogue for 2357 points of poison damage."),
            ExtendedLineInfo("You",
                             "Predator Rogue",
                             "damage",
                             "poison",
                             2357)),
        std::make_tuple(
            FormattedLineStub("You hit other",
                              "You hit Predator Rogue for 2329 points of chemical damage."),
            ExtendedLineInfo("You",
                             "Predator Rogue",
                             "damage",
                             "chemical",
                             2329)),
        std::make_tuple(
            FormattedLineStub("You hit other",
                              "You hit Predator Rogue for 2357 points of Fast Attack damage."),
            ExtendedLineInfo("You",
                             "Predator Rogue",
                             "damage",
                             "Fast Attack",
                             2357)),
        std::make_tuple(
            FormattedLineStub("You hit other",
                              "You hit Predator Rogue for 1942 points of Brawling damage."),
            ExtendedLineInfo("You",
                             "Predator Rogue",
                             "damage",
                             "Brawling",
                             1942)),
        std::make_tuple(
            FormattedLineStub("You hit other",
                              "You hit Kyr'Ozch Hive Technician for 3017 points of Fling Shot damage."),
            ExtendedLineInfo("You",
                             "Kyr'Ozch Hive Technician",
                             "damage",
                             "Fling Shot",
                             3017)),
        std::make_tuple(
            FormattedLineStub("You hit other",
                              "You hit Predator Rogue for 3911 points of poison damage.Critical hit!"),
            ExtendedLineInfo("You",
                             "Predator Rogue",
                             "damage",
                             "poison",
                             3911,
                             true)),
        std::make_tuple(
            FormattedLineStub("You hit other",
                              "Your reflect shield hit Predator Rogue for 49 points of damage."),
            ExtendedLineInfo("You",
                             "Predator Rogue",
                             "damage",
                             "reflect shield",
                             49)),
        std::make_tuple(
            FormattedLineStub("You hit other",
                              "Your damage shield hit Predator Rogue for 163 points of damage."),
            ExtendedLineInfo("You",
                             "Predator Rogue",
                             "damage",
                             "damage shield",
                             163))));
