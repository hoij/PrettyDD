#include <gtest/gtest.h>
#include "parser_param_test.h"


INSTANTIATE_TEST_CASE_P(yourPetHitByMonster, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Your pet hit by monster",
                              "Your pet Vios was damaged by a toxic substance for 25 points of damage."),
            ExtendedLineInfo("Environment",
                             "Vios",
                             "damage",
                             "a toxic substance",
                             25))));
