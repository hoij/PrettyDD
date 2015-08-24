#include "damage.h"
#include "line_info.h"

#include <gtest/gtest.h>
#include <limits>


class DamageTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        d = new Damage;
    }
    virtual void TearDown() {
        delete d;
    }

    Damage* d;
};

void verifyReceivedFromPlayerIsZero(Damage& d) {
    // Used to verify that the LineInfo is not added to the received damage.
    EXPECT_EQ(0, d.getTotalReceived());
    EXPECT_EQ(0, d.getCountReceived());
    EXPECT_EQ(-1, d.getRegularMaxReceived());
    EXPECT_EQ(std::numeric_limits<int>::max(), d.getRegularMinReceived());
    EXPECT_EQ(0, d.getCritTotalReceived());
    EXPECT_EQ(0, d.getCritCountReceived());
    EXPECT_EQ(-1, d.getCritMaxReceived());
    EXPECT_EQ(std::numeric_limits<int>::max(), d.getCritMinReceived());
    EXPECT_EQ(0, d.getDeflectsReceived());
    EXPECT_EQ(0, d.getMissesReceived());
}

TEST_F(DamageTest, addDamageDealtOnPlayer_nonCrit) {

    LineInfo li;
    li.amount = 3000;

    d->addDamageDealtOnPlayer(li);

    EXPECT_EQ(li.amount, d->getTotalDealt());
    EXPECT_EQ(1, d->getCountDealt());
    EXPECT_EQ(li.amount, d->getRegularMaxDealt());
    EXPECT_EQ(li.amount, d->getRegularMinDealt());
    EXPECT_EQ(0, d->getCritTotalDealt());
    EXPECT_EQ(0, d->getCritCountDealt());
    EXPECT_EQ(-1, d->getCritMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getCritMinDealt());
    EXPECT_EQ(0, d->getDeflectsDealt());
    EXPECT_EQ(0, d->getMissesDealt());

    verifyReceivedFromPlayerIsZero(*d);
}

TEST_F(DamageTest, addDamageDealtOnPlayer_nonCritNewMin) {

    LineInfo li1;
    LineInfo li2;
    li1.amount = 3000;
    li2.amount = 2999;

    d->addDamageDealtOnPlayer(li1);
    d->addDamageDealtOnPlayer(li2);

    EXPECT_EQ(li1.amount + li2.amount, d->getTotalDealt());
    EXPECT_EQ(2, d->getCountDealt());
    EXPECT_EQ(li1.amount, d->getRegularMaxDealt());
    EXPECT_EQ(li2.amount, d->getRegularMinDealt());
    EXPECT_EQ(0, d->getCritTotalDealt());
    EXPECT_EQ(0, d->getCritCountDealt());
    EXPECT_EQ(-1, d->getCritMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getCritMinDealt());
    EXPECT_EQ(0, d->getDeflectsDealt());
    EXPECT_EQ(0, d->getMissesDealt());
}

TEST_F(DamageTest, addDamageDealtOnPlayer_nonCritNewMax) {

    LineInfo li1;
    LineInfo li2;
    li1.amount = 3000;
    li2.amount = 3001;

    d->addDamageDealtOnPlayer(li1);
    d->addDamageDealtOnPlayer(li2);

    EXPECT_EQ(li1.amount + li2.amount, d->getTotalDealt());
    EXPECT_EQ(2, d->getCountDealt());
    EXPECT_EQ(li2.amount, d->getRegularMaxDealt());
    EXPECT_EQ(li1.amount, d->getRegularMinDealt());
    EXPECT_EQ(0, d->getCritTotalDealt());
    EXPECT_EQ(0, d->getCritCountDealt());
    EXPECT_EQ(-1, d->getCritMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getCritMinDealt());
    EXPECT_EQ(0, d->getDeflectsDealt());
    EXPECT_EQ(0, d->getMissesDealt());
}

