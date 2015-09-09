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
    EXPECT_EQ(0, d.getTotalReceivedFromPlayer());
    EXPECT_EQ(0, d.getCountReceivedFromPlayer());
    EXPECT_EQ(0, d.getRegularTotalReceivedFromPlayer());
    EXPECT_EQ(0, d.getRegularCountReceivedFromPlayer());
    EXPECT_EQ(-1, d.getRegularMaxReceivedFromPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d.getRegularMinReceivedFromPlayer());
    EXPECT_EQ(0, d.getNanobotTotalReceivedFromPlayer());
    EXPECT_EQ(0, d.getNanobotCountReceivedFromPlayer());
    EXPECT_EQ(-1, d.getNanobotMaxReceivedFromPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d.getRegularMinReceivedFromPlayer());
    EXPECT_EQ(0, d.getCritTotalReceivedFromPlayer());
    EXPECT_EQ(0, d.getCritCountReceivedFromPlayer());
    EXPECT_EQ(-1, d.getCritMaxReceivedFromPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d.getCritMinReceivedFromPlayer());
    EXPECT_EQ(0, d.getDeflectsReceivedFromPlayer());
    EXPECT_EQ(0, d.getMissesReceivedFromPlayer());
}


TEST_F(DamageTest, addDamageDealtOnPlayer_regularNonCrit) {

    LineInfo li1;
    LineInfo li2;
    LineInfo li3;
    li1.amount = 3000;
    li2.amount = 2999;
    li3.amount = 3001;

    d->addDamageDealtOnPlayer(li1);
    d->addDamageDealtOnPlayer(li2);
    d->addDamageDealtOnPlayer(li3);

    EXPECT_EQ(li1.amount + li2.amount + li3.amount,
              d->getTotalDealtOnPlayer());
    EXPECT_EQ(3, d->getCountDealtOnPlayer());
    EXPECT_EQ(li1.amount + li2.amount + li3.amount,
              d->getRegularTotalDealtOnPlayer());
    EXPECT_EQ(3, d->getRegularCountDealtOnPlayer());
    EXPECT_EQ(li3.amount, d->getRegularMaxDealtOnPlayer());
    EXPECT_EQ(li2.amount, d->getRegularMinDealtOnPlayer());
    // Default values:
    EXPECT_EQ(0, d->getNanobotTotalDealtOnPlayer());
    EXPECT_EQ(0, d->getNanobotCountDealtOnPlayer());
    EXPECT_EQ(-1, d->getNanobotMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getNanobotMinDealtOnPlayer());
    EXPECT_EQ(0, d->getCritTotalDealtOnPlayer());
    EXPECT_EQ(0, d->getCritCountDealtOnPlayer());
    EXPECT_EQ(-1, d->getCritMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getCritMinDealtOnPlayer());
    EXPECT_EQ(0, d->getDeflectsDealtOnPlayer());
    EXPECT_EQ(0, d->getMissesDealtOnPlayer());

    verifyReceivedFromPlayerIsZero(*d);
}

TEST_F(DamageTest, addDamageDealtOnPlayer_nanobots) {

    LineInfo li1;
    LineInfo li2;
    LineInfo li3;
    li1.amount = 3000;
    li2.amount = 2999;
    li3.amount = 3001;
    li1.nanobots = true;
    li2.nanobots = true;
    li3.nanobots = true;

    d->addDamageDealtOnPlayer(li1);
    d->addDamageDealtOnPlayer(li2);
    d->addDamageDealtOnPlayer(li3);

    EXPECT_EQ(li1.amount + li2.amount + li3.amount,
              d->getTotalDealtOnPlayer());
    EXPECT_EQ(3, d->getCountDealtOnPlayer());
    EXPECT_EQ(0, d->getRegularTotalDealtOnPlayer());
    EXPECT_EQ(0, d->getRegularCountDealtOnPlayer());
    EXPECT_EQ(-1, d->getRegularMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getRegularMinDealtOnPlayer());
    // Default values:
    EXPECT_EQ(li1.amount + li2.amount + li3.amount,
              d->getNanobotTotalDealtOnPlayer());
    EXPECT_EQ(3, d->getNanobotCountDealtOnPlayer());
    EXPECT_EQ(li3.amount, d->getNanobotMaxDealtOnPlayer());
    EXPECT_EQ(li2.amount, d->getNanobotMinDealtOnPlayer());
    EXPECT_EQ(0, d->getCritTotalDealtOnPlayer());
    EXPECT_EQ(0, d->getCritCountDealtOnPlayer());
    EXPECT_EQ(-1, d->getCritMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getCritMinDealtOnPlayer());
    EXPECT_EQ(0, d->getDeflectsDealtOnPlayer());
    EXPECT_EQ(0, d->getMissesDealtOnPlayer());

    verifyReceivedFromPlayerIsZero(*d);
}

