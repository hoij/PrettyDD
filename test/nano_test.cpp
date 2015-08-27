#include "nano.h"
#include "line_info.h"

#include <gtest/gtest.h>
#include <limits>


class NanoTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        n = new Nano;
    }
    virtual void TearDown() {
        delete n;
    }

    void verifyReceivedFromPlayerIsZero();
    void verifyDealtOnPlayerIsZero();

    Nano* n;
};


void NanoTest::verifyReceivedFromPlayerIsZero() {
    // Used to verify that the LineInfo is not added to the received Nano.
    EXPECT_EQ(0, n->getTotalReceived());
    EXPECT_EQ(0, n->getCountReceived());
    EXPECT_EQ(-1, n->getMaxReceived());
    EXPECT_EQ(std::numeric_limits<int>::max(), n->getMinReceived());
}

void NanoTest::verifyDealtOnPlayerIsZero() {
    // Used to verify that the LineInfo is not added to the dealt Nano.
    EXPECT_EQ(0, n->getTotalDealt());
    EXPECT_EQ(0, n->getCountDealt());
    EXPECT_EQ(-1, n->getMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), n->getMinDealt());
}

TEST_F(NanoTest, addNanoDealtOnPlayer) {
    /* Add a nano */
    LineInfo li1;
    li1.amount = 3000;
    n->addNanoDealtOnPlayer(li1);

    /* Add a second larger nano */
    LineInfo li2;
    li2.amount = 3001;
    n->addNanoDealtOnPlayer(li2);

    /* Add a third smallest nano */
    LineInfo li3;
    li3.amount = 2999;
    n->addNanoDealtOnPlayer(li3);

    /* Add a fourth nano of the same size*/
    LineInfo li4;
    li4.amount = 2999;
    n->addNanoDealtOnPlayer(li4);

    EXPECT_EQ(li1.amount + li2.amount + li3.amount + li4.amount,
              n->getTotalDealt());
    EXPECT_EQ(4, n->getCountDealt());
    EXPECT_EQ(li2.amount, n->getMaxDealt());
    EXPECT_EQ(li3.amount, n->getMinDealt());

    /* Verify values that should not have changed */
    verifyReceivedFromPlayerIsZero();
}

TEST_F(NanoTest, addNanoReceivedFromPlayer_actual) {
    /* Add a nano */
    LineInfo li1;
    li1.amount = 99;
    n->addNanoReceivedFromPlayer(li1);

    /* Add a second larger nano */
    LineInfo li2;
    li2.amount = 100;
    n->addNanoReceivedFromPlayer(li2);

    /* Add a third smallest nano */
    LineInfo li3;
    li3.amount = 98;
    n->addNanoReceivedFromPlayer(li3);

    /* Add a fourth nano of the same size */
    LineInfo li4;
    li4.amount = 98;
    n->addNanoReceivedFromPlayer(li4);

    /* Verify */
    EXPECT_EQ(li1.amount + li2.amount + li3.amount + li4.amount,
              n->getTotalReceived());
    EXPECT_EQ(4, n->getCountReceived());
    EXPECT_EQ(li2.amount, n->getMaxReceived());
    EXPECT_EQ(li3.amount, n->getMinReceived());

    /* Verify values that should not have changed */
    verifyDealtOnPlayerIsZero();
}

TEST(NanoOperatorPlusTest, addToEmptyNano) {

    LineInfo li1;
    LineInfo li2;
    li1.amount = 1999;
    li2.amount = 50000;

    Nano n1;
    Nano n2;
    n2.addNanoReceivedFromPlayer(li1);
    n2.addNanoDealtOnPlayer(li2);

    Nano n3;
    n3 = n1 + n2;

    EXPECT_EQ(li1.amount, n3.getTotalReceived());
    EXPECT_EQ(1, n3.getCountReceived());
    EXPECT_EQ(li1.amount, n3.getMaxReceived());
    EXPECT_EQ(li1.amount, n3.getMinReceived());

    EXPECT_EQ(li2.amount, n3.getTotalDealt());
    EXPECT_EQ(1, n3.getCountDealt());
    EXPECT_EQ(li2.amount, n3.getMaxDealt());
    EXPECT_EQ(li2.amount, n3.getMinDealt());
}
