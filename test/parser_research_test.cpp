#include <gtest/gtest.h>
#include "parser_param_test.h"


INSTANTIATE_TEST_CASE_P(research, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Research",
                              "139139 of your XP were allocated to your personal research.<br>"),
            ExtendedLineInfo("",
                             "You",
                             "research",
                             "",
                             139139)))) ;




