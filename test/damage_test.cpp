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

TEST_F(DamageTest, add_normalNonCrit) {

    LineInfo li1;
    LineInfo li2;
    LineInfo li3;
    li1.amount = 3000;
    li2.amount = 2999;
    li3.amount = 3001;

    d->add(li1);
    d->add(li2);
    d->add(li3);

    // Values that should have changed:
    EXPECT_EQ(li1.amount + li2.amount + li3.amount,
              d->getTotal());
    EXPECT_EQ(3, d->getCount());
    EXPECT_EQ(li1.amount + li2.amount + li3.amount,
              d->getNormalTotal());
    EXPECT_EQ(3, d->getNormalCount());
    EXPECT_EQ(li3.amount, d->getNormalMax());
    EXPECT_EQ(li2.amount, d->getNormalMin());
    // Default values:
    EXPECT_EQ(0, d->getRegularDeflectTotal());
    EXPECT_EQ(0, d->getRegularDeflectCount());
    EXPECT_EQ(-1, d->getRegularDeflectMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getRegularDeflectMin());
    EXPECT_EQ(0, d->getSpecialTotal());
    EXPECT_EQ(0, d->getSpecialCount());
    EXPECT_EQ(-1, d->getSpecialMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getSpecialMin());
    EXPECT_EQ(0, d->getSpecialDeflectTotal());
    EXPECT_EQ(0, d->getSpecialDeflectCount());
    EXPECT_EQ(-1, d->getSpecialDeflectMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getSpecialDeflectMin());
    EXPECT_EQ(0, d->getNanobotTotal());
    EXPECT_EQ(0, d->getNanobotCount());
    EXPECT_EQ(-1, d->getNanobotMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getNanobotMin());
    EXPECT_EQ(0, d->getCritTotal());
    EXPECT_EQ(0, d->getCritCount());
    EXPECT_EQ(-1, d->getCritMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getCritMin());
    EXPECT_EQ(0, d->getShieldTotal());
    EXPECT_EQ(0, d->getShieldCount());
    EXPECT_EQ(-1, d->getShieldMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getShieldMin());
    EXPECT_EQ(0, d->getRegularMisses());
    EXPECT_EQ(0, d->getSpecialMisses());
}

TEST_F(DamageTest, add_nanobots) {

    LineInfo li1;
    LineInfo li2;
    LineInfo li3;
    li1.amount = 3000;
    li2.amount = 2999;
    li3.amount = 3001;
    li1.nanobots = true;
    li2.nanobots = true;
    li3.nanobots = true;

    d->add(li1);
    d->add(li2);
    d->add(li3);

    // Values that should have changed:
    EXPECT_EQ(li1.amount + li2.amount + li3.amount,
              d->getTotal());
    EXPECT_EQ(3, d->getCount());
    // Default values:
    EXPECT_EQ(0, d->getNormalTotal());
    EXPECT_EQ(0, d->getNormalCount());
    EXPECT_EQ(-1, d->getNormalMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getNormalMin());
    EXPECT_EQ(0, d->getRegularDeflectTotal());
    EXPECT_EQ(0, d->getRegularDeflectCount());
    EXPECT_EQ(-1, d->getRegularDeflectMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getRegularDeflectMin());
    // Values that should have changed:
    EXPECT_EQ(li1.amount + li2.amount + li3.amount,
              d->getNanobotTotal());
    EXPECT_EQ(3, d->getNanobotCount());
    EXPECT_EQ(li3.amount, d->getNanobotMax());
    EXPECT_EQ(li2.amount, d->getNanobotMin());
    // Default values:
    EXPECT_EQ(0, d->getCritTotal());
    EXPECT_EQ(0, d->getCritCount());
    EXPECT_EQ(-1, d->getCritMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getCritMin());
    EXPECT_EQ(0, d->getRegularMisses());
    EXPECT_EQ(0, d->getSpecialMisses());
}

