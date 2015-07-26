#include <gtest/gtest.h>
#include "parser_param_test.h"


INSTANTIATE_TEST_CASE_P(meHitByEnvironment, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Me hit by environment",
                              "You were damaged by a toxic substance for 123 points of damage."),
            ExtendedLineInfo("Environment",
                             "You",
                             "damage",
                             "a toxic substance",
                             123))));

