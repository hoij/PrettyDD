#ifdef WIN32
#pragma warning(disable : 4250)
#pragma warning(disable : 4373)
#endif

#include "affected_player_factory_interface.h"
#include "affected_player_factory.h"
#include "affected_player_interface.h"
#include "affected_player_vector.h"
#include "damage.h"
#include "heal.h"
#include "line_info.h"
#include "my_time.h"
#include "nano.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <string>


/* Many of these test cases rely on the implementation of Damage/Heal/Nano/XP.
If a test case fails it might be a good idea to verify that the tests for
these classes pass. */


/* Test Player */

class MockAffectedPlayer : public AffectedPlayerInterface {
public:
    MockAffectedPlayer(std::string name, std::shared_ptr<MyTimeInterface> myTime) :
        name(name), myTime(myTime) {}
    MOCK_METHOD1(add, void(LineInfo& lineInfo));
    std::string getName() const {return name;}
    MOCK_CONST_METHOD0(getTotalDamageReceivedFromPlayer, Damage(void));
    MOCK_CONST_METHOD0(getTotalDamageDealtOnPlayer, Damage(void));
    MOCK_CONST_METHOD0(getDamageReceivedFromPlayer,
                       std::vector<std::pair<std::string, Damage>>(void));
    MOCK_CONST_METHOD0(getDamageDealtOnPlayer,
                       std::vector<std::pair<std::string, Damage>>(void));
    MOCK_CONST_METHOD0(getHeal, Heal&(void));
    MOCK_CONST_METHOD0(getNano, Nano&(void));

private:
    std::string name;
    std::shared_ptr<MyTimeInterface> myTime;
};

class MockAffectedPlayerFactory : public AffectedPlayerFactoryInterface {
public:
    virtual std::shared_ptr<AffectedPlayerInterface> createPlayer(std::string name) {
        return std::make_shared<::testing::NiceMock<MockAffectedPlayer>>(
            name,
            std::make_shared<MyTime>());
    }
};

/* Helper functions */

Damage createDamage(int amount) {
    LineInfo li;
    li.amount = amount;
    Damage d;
    d.add(li);
    return d;
}

Heal createHeal(int amount) {
    LineInfo li;
    li.amount = amount;
    Heal h;
    h.addHealDealtOnPlayer(li);
    return h;
}

Nano createNano(int amount) {
    LineInfo li;
    li.amount = amount;
    Nano n;
    n.addNanoDealtOnPlayer(li);
    return n;
}

/* Test Fixture */

class AffectedPlayerVectorTest : public ::testing::Test {
/* NiceMock suppresses warnings about function calls not expected via an
EXPECT_CALL(). */
protected:
    virtual void SetUp() {
        std::unique_ptr<AffectedPlayerFactoryInterface>
            mockAffectedPlayerFactory(new MockAffectedPlayerFactory());
        
        affectedPlayerVector =
            std::make_shared<AffectedPlayerVector>(std::move(mockAffectedPlayerFactory));

        // Set up standard return values.
        d1 = createDamage(10);
        d2 = createDamage(30);
    }
    virtual void TearDown() {}

    MockAffectedPlayer* addDealerToVector(std::string name);
    MockAffectedPlayer* addReceiverToVector(std::string name);
    void addPlayersToVector(std::string dealer, std::string receiver);

    std::shared_ptr<AffectedPlayerVector> affectedPlayerVector;
    Damage d1;
    Damage d2;
};

MockAffectedPlayer* AffectedPlayerVectorTest::addDealerToVector(
    std::string name) {

    LineInfo li;
    li.dealer_name = name;
    affectedPlayerVector->addToPlayers(li);
    AffectedPlayerInterface* p = affectedPlayerVector->getPlayer(name);
    EXPECT_FALSE(p == nullptr);
    return dynamic_cast<MockAffectedPlayer*>(p);
}

