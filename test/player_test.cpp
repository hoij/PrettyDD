#include "affected_player.h"
#include "affected_player_vector.h"
#include "damage.h"
#include "heal.h"
#include "line_info.h"
#include "player.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>


class MockAffectedPlayerVector : public AffectedPlayerVector<AffectedPlayer*> {
public:
    MOCK_METHOD1(addToPlayers, void(LineInfo& li));

    MOCK_CONST_METHOD0(getLongestNameLength, size_t());

    MOCK_CONST_METHOD1(getTotalDamage, Damage(std::string callerName));

    MOCK_CONST_METHOD2(getTotalDamagePerDamageType, Damage(std::string callerName,
                                                     const std::string damageType));

    MOCK_CONST_METHOD1(getTotalDamageForAllAffectedPlayers,
                       std::vector<std::pair<std::string, Damage>>(
                       std::string callerName));

    MOCK_CONST_METHOD1(getAllDamageFromAffectedPlayer,
                       std::vector<std::pair<std::string, Damage>>(std::string name));

    MOCK_METHOD1(getTotalHeals, Heal(std::string callerName));

    MOCK_CONST_METHOD0(getHealsForAllAffectedPlayers,
                       std::vector<std::pair<std::string, Heal>>(void));

    MOCK_CONST_METHOD1(getTotalNano, Nano(std::string callerName));

    MOCK_CONST_METHOD1(getNanoForAllAffectedPlayers,
                       std::vector<std::pair<std::string, Nano>>(std::string callerName));
};

class PlayerTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        // mockAffectedPlayerVector will be deleted in Player
        mockAffectedPlayerVector = new MockAffectedPlayerVector();
        player = new Player("You", mockAffectedPlayerVector);

        // Set up the return values.
        LineInfo li1;
        LineInfo li2;
        li1.amount = 10;
        li2.amount = 30;
        d1.addDamageDealtOnPlayer(li1);
        d2.addDamageDealtOnPlayer(li2);
    }
    virtual void TearDown() {
        delete player;
    }
    Player* player;
    MockAffectedPlayerVector* mockAffectedPlayerVector;
    Damage d1;
    Damage d2;
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
    EXPECT_CALL(*mockAffectedPlayerVector, addToPlayers(li))
        .Times(1);
    player->add(li);
}

TEST_F(PlayerTest, add_heal) {
    LineInfo li;
    li.type = "heal";
    EXPECT_CALL(*mockAffectedPlayerVector, addToPlayers(li))
        .Times(1);
    player->add(li);
}

TEST_F(PlayerTest, add_nano) {
    LineInfo li;
    li.type = "nano";
    EXPECT_CALL(*mockAffectedPlayerVector, addToPlayers(li))
        .Times(1);
    player->add(li);
}

TEST_F(PlayerTest, add_nanoCast) {
    /* This test uses the real class NanoProgram.
    Add a "nano cast" with a named nano.
    Expect the nanoPrograms vector to be empty (the nano is saved to a
    local variable).
    Add another "nano cast" with an empty nano name.
    Expect the nanoPrograms vector to contain one nano,
    the named nano added previously. */
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
    /*This test uses the real class XP.
    Test that a LineInfo object of type xp can be added successfully. */
    LineInfo li;
    li.type = "xp";
    li.subtype = "gained";
    li.amount = 123456;
    player->add(li);
    EXPECT_EQ(li.amount, player->getXp().getTotalGained("xp"));
}

TEST_F(PlayerTest, getTotalDamage) {
    /* Verifies that getTotalDamage() calls AffectedPlayerVectors
    getTotalDamage() */

    EXPECT_CALL(*mockAffectedPlayerVector, getTotalDamage(player->getName()))
        .WillOnce(::testing::Return(d1));

    Damage result = player->getTotalDamage();

    EXPECT_EQ(d1.getTotalDealtOnPlayer(), result.getTotalDealtOnPlayer());
}

TEST_F(PlayerTest, getTotalDamagePerDamageType) {
    /* Verifies that getTotalDamagePerDamageType() calls AffectedPlayerVectors
    getTotalRegularDamagePerDamageType() and that the damage is summed.*/

    std::string damageType = "chemical";
    EXPECT_CALL(*mockAffectedPlayerVector,
                getTotalDamagePerDamageType(player->getName(), damageType))
        .WillOnce(::testing::Return(d1));

    Damage result = player->getTotalDamagePerDamageType(damageType);

    EXPECT_EQ(d1.getTotalDealtOnPlayer(), result.getTotalDealtOnPlayer());
}

TEST_F(PlayerTest, getTotalDamageForAllAffectedPlayers) {
    /* Verifies that getTotalDamageForAllAffectedPlayers() calls
    PlayerVectors getTotalDamageForAllAffectedPlayers(std::string callerName)
    */

    std::vector<std::pair<std::string, Damage>> totalDamageForAllAffectedPlayers;
    totalDamageForAllAffectedPlayers.push_back(
        std::make_pair("AffectedPlayer1", d1));

    EXPECT_CALL(*mockAffectedPlayerVector,
        getTotalDamageForAllAffectedPlayers(player->getName()))
        .WillOnce(::testing::Return(totalDamageForAllAffectedPlayers));

    std::vector<std::pair<std::string, Damage>> result =
        player->getTotalDamageForAllAffectedPlayers();
}

TEST_F(PlayerTest, getAllDamageFromAffectedPlayer) {
    /* Verifies that getAllDamageFromAffectedPlayer() calls
    PlayerVectors getAllDamageFromAffectedPlayer(std::string callerName).
    */

    std::string name = "AffectedPlayer1";
    std::vector<std::pair<std::string, Damage>> damageFromAffectedPlayer;
    damageFromAffectedPlayer.push_back(std::make_pair(name, d1));

    EXPECT_CALL(*mockAffectedPlayerVector,
        getAllDamageFromAffectedPlayer(name))
        .WillOnce(::testing::Return(damageFromAffectedPlayer));

    std::vector<std::pair<std::string, Damage>> result =
        player->getAllDamageFromAffectedPlayer(name);

    EXPECT_EQ(damageFromAffectedPlayer[0].second.getTotalDealtOnPlayer(),
              result[0].second.getTotalDealtOnPlayer());
}

TEST_F(PlayerTest, getTotalHeals) {
    /* Verifies that getTotalHeals() calls
    PlayerVectors getTotalHeals. */

    LineInfo li;
    li.amount = 10;
    Heal h;
    h.addHealDealtOnPlayer(li);

    EXPECT_CALL(*mockAffectedPlayerVector,
        getTotalHeals(player->getName()))
        .WillOnce(::testing::Return(h));

    Heal result = player->getTotalHeals();

    EXPECT_EQ(h.getPotentialDealtOnPlayer(), result.getPotentialDealtOnPlayer());
}
