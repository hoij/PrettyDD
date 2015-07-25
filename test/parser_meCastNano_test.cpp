#include <gtest/gtest.h>
#include "parser_param_test.h"


TEST_P(ParseTest, Damage) {
    const ExtendedLineInfo& eli = GetParam();
    LineInfo result = parser->meCastNano(eli.message);
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
    EXPECT_EQ(eli.nanoProgramName, result.nanoProgramName);
}

INSTANTIATE_TEST_CASE_P(meCastNano, ParseTest,
    testing::Values(
        ExtendedLineInfo("Executing Nano Program: Improved Mongo Crush!.",
                         "You",
                         "",
                         "nano cast",
                         "execute",
                         0,
                         false,
                         false,
                         false,
                         false,
                         true,
                         "Improved Mongo Crush!"),
        ExtendedLineInfo("Nano program executed successfully.",
                         "You",
                         "",
                         "nano cast",
                         "land"),
        ExtendedLineInfo("Executing Nano Program: Thorough Overhaul.",
                         "You",
                         "",
                         "nano cast",
                         "execute",
                         0,
                         false,
                         false,
                         false,
                         false,
                         true,
                         "Thorough Overhaul"),
        ExtendedLineInfo("Executing Nano Program: Beauty of Life.",
                         "You",
                         "",
                         "nano cast",
                         "execute",
                         0,
                         false,
                         false,
                         false,
                         false,
                         true,
                         "Beauty of Life"),
        ExtendedLineInfo("Executing Nano Program: Uncontrollable Body Tremors.",
                         "You",
                         "",
                         "nano cast",
                         "execute",
                         0,
                         false,
                         false,
                         false,
                         false,
                         true,
                         "Uncontrollable Body Tremors"),
        ExtendedLineInfo("Nano program aborted.",
                         "You",
                         "",
                         "nano cast",
                         "abort"),
        ExtendedLineInfo("Target resisted.",
                         "You",
                         "",
                         "nano cast",
                         "resist"),
        ExtendedLineInfo("Your target countered the nano program.",
                         "You",
                         "",
                         "nano cast",
                         "counter")));