MockAffectedPlayer* AffectedPlayerVectorTest::addReceiverToVector(
    std::string name) {

    LineInfo li;
    li.receiver_name = name;
    affectedPlayerVector->addToPlayers(li);
    AffectedPlayerInterface* p = affectedPlayerVector->getPlayer(name);
    EXPECT_FALSE(p == nullptr);
    return dynamic_cast<MockAffectedPlayer*>(p);
}

void AffectedPlayerVectorTest::addPlayersToVector(std::string dealer,
    std::string receiver) {
    LineInfo li;
    li.dealer_name = dealer;
    li.receiver_name = receiver;
    affectedPlayerVector->addToPlayers(li);
    // Verify that they've been added to the vector
    if (dealer != "") {
        AffectedPlayerInterface* p = affectedPlayerVector->getPlayer(dealer);
        EXPECT_FALSE(p == nullptr);
    }
    if (receiver != "") {
        AffectedPlayerInterface* p = affectedPlayerVector->getPlayer(receiver);
        EXPECT_FALSE(p == nullptr);
    }
}

/* Test cases */

TEST_F(AffectedPlayerVectorTest, addToPlayers_noDealerNorReceiver) {
    /* Adds a line info object with no dealer and no receiver.
    Verifies that the total nr of players has not increased. */
    addPlayersToVector("", "");
    EXPECT_EQ(0, affectedPlayerVector->size());
}

TEST_F(AffectedPlayerVectorTest, addToPlayers_newDealerOnly) {
    /* Adds a line info object with only the dealer name defined.
    Verifies that only the dealer is added. */
    std::string dealerName = "Dealer";
    addPlayersToVector(dealerName, "");
    EXPECT_EQ(1, affectedPlayerVector->size());
}

TEST_F(AffectedPlayerVectorTest, addToPlayers_newReceiverOnly) {
    /* Adds a line info object with only the receiver name defined.
    Verifies that only the receiver is added. */
    std::string receiverName = "Receiver";
    addPlayersToVector(receiverName, "");
    EXPECT_EQ(1, affectedPlayerVector->size());
}

TEST_F(AffectedPlayerVectorTest, addToPlayers_newDealerAndReceiver) {
    /* Adds a line info object with both dealer and receiver names defined.
    Verifies that they are unique and that the total nr of players is 2. */
    std::string dealerName = "Dealer";
    std::string receiverName = "Receiver";
    addPlayersToVector(dealerName, receiverName);
    EXPECT_EQ(2, affectedPlayerVector->size());
}

TEST_F(AffectedPlayerVectorTest, getLongestNameLength) {
    /* Adds eigth players and gets the longest name length. */
    std::string theLongestName = "AReallyReallyLongName OfSomeone";
    addPlayersToVector("Name", "Name2");
    addPlayersToVector("AName2", "AName");
    addPlayersToVector("A Really Long Name Of Someone", theLongestName);
    addPlayersToVector("ALongName", "AReallyReallyLongNameOfSomeone");

    EXPECT_EQ(theLongestName.length(), affectedPlayerVector->getLongestNameLength());
}

TEST_F(AffectedPlayerVectorTest, addToPlayers_existingDealer) {
    /* Adds an identical line info object, with dealer name defined, twice.
    Verifies that the dealer info is added to the existing dealer.
    Also verifies that the added players add method gets called twice. */

    // The first time a new player will be created.
    MockAffectedPlayer* dealer1 = addDealerToVector("Dealer");
    // The second time the add method of the previously
    // created player will be called.
    EXPECT_CALL(*dealer1, add(::testing::_))
        .Times(1);
    MockAffectedPlayer* dealer2 = addDealerToVector("Dealer");

    EXPECT_EQ(dealer1, dealer2);
    EXPECT_EQ(1, affectedPlayerVector->size());
}

