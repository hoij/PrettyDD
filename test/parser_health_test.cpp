#include "parser_param_test.h"

#include <gtest/gtest.h>


INSTANTIATE_TEST_CASE_P(meGotHealth, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Me got health",
                              "You were healed for 193 points."),
            ExtendedLineInfo("",
                             "You",
                             LineType::heal,
                             "actual",
                             193)),
        std::make_tuple(
            FormattedLineStub("Me got health",
                              "You got healed by Starphoenix for 3173 points of health."),
            ExtendedLineInfo("Starphoenix",
                             "You",
                             LineType::heal,
                             "potential",
                             3173)),
        std::make_tuple(
            FormattedLineStub("Me got health",
                              "You were healed for 1949 points."),
            ExtendedLineInfo("",
                             "You",
                             LineType::heal,
                             "actual",
                             1949))));

INSTANTIATE_TEST_CASE_P(youGaveHealth, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("You gave health",
                              "You healed Letter for 3741 points of health."),
            ExtendedLineInfo("You",
                             "Letter",
                             LineType::heal,
                             "potential",
                             3741))));