TEST_F(DamageTest, add_crit) {

    LineInfo li1;
    LineInfo li2;
    LineInfo li3;
    li1.amount = 3000;
    li2.amount = 2999;
    li3.amount = 3001;
    li1.crit = true;
    li2.crit = true;
    li3.crit = true;

    d->add(li1);
    d->add(li2);
    d->add(li3);

    // Values that should have changed:
    EXPECT_EQ(li1.amount + li2.amount + li3.amount,
              d->getTotal());
    EXPECT_EQ(3, d->getCount());
    // Default values:
    EXPECT_EQ(0, d->getNormalTotal());
    EXPECT_EQ(0, d->getNormalCount());
    EXPECT_EQ(-1, d->getNormalMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getNormalMin());
    EXPECT_EQ(0, d->getRegularDeflectTotal());
    EXPECT_EQ(0, d->getRegularDeflectCount());
    EXPECT_EQ(-1, d->getRegularDeflectMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getRegularDeflectMin());
    EXPECT_EQ(0, d->getNanobotTotal());
    EXPECT_EQ(0, d->getNanobotCount());
    EXPECT_EQ(-1, d->getNanobotMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getNanobotMin());
    EXPECT_EQ(0, d->getRegularDeflectTotal());
    EXPECT_EQ(0, d->getRegularDeflectCount());
    EXPECT_EQ(-1, d->getRegularDeflectMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getRegularDeflectMin());
    // Values that should have changed:
    EXPECT_EQ(li1.amount + li2.amount + li3.amount,
              d->getCritTotal());
    EXPECT_EQ(3, d->getCritCount());
    EXPECT_EQ(li3.amount, d->getCritMax());
    EXPECT_EQ(li2.amount, d->getCritMin());
    // Default values:
    EXPECT_EQ(0, d->getRegularMisses());
    EXPECT_EQ(0, d->getSpecialMisses());
}

TEST_F(DamageTest, add_misses) {

    LineInfo li1;
    LineInfo li2;
    li1.miss = true;
    li2.miss = true;
    li2.special = true;

    d->add(li1);
    d->add(li1);
    d->add(li2);
    d->add(li2);
    d->add(li2);

    //  Values that should have changed:
    EXPECT_EQ(0, d->getTotal());
    EXPECT_EQ(5, d->getCount());
    // Default values:
    EXPECT_EQ(0, d->getNormalTotal());
    EXPECT_EQ(0, d->getNormalCount());
    EXPECT_EQ(-1, d->getNormalMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getNormalMin());
    EXPECT_EQ(0, d->getRegularDeflectTotal());
    EXPECT_EQ(0, d->getRegularDeflectCount());
    EXPECT_EQ(-1, d->getRegularDeflectMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getRegularDeflectMin());
    EXPECT_EQ(0, d->getNanobotTotal());
    EXPECT_EQ(0, d->getNanobotCount());
    EXPECT_EQ(-1, d->getNanobotMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getNanobotMin());
    EXPECT_EQ(0, d->getCritTotal());
    EXPECT_EQ(0, d->getCritCount());
    EXPECT_EQ(-1, d->getCritMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getCritMin());
    // Values that should have changed:
    EXPECT_EQ(2, d->getRegularMisses());
    EXPECT_EQ(3, d->getSpecialMisses());
}