TEST_F(AffectedPlayerVectorTest, getTotalDamageDealtOnPlayer) {
    /*
    Adds the a player with the same name as the caller to the vector.
    Calls getTotalDamage().
    Verifies that each players, except for the callers, getTotalDamage
    is in turn called and that the summed damage is correct.
    */

    // Add players to the vector
    MockAffectedPlayer* p1 = addDealerToVector("dealer1");
    MockAffectedPlayer* p2 = addDealerToVector("dealer2");

    MockAffectedPlayer* caller = addDealerToVector("Caller");

    EXPECT_CALL(*p1, getTotalDamageDealtOnPlayer())
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p2, getTotalDamageDealtOnPlayer())
        .WillOnce(::testing::Return(d2));
    EXPECT_CALL(*caller, getTotalDamageDealtOnPlayer())
        .Times(0);

    Damage totalDamage = affectedPlayerVector->getTotalDamageDealtOnPlayer("Caller");

    EXPECT_EQ((d1 + d2).getTotal(), totalDamage.getTotal());
}

TEST_F(AffectedPlayerVectorTest, getDamageDealtOnPlayer) {
    /* Verifies that the correct method in the affected player
    is called. */

    // Add players to the vector
    MockAffectedPlayer* p1 = addDealerToVector("dealer1");

    std::vector<std::pair<std::string, Damage>> expected;
    expected.emplace_back("type", d2);

    // It gets called twice (once to get size and
    // once to return.
    EXPECT_CALL(*p1, getDamageDealtOnPlayer())
        .WillOnce(::testing::Return(expected))
        .WillOnce(::testing::Return(expected));;

    std::vector<std::pair<std::string, Damage>> result2 =
        affectedPlayerVector->getDamageDealtOnPlayer("dealer1");

    EXPECT_EQ(expected[0].first, result2[0].first);
    EXPECT_EQ(expected[0].second.getTotal(),
              result2[0].second.getTotal());
}

TEST_F(AffectedPlayerVectorTest, getDamageDealtOnPlayer_notFound) {
    /* Verifies that a vector with the pair "empty" and an empty Damage
    is returned when the player is not found in the vector. */

    std::string name = "NotInTheVector";

    auto result = affectedPlayerVector->getDamageDealtOnPlayer(name);

    EXPECT_EQ(1, result.size());
    EXPECT_EQ("empty", result[0].first);
    EXPECT_EQ(0, result[0].second.getTotal());
}

TEST_F(AffectedPlayerVectorTest, getTotalHeals) {
    /* Verifies that getHeal is called for each player in the vector and
    that the returned sum is correct. */

    // Add players to the vector
    MockAffectedPlayer* p1 = addDealerToVector("dealer1");
    MockAffectedPlayer* p2 = addDealerToVector("dealer2");

    Heal h1 = createHeal(10);
    Heal h2 = createHeal(30);
    const MockAffectedPlayer* caller = addDealerToVector("Caller");

    EXPECT_CALL(*p1, getHeal())
        .WillOnce(::testing::ReturnRef(h1));
    EXPECT_CALL(*p2, getHeal())
        .WillOnce(::testing::ReturnRef(h2));
    EXPECT_CALL(*caller, getHeal())
        .Times(0);

    Heal totalHeal = affectedPlayerVector->getTotalHeals("Caller");

    EXPECT_EQ((h1 + h2).getPotentialDealtOnPlayer(), totalHeal.getPotentialDealtOnPlayer());
}

