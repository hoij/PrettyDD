#include <gtest/gtest.h>
#include "parser_param_test.h"


INSTANTIATE_TEST_CASE_P(meHitByPlayer, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Me hit by player",
                              "Player Balas hit you for 854 points of projectile damage."),
            ExtendedLineInfo("Balas",
                             "You",
                             "damage",
                             "projectile",
                             854)),
        std::make_tuple(
            FormattedLineStub("Me hit by player",
                              "Balas hit you for 949 points of Aimed Shot damage."),
            ExtendedLineInfo("Balas",
                             "You",
                             "damage",
                             "Aimed Shot",
                             949))));