TEST_F(DamageTest, add_twoDeflects) {

    LineInfo li;
    li.deflect = true;

    d->add(li);
    d->add(li);

    EXPECT_EQ(li.amount, d->getTotal());
    EXPECT_EQ(2, d->getCount());
    // Default values:
    EXPECT_EQ(0, d->getNormalTotal());
    EXPECT_EQ(0, d->getNormalCount());
    EXPECT_EQ(-1, d->getNormalMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getNormalMin());
    EXPECT_EQ(0, d->getRegularDeflectTotal());
    // Value that should have changed
    EXPECT_EQ(2, d->getRegularDeflectCount());
    EXPECT_EQ(0, d->getRegularDeflectMax());
    EXPECT_EQ(0, d->getRegularDeflectMin());
    // Default values:
    EXPECT_EQ(0, d->getNanobotTotal());
    EXPECT_EQ(0, d->getNanobotCount());
    EXPECT_EQ(-1, d->getNanobotMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getNanobotMin());
    EXPECT_EQ(0, d->getCritTotal());
    EXPECT_EQ(0, d->getCritCount());
    EXPECT_EQ(-1, d->getCritMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d->getCritMin());
    EXPECT_EQ(0, d->getRegularMisses());
    EXPECT_EQ(0, d->getSpecialMisses());
}

TEST(DamageOperatorPlusTest, nonCritNewMax) {

    LineInfo li1;
    LineInfo li2;
    li1.amount = 14800;
    li2.amount = 15000;

    Damage d1;
    Damage d2;
    d1.add(li1);
    d2.add(li2);

    Damage d3;
    d3 = d1 + d2;

    // Values that should have changed:
    EXPECT_EQ(li1.amount + li2.amount, d3.getTotal());
    EXPECT_EQ(2, d3.getCount());
    EXPECT_EQ(li1.amount + li2.amount, d3.getNormalTotal());
    EXPECT_EQ(2, d3.getNormalCount());
    EXPECT_EQ(li2.amount, d3.getNormalMax());
    EXPECT_EQ(li1.amount, d3.getNormalMin());
    // Default values:
    EXPECT_EQ(0, d3.getRegularDeflectTotal());
    EXPECT_EQ(0, d3.getRegularDeflectCount());
    EXPECT_EQ(-1, d3.getRegularDeflectMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getRegularDeflectMin());
    EXPECT_EQ(0, d3.getNanobotTotal());
    EXPECT_EQ(0, d3.getNanobotCount());
    EXPECT_EQ(-1, d3.getNanobotMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getNanobotMin());
    EXPECT_EQ(0, d3.getCritTotal());
    EXPECT_EQ(0, d3.getCritCount());
    EXPECT_EQ(-1, d3.getCritMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getCritMin());
    EXPECT_EQ(0, d3.getRegularMisses());
    EXPECT_EQ(0, d3.getSpecialMisses());
}

