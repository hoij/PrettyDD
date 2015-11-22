#include "xp.h"
#include "definitions.h"
#include "line_info.h"

#include <gtest/gtest.h>
#include <limits>


class XPTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        xp = new Experience;
    }
    virtual void TearDown() {
        delete xp;
    }

    void addXP(LineType type, std::string subtype, int amount);

    Experience* xp;
};

void XPTest::addXP(LineType type, std::string subtype, int amount) {
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
    addXP(XP, "gained", midXP1);
    addXP(XP, "gained", midXP2);
    addXP(XP, "gained", minXP);
    addXP(XP, "gained", maxXP);

    int midAIXP1 = 3999;
    int midAIXP2 = 3998;
    int maxAIXP = 4000;
    int minAIXP = 1;
    addXP(AIXP, "gained", midAIXP1);
    addXP(AIXP, "gained", midAIXP2);
    addXP(AIXP, "gained", maxAIXP);
    addXP(AIXP, "gained", minAIXP);

    int midXPLost = 500000000;
    int maxXPLost = 500000001;
    int minXPLost = 499999999;
    addXP(XP, "lost", maxXPLost);
    addXP(XP, "lost", midXPLost);
    addXP(XP, "lost", minXPLost);

    EXPECT_EQ(midXP1 + midXP2 + minXP + maxXP, xp->getTotalGained(XP));
    EXPECT_EQ(4, xp->getCountGained(XP));
    EXPECT_EQ(maxXP, xp->getMaxGained(XP));
    EXPECT_EQ(minXP, xp->getMinGained(XP));

    EXPECT_EQ(midAIXP1 + midAIXP2 + maxAIXP + minAIXP, xp->getTotalGained(AIXP));
    EXPECT_EQ(4, xp->getCountGained(AIXP));
    EXPECT_EQ(maxAIXP, xp->getMaxGained(AIXP));
    EXPECT_EQ(midAIXP2, xp->getMinGained(AIXP));

    EXPECT_EQ(midXPLost + maxXPLost + minXPLost, xp->getTotalLost(XP));
    EXPECT_EQ(3, xp->getCountLost(XP));
    EXPECT_EQ(maxXPLost, xp->getMaxLost(XP));
    EXPECT_EQ(minXPLost, xp->getMinLost(XP));
}

TEST_F(XPTest, xpPerHour) {
    int midXP1 = 3999;
    int midXP2 = 3998;
    int maxXP = 4000;
    int minXP = 1;
    addXP(XP, "gained", midXP1);
    addXP(XP, "gained", midXP2);
    addXP(XP, "gained", maxXP);
    addXP(XP, "gained", minXP);
    xp->calcXPH(29);
    // Expected: (3999 + 3998 + 4000 + 1) / (29 / 3600)
    EXPECT_EQ(1489406, xp->getXPH(XP));
    EXPECT_EQ(1489406, xp->getXPHGained(XP));

    int xpLost = 200;
    addXP(XP, "lost", xpLost);
    xp->calcXPH(29);
    // Expected: (3999 + 3998 + 4000 + 1 - 200) / (29 / 3600)
    EXPECT_EQ(1464579, xp->getXPH(XP));
    EXPECT_EQ(1489406, xp->getXPHGained(XP));

    xp->calcXPH(0);
    EXPECT_EQ(0, xp->getXPH(XP));
    EXPECT_EQ(0, xp->getXPHGained(XP));

    /* Negative XPH */
    int xpLost2 = 1000000;
    addXP(XP, "lost", xpLost2);
    xp->calcXPH(29);
    // Expected: (3999 + 3998 + 4000 + 1 - 200 - 1000000) / (29 / 3600)
    EXPECT_EQ(-122673351, xp->getXPH(XP));
    EXPECT_EQ(1489406, xp->getXPHGained(XP));
}

TEST_F(XPTest, getNonexistingXPType) {
    /* No xp has been added so the getters should return a standard value */
    EXPECT_EQ(0, xp->getTotal(XP));
    EXPECT_EQ(0, xp->getXPH(XP));
    EXPECT_EQ(0, xp->getTotalGained(XP));
    EXPECT_EQ(0, xp->getXPHGained(XP));
    EXPECT_EQ(0, xp->getCountGained(XP));
    EXPECT_EQ(-1, xp->getMaxGained(XP));
    EXPECT_EQ(std::numeric_limits<int>::max(), xp->getMinGained(XP));
    EXPECT_EQ(0, xp->getTotalLost(XP));
    EXPECT_EQ(0, xp->getCountLost(XP));
    EXPECT_EQ(-1, xp->getMaxLost(XP));
    EXPECT_EQ(std::numeric_limits<int>::max(), xp->getMinLost(XP));
}