TEST_F(DamageTest, addDamageDealtOnPlayer_twoMisses) {

    LineInfo li;
    li.miss = true;

    d->addDamageDealtOnPlayer(li);
    d->addDamageDealtOnPlayer(li);

    EXPECT_EQ(li.amount, d->getTotalDealt());
    EXPECT_EQ(2, d->getCountDealt());
    EXPECT_EQ(-1, d->getRegularMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getRegularMinDealt());
    EXPECT_EQ(0, d->getCritTotalDealt());
    EXPECT_EQ(0, d->getCritCountDealt());
    EXPECT_EQ(-1, d->getCritMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getCritMinDealt());
    EXPECT_EQ(0, d->getDeflectsDealt());
    EXPECT_EQ(2, d->getMissesDealt());

    verifyReceivedFromPlayerIsZero(*d);
}

TEST_F(DamageTest, addDamageDealtOnPlayer_twoDeflects) {

    LineInfo li;
    li.deflect = true;

    d->addDamageDealtOnPlayer(li);
    d->addDamageDealtOnPlayer(li);

    EXPECT_EQ(li.amount, d->getTotalDealt());
    EXPECT_EQ(2, d->getCountDealt());
    EXPECT_EQ(-1, d->getRegularMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getRegularMinDealt());
    EXPECT_EQ(0, d->getCritTotalDealt());
    EXPECT_EQ(0, d->getCritCountDealt());
    EXPECT_EQ(-1, d->getCritMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getCritMinDealt());
    EXPECT_EQ(2, d->getDeflectsDealt());
    EXPECT_EQ(0, d->getMissesDealt());

    verifyReceivedFromPlayerIsZero(*d);
}

TEST_F(DamageTest, addDamageDealtOnPlayer_crit) {

    LineInfo li;
    li.amount = 3000;
    li.crit = true;

    d->addDamageDealtOnPlayer(li);

    EXPECT_EQ(li.amount, d->getTotalDealt());
    EXPECT_EQ(1, d->getCountDealt());
    EXPECT_EQ(-1, d->getRegularMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getRegularMinDealt());
    EXPECT_EQ(li.amount, d->getCritTotalDealt());
    EXPECT_EQ(1, d->getCritCountDealt());
    EXPECT_EQ(li.amount, d->getCritMaxDealt());
    EXPECT_EQ(li.amount, d->getCritMinDealt());
    EXPECT_EQ(0, d->getDeflectsDealt());
    EXPECT_EQ(0, d->getMissesDealt());

    verifyReceivedFromPlayerIsZero(*d);
}

TEST_F(DamageTest, addDamageDealtOnPlayer_critNewMin) {

    LineInfo li1;
    LineInfo li2;
    li1.amount = 1;
    li2.amount = 0;
    li1.crit = true;
    li2.crit = true;

    d->addDamageDealtOnPlayer(li1);
    d->addDamageDealtOnPlayer(li2);

    EXPECT_EQ(li1.amount + li2.amount, d->getTotalDealt());
    EXPECT_EQ(2, d->getCountDealt());
    EXPECT_EQ(-1, d->getRegularMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getRegularMinDealt());
    EXPECT_EQ(li1.amount + li2.amount, d->getCritTotalDealt());
    EXPECT_EQ(2, d->getCritCountDealt());
    EXPECT_EQ(li1.amount, d->getCritMaxDealt());
    EXPECT_EQ(li2.amount, d->getCritMinDealt());
    EXPECT_EQ(0, d->getDeflectsDealt());
    EXPECT_EQ(0, d->getMissesDealt());
}

TEST_F(DamageTest, addDamageDealtOnPlayer_critNewMax) {

    LineInfo li1;
    LineInfo li2;
    li1.amount = 99;
    li2.amount = 100;
    li1.crit = true;
    li2.crit = true;

    d->addDamageDealtOnPlayer(li1);
    d->addDamageDealtOnPlayer(li2);

    EXPECT_EQ(li1.amount + li2.amount, d->getTotalDealt());
    EXPECT_EQ(2, d->getCountDealt());
    EXPECT_EQ(-1, d->getRegularMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getRegularMinDealt());
    EXPECT_EQ(li1.amount + li2.amount, d->getCritTotalDealt());
    EXPECT_EQ(2, d->getCritCountDealt());
    EXPECT_EQ(li2.amount, d->getCritMaxDealt());
    EXPECT_EQ(li1.amount, d->getCritMinDealt());
    EXPECT_EQ(0, d->getDeflectsDealt());
    EXPECT_EQ(0, d->getMissesDealt());
}

