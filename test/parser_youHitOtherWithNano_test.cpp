#include <gtest/gtest.h>
#include "parser_param_test.h"


INSTANTIATE_TEST_CASE_P(youHitOtherWithNano, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("You hit other with nano",
                              "You hit Kyr'Ozch Hive Medic with nanobots for 798 points of projectile damage."),
            ExtendedLineInfo("You",
                             "Kyr'Ozch Hive Medic",
                             "damage",
                             "projectile",
                             798,
                             false,
                             false,
                             false,
                             true))));

