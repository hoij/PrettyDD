#include <gtest/gtest.h>
#include "parser_param_test.h"


INSTANTIATE_TEST_CASE_P(meGotNano, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Me got nano",
                              "You got nano from Jspe80 for 288 points."),
            ExtendedLineInfo("Jspe80",
                             "You",
                             "nano",
                             "",
                             288))));
