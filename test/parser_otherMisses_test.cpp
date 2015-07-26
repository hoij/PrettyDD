#include <gtest/gtest.h>
#include "parser_param_test.h"


INSTANTIATE_TEST_CASE_P(otherMisses, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Other misses",
                              "Predator Rogue tried to hit you, but missed!"),
            ExtendedLineInfo("Predator Rogue",
                             "You",
                             "damage",
                             "regular miss",
                             0,
                             false,
                             false,
                             true)),
        std::make_tuple(
            FormattedLineStub("Other misses",
                              "Keepone tries to attack you with FastAttack, but misses!"),
            ExtendedLineInfo("Keepone",
                             "You",
                             "damage",
                             "Fast Attack",
                             0,
                             false,
                             false,
                             true))));