TEST_F(AffectedPlayerVectorTest, getHealsPerAffectedPlayer) {
    /*
    This test case depends on the implementation of Heal.
    If it fails, make sure that Heals's tests can pass.

    Calls getHealsPerAffectedPlayer().
    Verifies that each players getHeal is in turn called and that
    the returned vector is sorted on potential heal dealt.
    */

    // Add players to the vector
    MockAffectedPlayer* p1 = addDealerToVector("dealer1");
    MockAffectedPlayer* p2 = addDealerToVector("dealer2");
    MockAffectedPlayer* p3 = addDealerToVector("dealer3");
    MockAffectedPlayer* p4 = addDealerToVector("dealer4");

    Heal h1 = createHeal(10);
    Heal h2 = createHeal(0);
    Heal h3 = createHeal(500000);
    Heal h4 = createHeal(3000);

    EXPECT_CALL(*p1, getHeal())
        .WillOnce(::testing::ReturnRef(h1));
    EXPECT_CALL(*p2, getHeal())
        .WillOnce(::testing::ReturnRef(h2));
    EXPECT_CALL(*p3, getHeal())
        .WillOnce(::testing::ReturnRef(h3));
    EXPECT_CALL(*p4, getHeal())
        .WillOnce(::testing::ReturnRef(h4));

    std::vector<std::pair<std::string, Heal>> result =
        affectedPlayerVector->getHealsPerAffectedPlayer();

    // Assuming the order is the same as when added which could be untrue.
    EXPECT_EQ(h1.getPotentialDealtOnPlayer(), result[0].second.getPotentialDealtOnPlayer());
    EXPECT_EQ(h2.getPotentialDealtOnPlayer(), result[1].second.getPotentialDealtOnPlayer());
    EXPECT_EQ(h3.getPotentialDealtOnPlayer(), result[2].second.getPotentialDealtOnPlayer());
    EXPECT_EQ(h4.getPotentialDealtOnPlayer(), result[3].second.getPotentialDealtOnPlayer());
    EXPECT_EQ(4, result.size());
}

TEST_F(AffectedPlayerVectorTest, getTotalNano) {
    /* Verifies that getNano is called for each player in the vector and
    that the returned sum is correct. */

    // Add players to the vector
    MockAffectedPlayer* p1 = addDealerToVector("dealer1");
    MockAffectedPlayer* p2 = addDealerToVector("dealer2");

    Nano n1 = createNano(10);
    Nano n2 = createNano(30);
    const MockAffectedPlayer* caller = addDealerToVector("Caller");

    EXPECT_CALL(*p1, getNano())
        .WillOnce(::testing::ReturnRef(n1));
    EXPECT_CALL(*p2, getNano())
        .WillOnce(::testing::ReturnRef(n2));
    EXPECT_CALL(*caller, getNano())
        .Times(0);

    Nano totalNano = affectedPlayerVector->getTotalNano("Caller");

    EXPECT_EQ((n1 + n2).getTotalDealtOnPlayer(), totalNano.getTotalDealtOnPlayer());
}

TEST_F(AffectedPlayerVectorTest, getNanoPerAffectedPlayer) {
    /* Verifies that the nanos are returned in a sorted list */

    // Add players to the vector
    MockAffectedPlayer* p1 = addDealerToVector("dealer1");
    MockAffectedPlayer* p2 = addDealerToVector("dealer2");
    MockAffectedPlayer* p3 = addDealerToVector("dealer3");
    MockAffectedPlayer* p4 = addDealerToVector("dealer4");

    Nano n1 = createNano(10);
    Nano n2 = createNano(0);
    Nano n3 = createNano(500000);
    Nano n4 = createNano(3000);

    EXPECT_CALL(*p1, getNano())
        .WillOnce(::testing::ReturnRef(n1));
    EXPECT_CALL(*p2, getNano())
        .WillOnce(::testing::ReturnRef(n2));
    EXPECT_CALL(*p3, getNano())
        .WillOnce(::testing::ReturnRef(n3));
    EXPECT_CALL(*p4, getNano())
        .WillOnce(::testing::ReturnRef(n4));

    std::vector<std::pair<std::string, Nano>> result =
        affectedPlayerVector->getNanoPerAffectedPlayer();

    EXPECT_EQ(n3.getTotalDealtOnPlayer(), result[0].second.getTotalDealtOnPlayer());
    EXPECT_EQ(n4.getTotalDealtOnPlayer(), result[1].second.getTotalDealtOnPlayer());
    EXPECT_EQ(n1.getTotalDealtOnPlayer(), result[2].second.getTotalDealtOnPlayer());
    EXPECT_EQ(n2.getTotalDealtOnPlayer(), result[3].second.getTotalDealtOnPlayer());
    EXPECT_EQ(p3->getName(), result[0].first);
    EXPECT_EQ(p4->getName(), result[1].first);
    EXPECT_EQ(p1->getName(), result[2].first);
    EXPECT_EQ(p2->getName(), result[3].first);
    EXPECT_EQ(4, result.size());
}

