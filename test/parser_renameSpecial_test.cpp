#include <gtest/gtest.h>
#include <string.h>
#include "parser.h"


class RenameSpecialTest : public ::testing::Test {
public:
    static void SetUpTestCase() {
        parser = new Parser("");
    }
    static void TearDownTestCase() {
        delete parser;
    }

static Parser* parser;
};

Parser* RenameSpecialTest::parser = nullptr;

TEST_F(RenameSpecialTest, renameSpecials) {
    ASSERT_EQ("Fast Attack", parser->renameSpecial("FastAttack"));
    ASSERT_EQ("Fling Shot", parser->renameSpecial("FlingShot"));
    ASSERT_EQ("Brawling", parser->renameSpecial("Brawl"));
    ASSERT_EQ("Full Auto", parser->renameSpecial("FullAuto"));
    ASSERT_EQ("", parser->renameSpecial(""));
    ASSERT_EQ("regular miss", parser->renameSpecial("regular miss"));
    ASSERT_EQ("something random", parser->renameSpecial("something random"));
}
