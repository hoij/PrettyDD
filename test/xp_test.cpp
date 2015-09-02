#include "xp.h"
#include "line_info.h"

#include <gtest/gtest.h>
#include <limits>


class XPTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        xp = new XP;
    }
    virtual void TearDown() {
        delete xp;
    }

    void addXP(std::string type, std::string subtype, int amount);

    XP* xp;
};

void XPTest::addXP(std::string type, std::string subtype, int amount) {
    LineInfo li;
    li.type = type;
    li.subtype = subtype;
    li.amount = amount;
    xp->add(li);
}

TEST_F(XPTest, add) {
    /* Adds xp of both gained and lost subtypes and verifies that they've been
    added correctly. */
    int midXP1 = 50000000;
    int midXP2 = 50000001;
    int minXP = 49999999;
    int maxXP = 50000002;
    addXP("xp", "gained", midXP1);
    addXP("xp", "gained", midXP2);
    addXP("xp", "gained", minXP);
    addXP("xp", "gained", maxXP);

    int midAIXP1 = 3999;
    int midAIXP2 = 3998;
    int maxAIXP = 4000;
    int minAIXP = 0;
    addXP("aixp", "gained", midAIXP1);
    addXP("aixp", "gained", midAIXP2);
    addXP("aixp", "gained", maxAIXP);
    addXP("aixp", "gained", minAIXP);

    int midXPLost = 500000000;
    int maxXPLost = 500000001;
    int minXPLost = 499999999;
    addXP("xp", "lost", maxXPLost);
    addXP("xp", "lost", midXPLost);
    addXP("xp", "lost", minXPLost);

    EXPECT_EQ(midXP1 + midXP2 + minXP + maxXP, xp->getTotalGained("xp"));
    EXPECT_EQ(4, xp->getCountGained("xp"));
    EXPECT_EQ(maxXP, xp->getMaxGained("xp"));
    EXPECT_EQ(minXP, xp->getMinGained("xp"));

    EXPECT_EQ(midAIXP1 + midAIXP2 + maxAIXP + minAIXP, xp->getTotalGained("aixp"));
    EXPECT_EQ(4, xp->getCountGained("aixp"));
    EXPECT_EQ(maxAIXP, xp->getMaxGained("aixp"));
    EXPECT_EQ(minAIXP, xp->getMinGained("aixp"));

    EXPECT_EQ(midXPLost + maxXPLost + minXPLost, xp->getTotalLost("xp"));
    EXPECT_EQ(3, xp->getCountLost("xp"));
    EXPECT_EQ(maxXPLost, xp->getMaxLost("xp"));
    EXPECT_EQ(minXPLost, xp->getMinLost("xp"));
}

TEST_F(XPTest, getNonexistingXPType) {
    /* No "xp" has been added so the getters should return a standard value */
    EXPECT_EQ(0, xp->getTotalGained("xp"));
    EXPECT_EQ(0, xp->getCountGained("xp"));
    EXPECT_EQ(-1, xp->getMaxGained("xp"));
    EXPECT_EQ(std::numeric_limits<int>::max(), xp->getMinGained("xp"));
    EXPECT_EQ(0, xp->getTotalLost("xp"));
    EXPECT_EQ(0, xp->getCountLost("xp"));
    EXPECT_EQ(-1, xp->getMaxLost("xp"));
    EXPECT_EQ(std::numeric_limits<int>::max(), xp->getMinLost("xp"));
}