TEST_F(DamageTest, addDamageReceivedFromPlayer_nonCrit) {
    /* Because addDamageDealt and addDamageReceived uses the same logic,
    addDamageDealt is tested more thoroughly */

    LineInfo li;
    li.amount = 500001;

    d->addDamageReceivedFromPlayer(li);

    EXPECT_EQ(li.amount, d->getTotalReceived());
    EXPECT_EQ(1, d->getCountReceived());
    EXPECT_EQ(li.amount, d->getRegularMaxReceived());
    EXPECT_EQ(li.amount, d->getRegularMinReceived());
    EXPECT_EQ(0, d->getCritTotalReceived());
    EXPECT_EQ(0, d->getCritCountReceived());
    EXPECT_EQ(-1, d->getCritMaxReceived());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getCritMinReceived());
    EXPECT_EQ(0, d->getDeflectsReceived());
    EXPECT_EQ(0, d->getMissesReceived());

    // Verify that the LineInfo is not added to the dealt damage.
    EXPECT_EQ(0, d->getTotalDealt());
    EXPECT_EQ(0, d->getCountDealt());
    EXPECT_EQ(-1, d->getRegularMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getRegularMinDealt());
    EXPECT_EQ(0, d->getCritTotalDealt());
    EXPECT_EQ(0, d->getCritCountDealt());
    EXPECT_EQ(-1, d->getCritMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getCritMinDealt());
    EXPECT_EQ(0, d->getDeflectsDealt());
    EXPECT_EQ(0, d->getMissesDealt());
}

TEST(DamageOperatorPlusTest, nonCritNewMax) {

    LineInfo li1;
    LineInfo li2;
    li1.amount = 14800;
    li2.amount = 15000;

    Damage d1;
    Damage d2;
    d1.addDamageDealtOnPlayer(li1);
    d2.addDamageDealtOnPlayer(li2);

    Damage d3;
    d3 = d1 + d2;

    EXPECT_EQ(li1.amount + li2.amount, d3.getTotalDealt());
    EXPECT_EQ(2, d3.getCountDealt());
    EXPECT_EQ(li2.amount, d3.getRegularMaxDealt());
    EXPECT_EQ(li1.amount, d3.getRegularMinDealt());
    EXPECT_EQ(0, d3.getCritTotalDealt());
    EXPECT_EQ(0, d3.getCritCountDealt());
    EXPECT_EQ(-1, d3.getCritMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getCritMinDealt());
    EXPECT_EQ(0, d3.getDeflectsDealt());
    EXPECT_EQ(0, d3.getMissesDealt());

    verifyReceivedFromPlayerIsZero(d3);
}

TEST(DamageOperatorPlusTest, addNonCritToEmptyDamage) {

    LineInfo li2;
    li2.amount = 1999;

    Damage d1;
    Damage d2;
    d2.addDamageDealtOnPlayer(li2);

    Damage d3;
    d3 = d1 + d2;

    EXPECT_EQ(li2.amount, d3.getTotalDealt());
    EXPECT_EQ(1, d3.getCountDealt());
    EXPECT_EQ(li2.amount, d3.getRegularMaxDealt());
    EXPECT_EQ(li2.amount, d3.getRegularMinDealt());
    EXPECT_EQ(0, d3.getCritTotalDealt());
    EXPECT_EQ(0, d3.getCritCountDealt());
    EXPECT_EQ(-1, d3.getCritMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getCritMinDealt());
    EXPECT_EQ(0, d3.getDeflectsDealt());
    EXPECT_EQ(0, d3.getMissesDealt());

    verifyReceivedFromPlayerIsZero(d3);
}