TEST(DamageOperatorPlusTest, addNonCritToEmptyDamage) {

    LineInfo li2;
    li2.amount = 1999;

    Damage d1;
    Damage d2;
    d2.add(li2);

    Damage d3;
    d3 = d1 + d2;

    // Values that should have changed:
    EXPECT_EQ(li2.amount, d3.getTotal());
    EXPECT_EQ(1, d3.getCount());
    EXPECT_EQ(li2.amount, d3.getNormalTotal());
    EXPECT_EQ(1, d3.getNormalCount());
    EXPECT_EQ(li2.amount, d3.getNormalMax());
    EXPECT_EQ(li2.amount, d3.getNormalMin());
    // Default values:
    EXPECT_EQ(0, d3.getRegularDeflectTotal());
    EXPECT_EQ(0, d3.getRegularDeflectCount());
    EXPECT_EQ(-1, d3.getRegularDeflectMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getRegularDeflectMin());
    EXPECT_EQ(0, d3.getNanobotTotal());
    EXPECT_EQ(0, d3.getNanobotCount());
    EXPECT_EQ(-1, d3.getNanobotMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getNanobotMin());
    EXPECT_EQ(0, d3.getCritTotal());
    EXPECT_EQ(0, d3.getCritCount());
    EXPECT_EQ(-1, d3.getCritMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getCritMin());
    EXPECT_EQ(0, d3.getRegularMisses());
    EXPECT_EQ(0, d3.getSpecialMisses());
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
    d1.add(li1);
    d2.add(li2);
    d3.add(li3);

    Damage d4;
    d4 = d1 + d2 + d3;

    // Values that should have changed:
    EXPECT_EQ(li1.amount + li2.amount + li3.amount,
              d4.getTotal());
    EXPECT_EQ(3, d4.getCount());
    // Default values:
    EXPECT_EQ(0, d4.getNormalTotal());
    EXPECT_EQ(0, d4.getNormalCount());
    EXPECT_EQ(-1, d4.getNormalMax());
    // Values that should have changed:
    EXPECT_EQ(std::numeric_limits<int>::max(),
              d4.getNormalMin());
    // Default values:
    EXPECT_EQ(0, d4.getRegularDeflectTotal());
    EXPECT_EQ(0, d4.getRegularDeflectCount());
    EXPECT_EQ(-1, d4.getRegularDeflectMax());
    // Values that should have changed:
    EXPECT_EQ(std::numeric_limits<int>::max(), d4.getRegularDeflectMin());
    EXPECT_EQ(li1.amount + li2.amount + li3.amount,
              d4.getNanobotTotal());
    EXPECT_EQ(3, d4.getNanobotCount());
    EXPECT_EQ(li3.amount, d4.getNanobotMax());
    EXPECT_EQ(li1.amount, d4.getNanobotMin());
    // Default Values
    EXPECT_EQ(0, d4.getCritTotal());
    EXPECT_EQ(0, d4.getCritCount());
    EXPECT_EQ(-1, d4.getCritMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d4.getCritMin());
    EXPECT_EQ(0, d3.getRegularMisses());
    EXPECT_EQ(0, d3.getSpecialMisses());
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
    d1.add(li1);
    d2.add(li2);

    Damage d3;
    d3 = d1 + d2;

    // Values that should have changed:
    EXPECT_EQ(li1.amount + li2.amount, d3.getTotal());
    EXPECT_EQ(2, d3.getCount());
    // Default values:
    EXPECT_EQ(0, d3.getNormalTotal());
    EXPECT_EQ(0, d3.getNormalCount());
    EXPECT_EQ(-1, d3.getNormalMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getNormalMin());
    EXPECT_EQ(0, d3.getRegularDeflectTotal());
    EXPECT_EQ(0, d3.getRegularDeflectCount());
    EXPECT_EQ(-1, d3.getRegularDeflectMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getRegularDeflectMin());
    EXPECT_EQ(0, d3.getNanobotTotal());
    EXPECT_EQ(0, d3.getNanobotCount());
    EXPECT_EQ(-1, d3.getNanobotMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getNanobotMin());
    // Values that should have changed:
    EXPECT_EQ(li1.amount + li2.amount, d3.getCritTotal());
    EXPECT_EQ(2, d3.getCritCount());
    EXPECT_EQ(8000, d3.getCritMax());
    EXPECT_EQ(7999, d3.getCritMin());
    // Default values:
    EXPECT_EQ(0, d3.getRegularMisses());
    EXPECT_EQ(0, d3.getSpecialMisses());
}

TEST(DamageOperatorPlusTest, addCritToEmptyDamage) {

    LineInfo li2;
    li2.amount = 14;
    li2.crit = true;

    Damage d1;
    Damage d2;
    d2.add(li2);

    Damage d3;
    d3 = d1 + d2;

    // Values that should have changed:
    EXPECT_EQ(li2.amount, d3.getTotal());
    EXPECT_EQ(1, d3.getCount());
    // Default values:
    EXPECT_EQ(0, d3.getNormalTotal());
    EXPECT_EQ(0, d3.getNormalCount());
    EXPECT_EQ(-1, d3.getNormalMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getNormalMin());
    EXPECT_EQ(0, d3.getRegularDeflectTotal());
    EXPECT_EQ(0, d3.getRegularDeflectCount());
    EXPECT_EQ(-1, d3.getRegularDeflectMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getRegularDeflectMin());
    EXPECT_EQ(li2.amount, d3.getCritTotal());
    EXPECT_EQ(0, d3.getNanobotTotal());
    EXPECT_EQ(0, d3.getNanobotCount());
    EXPECT_EQ(-1, d3.getNanobotMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getNanobotMin());
    // Values that should have changed:
    EXPECT_EQ(1, d3.getCritCount());
    EXPECT_EQ(14, d3.getCritMax());
    EXPECT_EQ(14, d3.getCritMin());
    // Default values:
    EXPECT_EQ(0, d3.getRegularMisses());
    EXPECT_EQ(0, d3.getSpecialMisses());
}

