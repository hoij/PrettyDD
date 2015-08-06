#include "parser_param_test.h"

#include <gtest/gtest.h>


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
