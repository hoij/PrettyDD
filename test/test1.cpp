#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "parser.h"


TEST(Parser, Construct) {
    Parser parser("Sgtcuddle");
}

// TODO:
//void assertLineInfo(Parser& parser, std::string message,
//                    std::string dealer_name,
//                    std::string receiver_name,
//                    std::string type,
//                    std::string subtype,
//                    int amount,
//                    bool crit,
//                    bool deflect) {
//    LineInfo li1 = parser.otherAndYourPetHitByOther(message);
//    EXPECT_EQ(dealer_name, li1.dealer_name);
//    EXPECT_EQ(receiver_name, li1.receiver_name);
//    EXPECT_EQ(type, li1.type);
//    EXPECT_EQ(subtype, li1.subtype);
//    EXPECT_EQ(amount, li1.amount);
//    EXPECT_EQ(crit, li1.crit);
//    EXPECT_EQ(deflect, li1.deflect);
//}

TEST(Parser, otherAndYourPetHitByOther) {
    Parser parser("");

//    assertLineInfo(parser, "Sheila Marlene hit Predator Rogue for 461 points of melee damage.",
//                   "Sheila Marlene",
//                   "Predator Rogue",
//                   "damage",
//                   "melee",
//                   461,
//                   false,
//                   false);

    LineInfo li1 = parser.otherAndYourPetHitByOther("Sheila Marlene hit Predator Rogue for 461 points of melee damage.");
    EXPECT_EQ("Sheila Marlene", li1.dealer_name);
    EXPECT_EQ("Predator Rogue", li1.receiver_name);
    EXPECT_EQ("damage", li1.type);
    EXPECT_EQ("melee", li1.subtype);
    EXPECT_EQ(461, li1.amount);
    EXPECT_EQ(false, li1.crit);
    EXPECT_EQ(false, li1.deflect);

    LineInfo li9 = parser.otherAndYourPetHitByOther("Sgtcuddle hit Predator Rogue for 4302 points of Burst damage.");
    EXPECT_EQ("Sgtcuddle", li9.dealer_name);
    EXPECT_EQ("Predator Rogue", li9.receiver_name);
    EXPECT_EQ("damage", li9.type);
    EXPECT_EQ("Burst", li9.subtype);
    EXPECT_EQ(4302, li9.amount);
    EXPECT_EQ(false, li9.crit);
    EXPECT_EQ(false, li9.deflect);

    LineInfo li2 = parser.otherAndYourPetHitByOther("Sgtcuddle hit Predator Rogue for 15000 points of Full Auto damage.");
    EXPECT_EQ("Sgtcuddle", li2.dealer_name);
    EXPECT_EQ("Predator Rogue", li2.receiver_name);
    EXPECT_EQ("damage", li2.type);
    EXPECT_EQ("Full Auto", li2.subtype);
    EXPECT_EQ(15000, li2.amount);
    EXPECT_EQ(false, li2.crit);
    EXPECT_EQ(false, li2.deflect);

    LineInfo li3 = parser.otherAndYourPetHitByOther("Balas's reflect shield hit Junebop for 161 points of damage.");
    EXPECT_EQ("Balas", li3.dealer_name);
    EXPECT_EQ("Junebop", li3.receiver_name);
    EXPECT_EQ("damage", li3.type);
    EXPECT_EQ("reflect shield", li3.subtype);
    EXPECT_EQ(161, li3.amount);
    EXPECT_EQ(false, li3.crit);
    EXPECT_EQ(false, li3.deflect);

    LineInfo li4 = parser.otherAndYourPetHitByOther("Letter hit Reet of Paradise for 1586 points of melee damage.Critical hit!");
    EXPECT_EQ("Letter", li4.dealer_name);
    EXPECT_EQ("Reet of Paradise", li4.receiver_name);
    EXPECT_EQ("damage", li4.type);
    EXPECT_EQ("melee", li4.subtype);
    EXPECT_EQ(1586, li4.amount);
    EXPECT_EQ(true, li4.crit);
    EXPECT_EQ(false, li4.deflect);

    LineInfo li5 = parser.otherAndYourPetHitByOther("Cratdat hit Ensign - Ilari'Ra for 801 points of projectile damage. Glancing hit.");
    EXPECT_EQ("Cratdat", li5.dealer_name);
    EXPECT_EQ("Ensign - Ilari'Ra", li5.receiver_name);
    EXPECT_EQ("damage", li5.type);
    EXPECT_EQ("projectile", li5.subtype);
    EXPECT_EQ(801, li5.amount);
    EXPECT_EQ(false, li5.crit);
    EXPECT_EQ(true, li5.deflect);

    LineInfo li6 = parser.otherAndYourPetHitByOther("Someone absorbed 8198 points of energy damage.");
    EXPECT_EQ("Unknown", li6.dealer_name);
    EXPECT_EQ("Someone", li6.receiver_name);
    EXPECT_EQ("damage", li6.type);
    EXPECT_EQ("energy", li6.subtype);
    EXPECT_EQ(8198, li6.amount);
    EXPECT_EQ(false, li6.crit);
    EXPECT_EQ(false, li6.deflect);

    LineInfo li7 = parser.otherAndYourPetHitByOther("Something hit Addicted2 for 49 points of damage by reflect shield.");
    EXPECT_EQ("Unknown", li7.dealer_name);
    EXPECT_EQ("Addicted2", li7.receiver_name);
    EXPECT_EQ("damage", li7.type);
    EXPECT_EQ("reflect shield", li7.subtype);
    EXPECT_EQ(49, li7.amount);
    EXPECT_EQ(false, li7.crit);
    EXPECT_EQ(false, li7.deflect);

    LineInfo li8 = parser.otherAndYourPetHitByOther("Something hit Enfodruid for 1 points of damage by damage shield.");
    EXPECT_EQ("Unknown", li8.dealer_name);
    EXPECT_EQ("Enfodruid", li8.receiver_name);
    EXPECT_EQ("damage", li8.type);
    EXPECT_EQ("damage shield", li8.subtype);
    EXPECT_EQ(1, li8.amount);
    EXPECT_EQ(false, li8.crit);
    EXPECT_EQ(false, li8.deflect);
}



    //EXPECT_EQ(3, t.getI());

// This is a fixture
// More test can be added by
// TEST_F(MyTest, testName) {
//   // actual testing
// }
//class MyTest : public testing::Test {
//protected:
//	MyTest() {}
//	~MyTest() {}
//
//	virtual void SetUp() {
//		cout << "Running SetUp" << endl;
//	}
//	virtual void TearDown() {
//		cout << "Running TearDown" << endl;
//	}
//};
