#include <gtest/gtest.h>
#include "parser_param_test.h"


INSTANTIATE_TEST_CASE_P(meGotHealth, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Me got health",
                              "You were healed for 193 points."),
            ExtendedLineInfo("",
                             "You",
                             "heal",
                             "actual",
                             193)),
        std::make_tuple(
            FormattedLineStub("Me got health",
                              "You got healed by Starphoenix for 3173 points of health."),
            ExtendedLineInfo("Starphoenix",
                             "You",
                             "heal",
                             "potential",
                             3173)),
        std::make_tuple(
            FormattedLineStub("Me got health",
                              "You were healed for 1949 points."),
            ExtendedLineInfo("",
                             "You",
                             "heal",
                             "actual",
                             1949))));

