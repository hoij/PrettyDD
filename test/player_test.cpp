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
    MOCK_CONST_METHOD1(getTotalRegularDamage, Damage(std::string callerName));
    MOCK_CONST_METHOD1(getTotalNanobotsDamage, Damage(std::string callerName));

    MOCK_CONST_METHOD2(getTotalDamagePerDamageType, Damage(std::string callerName,
                                                     const std::string damageType));
    MOCK_CONST_METHOD2(getTotalRegularDamagePerDamageType, Damage(std::string callerName,
                                                     const std::string damageType));
    MOCK_CONST_METHOD2(getTotalNanobotsDamagePerDamageType, Damage(std::string callerName,
                                                     const std::string damageType));

    MOCK_CONST_METHOD1(getTotalDamageForEachPlayer,
                       std::vector<std::pair<std::string, Damage>>(
                           std::string callerName));


    MOCK_CONST_METHOD1(getNanobotsDamagePerAffectedPlayer,
                       std::map<std::string, Damage>&(std::string name));

    MOCK_CONST_METHOD1(getRegularDamagePerAffectedPlayer,
                       std::map<std::string, Damage>&(std::string name));

    MOCK_METHOD1(getTotalHeals, Heal(std::string callerName));

    MOCK_CONST_METHOD0(getHealsForEachAffectedPlayer,
                        std::vector<std::pair<std::string, Heal>>(void));

    MOCK_CONST_METHOD1(getTotalNano, Nano(std::string callerName));

    MOCK_CONST_METHOD1(getNanoForEachAffectedPlayer,
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
    EXPECT_EQ(li.amount, player->getXp().gained.at("xp").total);
}

TEST_F(PlayerTest, getTotalNanobotsDamage) {
    /* Verifies that getTotalNanobotsDamage() calls AffectedPlayerVectors
    getTotalNanobotsDamage() method.  */

    EXPECT_CALL(*mockAffectedPlayerVector, getTotalNanobotsDamage(player->getName()))
        .WillOnce(::testing::Return(d1));

    Damage result = player->getTotalNanobotsDamage();

    EXPECT_EQ(d1.getTotalDealt(), result.getTotalDealt());
}

TEST_F(PlayerTest, getTotalRegularDamage) {
    /* Verifies that getTotalRegularDamage() calls AffectedPlayerVectors
    getTotalRegularDamage() method.  */

    EXPECT_CALL(*mockAffectedPlayerVector, getTotalRegularDamage(player->getName()))
        .WillOnce(::testing::Return(d1));

    Damage result = player->getTotalRegularDamage();

    EXPECT_EQ(d1.getTotalDealt(), result.getTotalDealt());
}

TEST_F(PlayerTest, getTotalDamage) {
    /* Verifies that getTotalDamage() calls AffectedPlayerVectors
    getTotalDamage() */

    EXPECT_CALL(*mockAffectedPlayerVector, getTotalDamage(player->getName()))
        .WillOnce(::testing::Return(d1));

    Damage result = player->getTotalDamage();

    EXPECT_EQ(d1.getTotalDealt(), result.getTotalDealt());
}

TEST_F(PlayerTest, getTotalNanobotsDamagePerDamageType) {
    /* Verifies that getTotalNanobotsDamagePerDamageType() calls
    AffectedPlayerVectors getTotalNanobotsDamagePerDamageType() method. */

    std::string damageType = "chemical";
    EXPECT_CALL(*mockAffectedPlayerVector,
                getTotalNanobotsDamagePerDamageType(player->getName(), damageType))
        .WillOnce(::testing::Return(d1));

    Damage result = player->getTotalNanobotsDamagePerDamageType(damageType);

    EXPECT_EQ(d1.getTotalDealt(), result.getTotalDealt());
}

TEST_F(PlayerTest, getTotalRegularDamagePerDamageType) {
    /* Verifies that getTotalRegularDamagePerDamageType() calls
    AffectedPlayerVectors getTotalRegularDamagePerDamageType() method. */

    std::string damageType = "chemical";
    EXPECT_CALL(*mockAffectedPlayerVector,
                getTotalRegularDamagePerDamageType(player->getName(), damageType))
        .WillOnce(::testing::Return(d1));

    Damage result = player->getTotalRegularDamagePerDamageType(damageType);

    EXPECT_EQ(d1.getTotalDealt(), result.getTotalDealt());
}

