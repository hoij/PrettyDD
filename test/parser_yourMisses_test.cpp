#include <gtest/gtest.h>
#include "parser_param_test.h"


INSTANTIATE_TEST_CASE_P(yourMisses, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Your misses",
                              "You try to attack Peal Thunder with Brawl, but you miss!"),
            ExtendedLineInfo("You",
                             "Peal Thunder",
                             "damage",
                             "Brawling",
                             0,
                             false,
                             false,
                             true)),
        std::make_tuple(
            FormattedLineStub("Your misses",
                              "You tried to hit Stim Fiend, but missed!"),
            ExtendedLineInfo("You",
                             "Stim Fiend",
                             "damage",
                             "regular miss",
                             0,
                             false,
                             false,
                             true))));