TEST_F(AffectedPlayerVectorTest, getTotalDamageForAllAffectedPlayers) {
    /*
    Adds several players with different damage and retreives the damage
    per player.
    Verifies that each AffectedPlayers correct get method is called.
    */

    // Add players to the vector
    MockAffectedPlayer* p1 = addReceiverToVector("Receiver1");
    MockAffectedPlayer* p2 = addReceiverToVector("Receiver2");
    MockAffectedPlayer* p3 = addReceiverToVector("Receiver3");
    MockAffectedPlayer* p4 = addReceiverToVector("Receiver4");

    // Set up the return values.
    Damage d3 = createDamage(7000);
    Damage d4 = createDamage(0);
    Damage d5 = createDamage(500000);
    Damage d6 = createDamage(1500);

    EXPECT_CALL(*p1, getTotalDamageReceivedFromPlayer())
        .WillOnce(::testing::Return(d3));
    EXPECT_CALL(*p2, getTotalDamageReceivedFromPlayer())
        .WillOnce(::testing::Return(d4));
    EXPECT_CALL(*p3, getTotalDamageReceivedFromPlayer())
        .WillOnce(::testing::Return(d5));
    EXPECT_CALL(*p4, getTotalDamageReceivedFromPlayer())
        .WillOnce(::testing::Return(d6));

    std::vector<std::pair<std::string, Damage>> result =
        affectedPlayerVector->
            getTotalDamageReceivedFromPlayerPerAffectedPlayer("Caller");

    // Assuming the order is the same as when added which could be untrue.
    EXPECT_EQ(d3.getTotal(), result[0].second.getTotal());
    EXPECT_EQ(d4.getTotal(), result[1].second.getTotal());
    EXPECT_EQ(d5.getTotal(), result[2].second.getTotal());
    EXPECT_EQ(d6.getTotal(), result[3].second.getTotal());
    EXPECT_EQ(4, result.size());
}

//TEST_F(AffectedPlayerVectorTest, copyConstructor) {
//    /* Verifies that all affected players are copied
//    when the copy constructor is used. */
//
//    // Add players to the vector
//    std::shared_ptr<MockAffectedPlayer> p1 = addReceiverToVector("Receiver1");
//    std::shared_ptr<MockAffectedPlayer> p2 = addReceiverToVector("Receiver2");
//    std::shared_ptr<MockAffectedPlayer> p3 = addDealerToVector("Dealer1");
//    std::shared_ptr<MockAffectedPlayer> p4 = addDealerToVector("Dealer2");
//
//    EXPECT_CALL(*p1, clone());
//    EXPECT_CALL(*p2, clone());
//    EXPECT_CALL(*p3, clone());
//    EXPECT_CALL(*p4, clone());
//
//    AffectedPlayerVector copyOfVector(*affectedPlayerVector);
//
//    EXPECT_EQ(4, copyOfVector.size());
//
//    std::cout << (copyOfVector.getPlayer("Receiver1")) << std::endl;
//    std::cout << "here" << std::endl;
//    EXPECT_TRUE(copyOfVector.getPlayer("Receiver2") != nullptr);
//    EXPECT_TRUE(copyOfVector.getPlayer("Dealer1") != nullptr);
//    EXPECT_TRUE(copyOfVector.getPlayer("Dealer2") != nullptr);
//}