TEST_F(PlayerTest, getTotalDamagePerDamageType) {
    /* Verifies that getTotalDamagePerDamageType() calls AffectedPlayerVectors
    getTotalRegularDamagePerDamageType() and that the damage is summed.*/

    std::string damageType = "chemical";
    EXPECT_CALL(*mockAffectedPlayerVector,
                getTotalDamagePerDamageType(player->getName(), damageType))
        .WillOnce(::testing::Return(d1));

    Damage result = player->getTotalDamagePerDamageType(damageType);

    EXPECT_EQ(d1.getTotalDealt(), result.getTotalDealt());
}

TEST_F(PlayerTest, getTotalDamageForEachAffectedPlayer) {
    /* Verifies that getTotalDamageForEachAffectedPlayer() calls
    PlayerVectors getTotalDamageForEachPlayer(std::string callerName) */

    std::vector<std::pair<std::string, Damage>> totalDamagePerAffectedPlayer;
    totalDamagePerAffectedPlayer.push_back(
        std::make_pair("AffectedPlayer1", d1));

    EXPECT_CALL(*mockAffectedPlayerVector,
        getTotalDamageForEachPlayer(player->getName()))
        .WillOnce(::testing::Return(totalDamagePerAffectedPlayer));

    std::vector<std::pair<std::string, Damage>> result =
        player->getTotalDamageForEachAffectedPlayer();
}

TEST_F(PlayerTest, getNanobotsDamagePerAffectedPlayer) {
    /* Verifies that getTotalDamageForEachAffectedPlayer() calls
    PlayerVectors getTotalDamageForEachPlayer(std::string callerName) */

    std::string name = "AffectedPlayer1";
    std::map<std::string, Damage> nanobotsDamagePerAffectedPlayer;
    nanobotsDamagePerAffectedPlayer[name] = d1;

    EXPECT_CALL(*mockAffectedPlayerVector,
        getNanobotsDamagePerAffectedPlayer(name))
        .WillOnce(::testing::ReturnRef(nanobotsDamagePerAffectedPlayer));

    std::map<std::string, Damage> result =
        player->getNanobotsDamagePerAffectedPlayer(name);

    EXPECT_EQ(nanobotsDamagePerAffectedPlayer.at(name).getTotalDealt(),
              result.at(name).getTotalDealt());
}

TEST_F(PlayerTest, getRegularDamagePerAffectedPlayer) {
    /* Verifies that getTotalDamageForEachAffectedPlayer() calls
    PlayerVectors getTotalDamageForEachPlayer(std::string callerName) */

    std::string name = "AffectedPlayer1";
    std::map<std::string, Damage> regularDamagePerAffectedPlayer;
    regularDamagePerAffectedPlayer[name] = d1;

    EXPECT_CALL(*mockAffectedPlayerVector,
        getRegularDamagePerAffectedPlayer(name))
        .WillOnce(::testing::ReturnRef(regularDamagePerAffectedPlayer));

    std::map<std::string, Damage> result =
        player->getRegularDamagePerAffectedPlayer(name);

    EXPECT_EQ(regularDamagePerAffectedPlayer.at(name).getTotalDealt(),
              result.at(name).getTotalDealt());
}

TEST_F(PlayerTest, getTotalHeals) {
    /* Verifies that getTotalDamageForEachAffectedPlayer() calls
    PlayerVectors getTotalDamageForEachPlayer(std::string callerName) */

    LineInfo li;
    li.amount = 10;
    Heal h;
    h.add(li, "dealer");

    EXPECT_CALL(*mockAffectedPlayerVector,
        getTotalHeals(player->getName()))
        .WillOnce(::testing::Return(h));

    Heal result = player->getTotalHeals();

    EXPECT_EQ(h.getPotentialDealt(), result.getPotentialDealt());
}
