#include <gtest/gtest.h>
#include "parser_param_test.h"


INSTANTIATE_TEST_CASE_P(meGotSK, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Me got SK",
                              "You gained 200 points of Shadowknowledge."),
            ExtendedLineInfo("",
                             "You",
                             "sk",
                             "gained",
                             200)),
        std::make_tuple(
            FormattedLineStub("Me got SK",
                              "You lost 200 points of Shadowknowledge."),
            ExtendedLineInfo("",
                             "You",
                             "sk",
                             "lost",
                             200))));