TEST_F(DamageTest, addDamageDealtOnPlayer_crit) {

    LineInfo li1;
    LineInfo li2;
    LineInfo li3;
    li1.amount = 3000;
    li2.amount = 2999;
    li3.amount = 3001;
    li1.crit = true;
    li2.crit = true;
    li3.crit = true;

    d->addDamageDealtOnPlayer(li1);
    d->addDamageDealtOnPlayer(li2);
    d->addDamageDealtOnPlayer(li3);

    EXPECT_EQ(li1.amount + li2.amount + li3.amount,
              d->getTotalDealtOnPlayer());
    EXPECT_EQ(3, d->getCountDealtOnPlayer());
    EXPECT_EQ(0, d->getRegularTotalDealtOnPlayer());
    EXPECT_EQ(0, d->getRegularCountDealtOnPlayer());
    EXPECT_EQ(-1, d->getRegularMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getRegularMinDealtOnPlayer());
    EXPECT_EQ(0, d->getNanobotTotalDealtOnPlayer());
    EXPECT_EQ(0, d->getNanobotCountDealtOnPlayer());
    EXPECT_EQ(-1, d->getNanobotMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getNanobotMinDealtOnPlayer());
    EXPECT_EQ(li1.amount + li2.amount + li3.amount,
              d->getCritTotalDealtOnPlayer());
    EXPECT_EQ(3, d->getCritCountDealtOnPlayer());
    EXPECT_EQ(li3.amount, d->getCritMaxDealtOnPlayer());
    EXPECT_EQ(li2.amount, d->getCritMinDealtOnPlayer());
    EXPECT_EQ(0, d->getDeflectsDealtOnPlayer());
    EXPECT_EQ(0, d->getMissesDealtOnPlayer());

    verifyReceivedFromPlayerIsZero(*d);
}

TEST_F(DamageTest, addDamageDealtOnPlayer_twoMisses) {

    LineInfo li;
    li.miss = true;

    d->addDamageDealtOnPlayer(li);
    d->addDamageDealtOnPlayer(li);

    EXPECT_EQ(0, d->getTotalDealtOnPlayer());
    EXPECT_EQ(2, d->getCountDealtOnPlayer());
    // Default values:
    EXPECT_EQ(0, d->getRegularTotalDealtOnPlayer());
    EXPECT_EQ(0, d->getRegularCountDealtOnPlayer());
    EXPECT_EQ(-1, d->getRegularMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getRegularMinDealtOnPlayer());
    EXPECT_EQ(0, d->getNanobotTotalDealtOnPlayer());
    EXPECT_EQ(0, d->getNanobotCountDealtOnPlayer());
    EXPECT_EQ(-1, d->getNanobotMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getNanobotMinDealtOnPlayer());
    EXPECT_EQ(0, d->getCritTotalDealtOnPlayer());
    EXPECT_EQ(0, d->getCritCountDealtOnPlayer());
    EXPECT_EQ(-1, d->getCritMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getCritMinDealtOnPlayer());
    EXPECT_EQ(0, d->getDeflectsDealtOnPlayer());
    // Misses
    EXPECT_EQ(2, d->getMissesDealtOnPlayer());

    verifyReceivedFromPlayerIsZero(*d);
}

