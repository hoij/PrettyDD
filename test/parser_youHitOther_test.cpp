#include "parser_param_test.h"

#include <gtest/gtest.h>


INSTANTIATE_TEST_CASE_P(youHitOther, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("You hit other",
                              "You hit Predator Rogue for 2357 points of poison damage."),
            ExtendedLineInfo("You",
                             "Predator Rogue",
                             DAMAGE,
                             "poison",
                             2357)),
        std::make_tuple(
            FormattedLineStub("You hit other",
                              "You hit Predator Rogue for 2329 points of chemical damage."),
            ExtendedLineInfo("You",
                             "Predator Rogue",
                             DAMAGE,
                             "chemical",
                             2329)),
        std::make_tuple(
            FormattedLineStub("You hit other",
                              "You hit Predator Rogue for 2357 points of Fast Attack damage."),
            ExtendedLineInfo("You",
                             "Predator Rogue",
                             DAMAGE,
                             "Fast Attack",
                             2357)),
        std::make_tuple(
            FormattedLineStub("You hit other",
                              "You hit Predator Rogue for 1942 points of Brawling damage."),
            ExtendedLineInfo("You",
                             "Predator Rogue",
                             DAMAGE,
                             "Brawling",
                             1942)),
        std::make_tuple(
            FormattedLineStub("You hit other",
                              "You hit Kyr'Ozch Hive Technician for 3017 points of Fling Shot damage."),
            ExtendedLineInfo("You",
                             "Kyr'Ozch Hive Technician",
                             DAMAGE,
                             "Fling Shot",
                             3017)),
        std::make_tuple(
            FormattedLineStub("You hit other",
                              "You hit Predator Rogue for 3911 points of poison damage.Critical hit!"),
            ExtendedLineInfo("You",
                             "Predator Rogue",
                             DAMAGE,
                             "poison",
                             3911,
                             true)),
        std::make_tuple(
            FormattedLineStub("You hit other",
                              "Your reflect shield hit Predator Rogue for 49 points of damage."),
            ExtendedLineInfo("You",
                             "Predator Rogue",
                             DAMAGE,
                             "reflect shield",
                             49)),
        std::make_tuple(
            FormattedLineStub("You hit other",
                              "Your damage shield hit Predator Rogue for 163 points of damage."),
            ExtendedLineInfo("You",
                             "Predator Rogue",
                             DAMAGE,
                             "damage shield",
                             163))));

INSTANTIATE_TEST_CASE_P(youHitOtherWithNano, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("You hit other with nano",
                              "You hit Kyr'Ozch Hive Medic with nanobots for 798 points of projectile damage."),
            ExtendedLineInfo("You",
                             "Kyr'Ozch Hive Medic",
                             DAMAGE,
                             "projectile",
                             798,
                             false,
                             false,
                             false,
                             true))));
