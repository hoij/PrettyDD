#include "line_info.h"
#include "parser.h"

#include <gtest/gtest.h>
#include <tuple>



// TODO: Make this test case test through the public parser->parse

class ParseChatTest : public ::testing::TestWithParam<std::tuple<std::string, std::string, std::string>> {
public:
    static void SetUpTestCase() {
        parser = new Parser("Sgtcuddle");
    }
    static void TearDownTestCase() {
        delete parser;
    }

static Parser* parser;
};

Parser* ParseChatTest::parser = nullptr;

TEST_P(ParseChatTest, parseChat) {
    std::string message;
    std::string sender;
    std::string expectedCommand;
    std::tie(message, sender, expectedCommand) = GetParam();
    LineInfo result = parser->chat(message, sender);
    EXPECT_EQ(expectedCommand, result.command);
}

// TODO: This will need a different ExtendedLineInfo because it uses commands.
INSTANTIATE_TEST_CASE_P(parseChat, ParseChatTest,
    testing::Values(std::make_tuple("dd", "Sgtcuddle", "dd"),
                    std::make_tuple("ddd", "Sgtcuddle", ""),
                    std::make_tuple(" dd", "Sgtcuddle", ""),
                    std::make_tuple("Look at this amazing dd", "Sgtcuddle", ""),
                    std::make_tuple("dd", "SSgtcuddle", ""),
                    std::make_tuple("dd", "Sgtcuddlee", ""),
                    std::make_tuple("dd", " Sgtcuddle", ""),
                    std::make_tuple("dd", "An album cover", "")));