TEST_F(DamageTest, addDamageDealtOnPlayer_twoDeflects) {

    LineInfo li;
    li.deflect = true;

    d->addDamageDealtOnPlayer(li);
    d->addDamageDealtOnPlayer(li);

    EXPECT_EQ(li.amount, d->getTotalDealtOnPlayer());
    EXPECT_EQ(2, d->getCountDealtOnPlayer());
    // Default values:
    EXPECT_EQ(0, d->getRegularTotalDealtOnPlayer());
    EXPECT_EQ(0, d->getRegularCountDealtOnPlayer());
    EXPECT_EQ(-1, d->getRegularMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getRegularMinDealtOnPlayer());
    EXPECT_EQ(0, d->getNanobotTotalDealtOnPlayer());
    EXPECT_EQ(0, d->getNanobotCountDealtOnPlayer());
    EXPECT_EQ(-1, d->getNanobotMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getNanobotMinDealtOnPlayer());
    EXPECT_EQ(0, d->getCritTotalDealtOnPlayer());
    EXPECT_EQ(0, d->getCritCountDealtOnPlayer());
    EXPECT_EQ(-1, d->getCritMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getCritMinDealtOnPlayer());
    // Deflects
    EXPECT_EQ(2, d->getDeflectsDealtOnPlayer());
    // Default values:
    EXPECT_EQ(0, d->getMissesDealtOnPlayer());

    verifyReceivedFromPlayerIsZero(*d);
}

TEST_F(DamageTest, addDamageReceivedFromPlayer_nonCrit) {
    /* Because addDamageDealt and addDamageReceived uses the same logic,
    addDamageDealt is tested more thoroughly */

    LineInfo li;
    li.amount = 500001;

    d->addDamageReceivedFromPlayer(li);

    EXPECT_EQ(li.amount, d->getTotalReceivedFromPlayer());
    EXPECT_EQ(1, d->getCountReceivedFromPlayer());
    EXPECT_EQ(li.amount, d->getRegularTotalReceivedFromPlayer());
    EXPECT_EQ(1, d->getRegularCountReceivedFromPlayer());
    EXPECT_EQ(li.amount, d->getRegularMaxReceivedFromPlayer());
    EXPECT_EQ(li.amount, d->getRegularMinReceivedFromPlayer());
    EXPECT_EQ(0, d->getNanobotTotalReceivedFromPlayer());
    EXPECT_EQ(0, d->getNanobotCountReceivedFromPlayer());
    EXPECT_EQ(-1, d->getNanobotMaxReceivedFromPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getNanobotMinReceivedFromPlayer());
    EXPECT_EQ(0, d->getCritTotalReceivedFromPlayer());
    EXPECT_EQ(0, d->getCritCountReceivedFromPlayer());
    EXPECT_EQ(-1, d->getCritMaxReceivedFromPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getCritMinReceivedFromPlayer());
    EXPECT_EQ(0, d->getDeflectsReceivedFromPlayer());
    EXPECT_EQ(0, d->getMissesReceivedFromPlayer());

    // Verify that the LineInfo is not added to the dealt damage.
    EXPECT_EQ(0, d->getTotalDealtOnPlayer());
    EXPECT_EQ(0, d->getCountDealtOnPlayer());
    EXPECT_EQ(0, d->getRegularTotalDealtOnPlayer());
    EXPECT_EQ(0, d->getRegularCountDealtOnPlayer());
    EXPECT_EQ(-1, d->getRegularMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getRegularMinDealtOnPlayer());
    EXPECT_EQ(0, d->getNanobotTotalDealtOnPlayer());
    EXPECT_EQ(0, d->getNanobotCountDealtOnPlayer());
    EXPECT_EQ(-1, d->getNanobotMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getNanobotMinDealtOnPlayer());
    EXPECT_EQ(0, d->getCritTotalDealtOnPlayer());
    EXPECT_EQ(0, d->getCritCountDealtOnPlayer());
    EXPECT_EQ(-1, d->getCritMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getCritMinDealtOnPlayer());
    EXPECT_EQ(0, d->getDeflectsDealtOnPlayer());
    EXPECT_EQ(0, d->getMissesDealtOnPlayer());
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

    EXPECT_EQ(li1.amount + li2.amount, d3.getTotalDealtOnPlayer());
    EXPECT_EQ(2, d3.getCountDealtOnPlayer());
    EXPECT_EQ(li1.amount + li2.amount, d3.getRegularTotalDealtOnPlayer());
    EXPECT_EQ(2, d3.getRegularCountDealtOnPlayer());
    EXPECT_EQ(li2.amount, d3.getRegularMaxDealtOnPlayer());
    EXPECT_EQ(li1.amount, d3.getRegularMinDealtOnPlayer());
    // Default values:
    EXPECT_EQ(0, d3.getNanobotTotalDealtOnPlayer());
    EXPECT_EQ(0, d3.getNanobotCountDealtOnPlayer());
    EXPECT_EQ(-1, d3.getNanobotMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getNanobotMinDealtOnPlayer());
    EXPECT_EQ(0, d3.getCritTotalDealtOnPlayer());
    EXPECT_EQ(0, d3.getCritCountDealtOnPlayer());
    EXPECT_EQ(-1, d3.getCritMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getCritMinDealtOnPlayer());
    EXPECT_EQ(0, d3.getDeflectsDealtOnPlayer());
    EXPECT_EQ(0, d3.getMissesDealtOnPlayer());

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

    EXPECT_EQ(li2.amount, d3.getTotalDealtOnPlayer());
    EXPECT_EQ(1, d3.getCountDealtOnPlayer());
    EXPECT_EQ(li2.amount, d3.getRegularTotalDealtOnPlayer());
    EXPECT_EQ(1, d3.getRegularCountDealtOnPlayer());
    EXPECT_EQ(li2.amount, d3.getRegularMaxDealtOnPlayer());
    EXPECT_EQ(li2.amount, d3.getRegularMinDealtOnPlayer());
    // Default values:
    EXPECT_EQ(0, d3.getNanobotTotalDealtOnPlayer());
    EXPECT_EQ(0, d3.getNanobotCountDealtOnPlayer());
    EXPECT_EQ(-1, d3.getNanobotMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getNanobotMinDealtOnPlayer());
    EXPECT_EQ(0, d3.getCritTotalDealtOnPlayer());
    EXPECT_EQ(0, d3.getCritCountDealtOnPlayer());
    EXPECT_EQ(-1, d3.getCritMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getCritMinDealtOnPlayer());
    EXPECT_EQ(0, d3.getDeflectsDealtOnPlayer());
    EXPECT_EQ(0, d3.getMissesDealtOnPlayer());

    verifyReceivedFromPlayerIsZero(d3);
}

