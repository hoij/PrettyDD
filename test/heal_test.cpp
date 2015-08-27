#include "heal.h"
#include "line_info.h"

#include <gtest/gtest.h>
#include <limits>


class HealTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        h = new Heal;
    }
    virtual void TearDown() {
        delete h;
    }

    void verifyReceivedFromPlayerIsZero();
    void verifyDealtOnPlayerIsZero();

    Heal* h;
};


void HealTest::verifyReceivedFromPlayerIsZero() {
    // Used to verify that the LineInfo is not added to the received Heal.
    EXPECT_EQ(0, h->getActualReceived());
    EXPECT_EQ(0, h->getPotentialReceived());
    EXPECT_EQ(0, h->getCountReceived());
    EXPECT_EQ(-1, h->getActualMaxReceived());
    EXPECT_EQ(std::numeric_limits<int>::max(), h->getActualMinReceived());
    EXPECT_EQ(-1, h->getPotentialMaxReceived());
    EXPECT_EQ(std::numeric_limits<int>::max(), h->getPotentialMinReceived());
}

void HealTest::verifyDealtOnPlayerIsZero() {
    // Used to verify that the LineInfo is not added to the dealt Heal.
    EXPECT_EQ(0, h->getActualDealt());
    EXPECT_EQ(0, h->getPotentialDealt());
    EXPECT_EQ(0, h->getCountDealt());
    EXPECT_EQ(-1, h->getActualMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), h->getActualMinDealt());
    EXPECT_EQ(-1, h->getPotentialMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), h->getPotentialMinDealt());
}

TEST_F(HealTest, addHealDealtOnPlayer) {
    /* Add a heal */
    LineInfo li1;
    li1.subtype = "potential";
    li1.amount = 3000;
    h->addHealDealtOnPlayer(li1);

    /* Add a second larger heal */
    LineInfo li2;
    li2.subtype = "potential";
    li2.amount = 3001;
    h->addHealDealtOnPlayer(li2);

    /* Add a third smallest heal */
    LineInfo li3;
    li3.subtype = "potential";
    li3.amount = 2999;
    h->addHealDealtOnPlayer(li3);

    /* Add a fourth heal of the same size*/
    LineInfo li4;
    li4.subtype = "potential";
    li4.amount = 2999;
    h->addHealDealtOnPlayer(li4);

    EXPECT_EQ(li1.amount + li2.amount + li3.amount + li4.amount,
              h->getPotentialDealt());
    EXPECT_EQ(4, h->getCountDealt());
    EXPECT_EQ(li2.amount, h->getPotentialMaxDealt());
    EXPECT_EQ(li3.amount, h->getPotentialMinDealt());

    /* Verify values that should not have changed */
    EXPECT_EQ(0, h->getActualDealt());
    EXPECT_EQ(-1, h->getActualMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), h->getActualMinDealt());
    verifyReceivedFromPlayerIsZero();
}

TEST_F(HealTest, addHealReceivedFromPlayer_actual) {
    /* Add a heal */
    LineInfo li1;
    li1.subtype = "actual";
    li1.amount = 99;
    h->addHealReceivedFromPlayer(li1);

    /* Add a second larger heal */
    LineInfo li2;
    li2.subtype = "actual";
    li2.amount = 100;
    h->addHealReceivedFromPlayer(li2);

    /* Add a third smallest heal */
    LineInfo li3;
    li3.subtype = "actual";
    li3.amount = 98;
    h->addHealReceivedFromPlayer(li3);
    /* Add a fourth heal of the same size */
    LineInfo li4;
    li4.subtype = "actual";
    li4.amount = 98;
    h->addHealReceivedFromPlayer(li4);

    /* Verify */
    EXPECT_EQ(li1.amount + li2.amount + li3.amount + li4.amount,
              h->getActualReceived());
    EXPECT_EQ(4, h->getCountReceived());
    EXPECT_EQ(li2.amount, h->getActualMaxReceived());
    EXPECT_EQ(li3.amount, h->getActualMinReceived());

    /* Verify values that should not have changed */
    EXPECT_EQ(0, h->getPotentialReceived());
    EXPECT_EQ(-1, h->getPotentialMaxReceived());
    EXPECT_EQ(std::numeric_limits<int>::max(), h->getPotentialMinReceived());

    verifyDealtOnPlayerIsZero();
}

TEST(HealOperatorPlusTest, addToEmptyHeal) {

    LineInfo li1;
    LineInfo li2;
    LineInfo li3;
    li1.subtype = "actual";
    li2.subtype = "potential";
    li3.subtype = "potential";
    li1.amount = 1999;
    li2.amount = 50000;
    li3.amount = 10000;

    Heal h1;
    Heal h2;
    h2.addHealReceivedFromPlayer(li1);
    h2.addHealDealtOnPlayer(li2);
    h2.addHealReceivedFromPlayer(li3);

    Heal h3;
    h3 = h1 + h2;

    EXPECT_EQ(li1.amount, h3.getActualReceived());
    EXPECT_EQ(li3.amount, h3.getPotentialReceived());
    EXPECT_EQ(2, h3.getCountReceived());
    EXPECT_EQ(li1.amount, h3.getActualMaxReceived());
    EXPECT_EQ(li1.amount, h3.getActualMinReceived());
    EXPECT_EQ(li3.amount, h3.getPotentialMaxReceived());
    EXPECT_EQ(li3.amount, h3.getPotentialMinReceived());

    EXPECT_EQ(0, h3.getActualDealt());
    EXPECT_EQ(li2.amount, h3.getPotentialDealt());
    EXPECT_EQ(1, h3.getCountDealt());
    EXPECT_EQ(-1, h3.getActualMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), h3.getActualMinDealt());
    EXPECT_EQ(li2.amount, h3.getPotentialMaxDealt());
    EXPECT_EQ(li2.amount, h3.getPotentialMinDealt());
}
