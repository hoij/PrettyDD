#include <gtest/gtest.h>
#include "parser_param_test.h"


INSTANTIATE_TEST_CASE_P(youGaveNano, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("You gave nano",
                              "You increased nano on Sayet for 2102 points."),
            ExtendedLineInfo("You",
                             "Sayet",
                             "nano",
                             "",
                             2102))));