TEST(DamageOperatorPlusTest, nanobots) {

    LineInfo li1;
    LineInfo li2;
    LineInfo li3;
    li1.amount = 14800;
    li2.amount = 15000;
    li3.amount = 15001;
    li1.nanobots = true;
    li2.nanobots = true;
    li3.nanobots = true;

    Damage d1;
    Damage d2;
    Damage d3;
    d1.addDamageDealtOnPlayer(li1);
    d2.addDamageDealtOnPlayer(li2);
    d3.addDamageDealtOnPlayer(li3);

    Damage d4;
    d4 = d1 + d2 + d3;

    EXPECT_EQ(li1.amount + li2.amount + li3.amount,
              d4.getTotalDealtOnPlayer());
    EXPECT_EQ(3, d4.getCountDealtOnPlayer());
    EXPECT_EQ(0, d4.getRegularTotalDealtOnPlayer());
    EXPECT_EQ(0, d4.getRegularCountDealtOnPlayer());
    EXPECT_EQ(-1, d4.getRegularMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(),
              d4.getRegularMinDealtOnPlayer());
    EXPECT_EQ(li1.amount + li2.amount + li3.amount,
              d4.getNanobotTotalDealtOnPlayer());
    EXPECT_EQ(3, d4.getNanobotCountDealtOnPlayer());
    EXPECT_EQ(li3.amount, d4.getNanobotMaxDealtOnPlayer());
    EXPECT_EQ(li1.amount, d4.getNanobotMinDealtOnPlayer());
    EXPECT_EQ(0, d4.getCritTotalDealtOnPlayer());
    EXPECT_EQ(0, d4.getCritCountDealtOnPlayer());
    EXPECT_EQ(-1, d4.getCritMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d4.getCritMinDealtOnPlayer());
    EXPECT_EQ(0, d4.getDeflectsDealtOnPlayer());
    EXPECT_EQ(0, d4.getMissesDealtOnPlayer());

    verifyReceivedFromPlayerIsZero(d4);
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

    EXPECT_EQ(li1.amount + li2.amount, d3.getTotalDealtOnPlayer());
    EXPECT_EQ(2, d3.getCountDealtOnPlayer());
    EXPECT_EQ(0, d3.getRegularTotalDealtOnPlayer());
    EXPECT_EQ(0, d3.getRegularCountDealtOnPlayer());
    EXPECT_EQ(-1, d3.getRegularMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getRegularMinDealtOnPlayer());
    EXPECT_EQ(0, d3.getNanobotTotalDealtOnPlayer());
    EXPECT_EQ(0, d3.getNanobotCountDealtOnPlayer());
    EXPECT_EQ(-1, d3.getNanobotMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getNanobotMinDealtOnPlayer());
    EXPECT_EQ(li1.amount + li2.amount, d3.getCritTotalDealtOnPlayer());
    EXPECT_EQ(2, d3.getCritCountDealtOnPlayer());
    EXPECT_EQ(8000, d3.getCritMaxDealtOnPlayer());
    EXPECT_EQ(7999, d3.getCritMinDealtOnPlayer());
    EXPECT_EQ(0, d3.getDeflectsDealtOnPlayer());
    EXPECT_EQ(0, d3.getMissesDealtOnPlayer());

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

    EXPECT_EQ(li2.amount, d3.getTotalDealtOnPlayer());
    EXPECT_EQ(1, d3.getCountDealtOnPlayer());
    EXPECT_EQ(0, d3.getRegularTotalDealtOnPlayer());
    EXPECT_EQ(0, d3.getRegularCountDealtOnPlayer());
    EXPECT_EQ(-1, d3.getRegularMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getRegularMinDealtOnPlayer());
    EXPECT_EQ(li2.amount, d3.getCritTotalDealtOnPlayer());
    EXPECT_EQ(0, d3.getNanobotTotalDealtOnPlayer());
    EXPECT_EQ(0, d3.getNanobotCountDealtOnPlayer());
    EXPECT_EQ(-1, d3.getNanobotMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getNanobotMinDealtOnPlayer());
    EXPECT_EQ(1, d3.getCritCountDealtOnPlayer());
    EXPECT_EQ(14, d3.getCritMaxDealtOnPlayer());
    EXPECT_EQ(14, d3.getCritMinDealtOnPlayer());
    EXPECT_EQ(0, d3.getDeflectsDealtOnPlayer());
    EXPECT_EQ(0, d3.getMissesDealtOnPlayer());

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

    EXPECT_EQ(0, d3.getTotalDealtOnPlayer());
    EXPECT_EQ(2, d3.getCountDealtOnPlayer());
    EXPECT_EQ(0, d3.getRegularTotalDealtOnPlayer());
    EXPECT_EQ(0, d3.getRegularCountDealtOnPlayer());
    EXPECT_EQ(-1, d3.getRegularMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getRegularMinDealtOnPlayer());
    EXPECT_EQ(0, d3.getNanobotTotalDealtOnPlayer());
    EXPECT_EQ(0, d3.getNanobotCountDealtOnPlayer());
    EXPECT_EQ(-1, d3.getNanobotMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getNanobotMinDealtOnPlayer());
    EXPECT_EQ(0, d3.getCritTotalDealtOnPlayer());
    EXPECT_EQ(0, d3.getCritCountDealtOnPlayer());
    EXPECT_EQ(-1, d3.getCritMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getCritMinDealtOnPlayer());
    EXPECT_EQ(0, d3.getDeflectsDealtOnPlayer());
    EXPECT_EQ(2, d3.getMissesDealtOnPlayer());

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

    EXPECT_EQ(0, d3.getTotalDealtOnPlayer());
    EXPECT_EQ(2, d3.getCountDealtOnPlayer());
    EXPECT_EQ(0, d3.getRegularTotalDealtOnPlayer());
    EXPECT_EQ(0, d3.getRegularCountDealtOnPlayer());
    EXPECT_EQ(-1, d3.getRegularMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getRegularMinDealtOnPlayer());
    EXPECT_EQ(0, d3.getNanobotTotalDealtOnPlayer());
    EXPECT_EQ(0, d3.getNanobotCountDealtOnPlayer());
    EXPECT_EQ(-1, d3.getNanobotMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getNanobotMinDealtOnPlayer());
    EXPECT_EQ(0, d3.getCritTotalDealtOnPlayer());
    EXPECT_EQ(0, d3.getCritCountDealtOnPlayer());
    EXPECT_EQ(-1, d3.getCritMaxDealtOnPlayer());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getCritMinDealtOnPlayer());
    EXPECT_EQ(2, d3.getDeflectsDealtOnPlayer());
    EXPECT_EQ(0, d3.getMissesDealtOnPlayer());

    verifyReceivedFromPlayerIsZero(d3);
}
