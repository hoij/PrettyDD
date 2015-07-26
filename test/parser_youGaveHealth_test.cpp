#include <gtest/gtest.h>
#include "parser_param_test.h"


INSTANTIATE_TEST_CASE_P(youGaveHealth, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("You gave health",
                              "You healed Letter for 3741 points of health."),
            ExtendedLineInfo("You",
                             "Letter",
                             "heal",
                             "potential",
                             3741))));

