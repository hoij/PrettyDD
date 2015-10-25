#include "parser_param_test.h"

#include <gtest/gtest.h>


INSTANTIATE_TEST_CASE_P(otherMisses, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Other misses",
                              "Predator Rogue tried to hit you, but missed!"),
            ExtendedLineInfo("Predator Rogue",
                             "You",
                             "damage",
                             "Regular Miss",
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
                             "Regular Miss",
                             0,
                             false,
                             false,
                             true))));
