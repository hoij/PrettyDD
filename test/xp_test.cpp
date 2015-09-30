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
    int minAIXP = 1;
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
    EXPECT_EQ(midAIXP2, xp->getMinGained("aixp"));

    EXPECT_EQ(midXPLost + maxXPLost + minXPLost, xp->getTotalLost("xp"));
    EXPECT_EQ(3, xp->getCountLost("xp"));
    EXPECT_EQ(maxXPLost, xp->getMaxLost("xp"));
    EXPECT_EQ(minXPLost, xp->getMinLost("xp"));
}

TEST_F(XPTest, xpPerHour) {
    int midXP1 = 3999;
    int midXP2 = 3998;
    int maxXP = 4000;
    int minXP = 1;
    addXP("xp", "gained", midXP1);
    addXP("xp", "gained", midXP2);
    addXP("xp", "gained", maxXP);
    addXP("xp", "gained", minXP);
    xp->calcXPH(29);
    // Expected: (3999 + 3998 + 4000 + 1) / (29 / 3600)
    EXPECT_EQ(1489406, xp->getXPH("xp"));
    EXPECT_EQ(1489406, xp->getXPHGained("xp"));

    int xpLost = 200;
    addXP("xp", "lost", xpLost);
    xp->calcXPH(29);
    // Expected: (3999 + 3998 + 4000 + 1 - 200) / (29 / 3600)
    EXPECT_EQ(1464579, xp->getXPH("xp"));
    EXPECT_EQ(1489406, xp->getXPHGained("xp"));

    xp->calcXPH(0);
    EXPECT_EQ(0, xp->getXPH("xp"));
    EXPECT_EQ(0, xp->getXPHGained("xp"));

    /* Negative XPH */
    int xpLost2 = 1000000;
    addXP("xp", "lost", xpLost2);
    xp->calcXPH(29);
    // Expected: (3999 + 3998 + 4000 + 1 - 200 - 1000000) / (29 / 3600)
    EXPECT_EQ(-122673351, xp->getXPH("xp"));
    EXPECT_EQ(1489406, xp->getXPHGained("xp"));
}

TEST_F(XPTest, getNonexistingXPType) {
    /* No "xp" has been added so the getters should return a standard value */
    EXPECT_EQ(0, xp->getTotal("xp"));
    EXPECT_EQ(0, xp->getXPH("xp"));
    EXPECT_EQ(0, xp->getTotalGained("xp"));
    EXPECT_EQ(0, xp->getXPHGained("xp"));
    EXPECT_EQ(0, xp->getCountGained("xp"));
    EXPECT_EQ(-1, xp->getMaxGained("xp"));
    EXPECT_EQ(std::numeric_limits<int>::max(), xp->getMinGained("xp"));
    EXPECT_EQ(0, xp->getTotalLost("xp"));
    EXPECT_EQ(0, xp->getCountLost("xp"));
    EXPECT_EQ(-1, xp->getMaxLost("xp"));
    EXPECT_EQ(std::numeric_limits<int>::max(), xp->getMinLost("xp"));
}
