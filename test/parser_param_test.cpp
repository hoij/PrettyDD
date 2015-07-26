#include "parser_param_test.h"


Parser* ParseTest::parser = nullptr;

TEST_P(ParseTest, parser) {
    // Parameterized test used by parser test cases.
    // Contents for FormattedLineStub and ExtendedLineInfo is create in
    // respective test case instantiation.
    FormattedLineStub fls;
    ExtendedLineInfo eli;
    std::tie(fls, eli) = GetParam();

    LineInfo result = parser->parse(fls);

    EXPECT_EQ(eli.dealer_name, result.dealer_name);
    EXPECT_EQ(eli.receiver_name, result.receiver_name);
    EXPECT_EQ(eli.type, result.type);
    EXPECT_EQ(eli.subtype, result.subtype);
    EXPECT_EQ(eli.amount, result.amount);
    EXPECT_EQ(eli.crit, result.crit);
    EXPECT_EQ(eli.deflect, result.deflect);
    EXPECT_EQ(eli.miss, result.miss);
    EXPECT_EQ(eli.nanobots, result.nanobots);
    EXPECT_EQ(eli.hasStats, result.hasStats);
}