TEST(DamageOperatorPlusTest, critNewMax) {

    LineInfo li1;
    LineInfo li2;
    li1.amount = 7999;
    li2.amount = 8000;
    li1.crit = true;
    li2.crit = true;

    Damage d1;
    Damage d2;
    d1.addDamageDealtOnPlayer(li1);
    d2.addDamageDealtOnPlayer(li2);

    Damage d3;
    d3 = d1 + d2;

    EXPECT_EQ(li1.amount + li2.amount, d3.getTotalDealt());
    EXPECT_EQ(2, d3.getCountDealt());
    EXPECT_EQ(-1, d3.getRegularMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getRegularMinDealt());
    EXPECT_EQ(li1.amount + li2.amount, d3.getCritTotalDealt());
    EXPECT_EQ(2, d3.getCritCountDealt());
    EXPECT_EQ(8000, d3.getCritMaxDealt());
    EXPECT_EQ(7999, d3.getCritMinDealt());
    EXPECT_EQ(0, d3.getDeflectsDealt());
    EXPECT_EQ(0, d3.getMissesDealt());

    verifyReceivedFromPlayerIsZero(d3);
}

TEST(DamageOperatorPlusTest, addCritToEmptyDamage) {

    LineInfo li2;
    li2.amount = 14;
    li2.crit = true;

    Damage d1;
    Damage d2;
    d2.addDamageDealtOnPlayer(li2);

    Damage d3;
    d3 = d1 + d2;

    EXPECT_EQ(li2.amount, d3.getTotalDealt());
    EXPECT_EQ(1, d3.getCountDealt());
    EXPECT_EQ(-1, d3.getRegularMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getRegularMinDealt());
    EXPECT_EQ(li2.amount, d3.getCritTotalDealt());
    EXPECT_EQ(1, d3.getCritCountDealt());
    EXPECT_EQ(14, d3.getCritMaxDealt());
    EXPECT_EQ(14, d3.getCritMinDealt());
    EXPECT_EQ(0, d3.getDeflectsDealt());
    EXPECT_EQ(0, d3.getMissesDealt());

    verifyReceivedFromPlayerIsZero(d3);
}

TEST(DamageOperatorPlusTest, addMiss) {

    LineInfo li1;
    LineInfo li2;
    li1.miss = true;
    li2.miss = true;

    Damage d1;
    Damage d2;
    d1.addDamageDealtOnPlayer(li1);
    d2.addDamageDealtOnPlayer(li2);

    Damage d3;
    d3 = d1 + d2;

    EXPECT_EQ(0, d3.getTotalDealt());
    EXPECT_EQ(2, d3.getCountDealt());
    EXPECT_EQ(-1, d3.getRegularMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getRegularMinDealt());
    EXPECT_EQ(0, d3.getCritTotalDealt());
    EXPECT_EQ(0, d3.getCritCountDealt());
    EXPECT_EQ(-1, d3.getCritMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getCritMinDealt());
    EXPECT_EQ(0, d3.getDeflectsDealt());
    EXPECT_EQ(2, d3.getMissesDealt());

    verifyReceivedFromPlayerIsZero(d3);
}

TEST(DamageOperatorPlusTest, addDeflect) {

    LineInfo li1;
    LineInfo li2;
    li1.deflect = true;
    li2.deflect = true;

    Damage d1;
    Damage d2;
    d1.addDamageDealtOnPlayer(li1);
    d2.addDamageDealtOnPlayer(li2);

    Damage d3;
    d3 = d1 + d2;

    EXPECT_EQ(0, d3.getTotalDealt());
    EXPECT_EQ(2, d3.getCountDealt());
    EXPECT_EQ(-1, d3.getRegularMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getRegularMinDealt());
    EXPECT_EQ(0, d3.getCritTotalDealt());
    EXPECT_EQ(0, d3.getCritCountDealt());
    EXPECT_EQ(-1, d3.getCritMaxDealt());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getCritMinDealt());
    EXPECT_EQ(2, d3.getDeflectsDealt());
    EXPECT_EQ(0, d3.getMissesDealt());

    verifyReceivedFromPlayerIsZero(d3);
}
