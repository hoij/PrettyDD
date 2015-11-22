#include "parser_param_test.h"

#include <gtest/gtest.h>


INSTANTIATE_TEST_CASE_P(meGotNano, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Me got nano",
                              "You got nano from Jspe80 for 288 points."),
            ExtendedLineInfo("Jspe80",
                             "You",
                             LineType::nano,
                             "",
                             288))));

INSTANTIATE_TEST_CASE_P(youGaveNano, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("You gave nano",
                              "You increased nano on Sayet for 2102 points."),
            ExtendedLineInfo("You",
                             "Sayet",
                             LineType::nano,
                             "",
                             2102))));