TEST(DamageOperatorPlusTest, addMiss) {

    LineInfo li1;
    LineInfo li2;
    li1.miss = true;
    li2.miss = true;
    li2.special = true;

    Damage d1;
    Damage d2;
    Damage d3;
    Damage d4;
    Damage d5;
    d1.add(li1);
    d2.add(li1);
    d3.add(li1);
    d4.add(li2);
    d5.add(li2);

    Damage d6;
    d6 = d1 + d2 + d3 + d4 + d5;

    // Default values:
    EXPECT_EQ(0, d6.getTotal());
    // Values that should have changed:
    EXPECT_EQ(5, d6.getCount());
    // Default values:
    EXPECT_EQ(0, d6.getNormalTotal());
    EXPECT_EQ(0, d6.getNormalCount());
    EXPECT_EQ(-1, d6.getNormalMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d6.getNormalMin());
    EXPECT_EQ(0, d6.getRegularDeflectTotal());
    EXPECT_EQ(0, d6.getRegularDeflectCount());
    EXPECT_EQ(-1, d6.getRegularDeflectMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d6.getRegularDeflectMin());
    EXPECT_EQ(0, d6.getNanobotTotal());
    EXPECT_EQ(0, d6.getNanobotCount());
    EXPECT_EQ(-1, d6.getNanobotMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d6.getNanobotMin());
    EXPECT_EQ(0, d6.getCritTotal());
    EXPECT_EQ(0, d6.getCritCount());
    EXPECT_EQ(-1, d6.getCritMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d6.getCritMin());
    // Values that should have changed:
    EXPECT_EQ(3, d6.getRegularMisses());
    EXPECT_EQ(2, d6.getSpecialMisses());
}

TEST(DamageOperatorPlusTest, addDeflect) {

    LineInfo li1;
    LineInfo li2;
    li1.deflect = true;
    li2.deflect = true;
    li1.amount = 999;
    li2.amount = 101;

    Damage d1;
    Damage d2;
    d1.add(li1);
    d2.add(li2);

    Damage d3;
    d3 = d1 + d2;

    EXPECT_EQ(li1.amount + li2.amount, d3.getTotal());
    EXPECT_EQ(2, d3.getCount());
    EXPECT_EQ(0, d3.getNormalTotal());
    EXPECT_EQ(0, d3.getNormalCount());
    EXPECT_EQ(-1, d3.getNormalMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getNormalMin());
    EXPECT_EQ(li1.amount + li2.amount, d3.getRegularDeflectTotal());
    EXPECT_EQ(2, d3.getRegularDeflectCount());
    EXPECT_EQ(li1.amount, d3.getRegularDeflectMax());
    EXPECT_EQ(li2.amount, d3.getRegularDeflectMin());
    EXPECT_EQ(0, d3.getNanobotTotal());
    EXPECT_EQ(0, d3.getNanobotCount());
    EXPECT_EQ(-1, d3.getNanobotMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getNanobotMin());
    EXPECT_EQ(0, d3.getCritTotal());
    EXPECT_EQ(0, d3.getCritCount());
    EXPECT_EQ(-1, d3.getCritMax());
    EXPECT_EQ(std::numeric_limits<int>::max(), d3.getCritMin());
    // Default values:
    EXPECT_EQ(0, d3.getRegularMisses());
    EXPECT_EQ(0, d3.getSpecialMisses());
}
