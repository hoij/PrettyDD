#include "line_info.h"
#include "player.h"
#include "affected_player_vector.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>


class MockPlayerVector : public AffectedPlayerVector {
public:
    MOCK_METHOD1(addToPlayers, void(LineInfo& li));
    MOCK_CONST_METHOD0(getLongestNameLength, int());
};

class PlayerTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        // mpv will be deleted in Player
        mpv = new MockPlayerVector();
        player = new Player("You", mpv);
    }
    virtual void TearDown() {
        delete player;
    }
    Player* player;
    MockPlayerVector* mpv;
};

bool operator==(const LineInfo& lhs, const LineInfo& rhs) {
    return lhs.dealer_name == rhs.dealer_name &&
    lhs.receiver_name == rhs.receiver_name &&
    lhs.type == rhs.type &&
    lhs.subtype == rhs.subtype &&
    lhs.nanoProgramName == rhs.nanoProgramName;
}

TEST_F(PlayerTest, add_damage) {
    LineInfo li;
    li.type = "damage";
    EXPECT_CALL(*mpv, addToPlayers(li))
        .Times(1);
    player->add(li);
}

TEST_F(PlayerTest, add_heal) {
    LineInfo li;
    li.type = "heal";
    EXPECT_CALL(*mpv, addToPlayers(li))
        .Times(1);
    player->add(li);
}

TEST_F(PlayerTest, add_nano) {
    LineInfo li;
    li.type = "nano";
    EXPECT_CALL(*mpv, addToPlayers(li))
        .Times(1);
    player->add(li);
}

TEST_F(PlayerTest, add_nanoCast) {
    // This test uses the real class NanoProgram.
    // Add a "nano cast" with a named nano.
    // Expect the nanoPrograms vector to be empty (the nano is saved to a
    // local variable).
    // Add another "nano cast" with an empty nano name.
    // Expect the nanoPrograms vector to contain one nano,
    // the named nano added previously.
    LineInfo li1;
    li1.type = "nano cast";
    li1.nanoProgramName = "Test Program";
    player->add(li1);
    EXPECT_EQ(0, player->getNanoPrograms().size());

    LineInfo li2;
    li2.type = "nano cast";
    li2.nanoProgramName = "";
    player->add(li2);
    EXPECT_EQ(1, player->getNanoPrograms().size());
    std::string storedNanoName = player->getNanoPrograms()[0].getName();
    EXPECT_EQ(li1.nanoProgramName, storedNanoName);
}

TEST_F(PlayerTest, add_xp) {
    // This test uses the real class XP.
    // Test that a LineInfo object of type xp can be added successfully.
    LineInfo li;
    li.type = "xp";
    li.subtype = "gained";
    li.amount = 123456;
    player->add(li);
    EXPECT_EQ(li.amount, player->getXp().gained.at("xp").total);
}

TEST_F(PlayerTest, getTotalDamage) {
    // Add a few line infos to the player vector.
    // Verify that the total damage is correct.

    // OR: Just verify that it calls PlayerVectors getTotalDamage method.
    LineInfo li1;
    li1.dealer_name = "You";
    li1.receiver_name = "Someone else";
    li1.type = "damage";
    li1.subtype = "projectile";
    li1.amount = 10;


}
