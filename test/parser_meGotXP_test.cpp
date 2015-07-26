#include <gtest/gtest.h>
#include "parser_param_test.h"


INSTANTIATE_TEST_CASE_P(meGotXP, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Me got XP",
                              "You received 247 xp."),
            ExtendedLineInfo("",
                             "You",
                             "xp",
                             "gained",
                             247)),
        std::make_tuple(
            FormattedLineStub("Me got XP",
                              "You gained 2562 new Alien Experience Points."),
            ExtendedLineInfo("",
                             "You",
                             "aixp",
                             "gained",
                             2562)),
        std::make_tuple(
            FormattedLineStub("Me got XP",
                              "You lost 9822 xp."),
            ExtendedLineInfo("",
                             "You",
                             "xp",
                             "lost",
                             9822))));



