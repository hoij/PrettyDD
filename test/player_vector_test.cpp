#include "base_player.h"
#include "line_info.h"
#include "player_vector.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>


class TestPlayer : public BasePlayer {
public:
    TestPlayer(std::string name) : BasePlayer(name) {}
    void add(LineInfo& li) {
        (void)li;
        callsToAdd++;
    }
    int callsToAdd = 0;
};


class PlayerVectorTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        playerVector = new PlayerVector<TestPlayer>;
    }
    virtual void TearDown() {
        delete playerVector;
    }
    PlayerVector<TestPlayer>* playerVector;
};

TEST_F(PlayerVectorTest, addToPlayers_noDealerNorReceiver) {
    // Adds a line info object with no dealer and no receiver.
    // Verifies that the total nr of players is 0.
    LineInfo li;
    playerVector->addToPlayers(li);

    int nrOfPlayers = 0;
    for (const auto& player : *playerVector) {
        (void)player;
        nrOfPlayers++;
    }

    EXPECT_EQ(0, nrOfPlayers);
}


TEST_F(PlayerVectorTest, addToPlayers_newDealerOnly) {
    // Adds a line info object with only the dealer name defined
    // Verifies that only the dealer is added.
    LineInfo li;
    li.dealer_name = "Dealer";
    playerVector->addToPlayers(li);

    int nrOfPlayers = 0;
    for (const auto& player : *playerVector) {
        nrOfPlayers++;
        EXPECT_EQ("Dealer", player.getName());
    }
    EXPECT_EQ(1, nrOfPlayers);
}

TEST_F(PlayerVectorTest, addToPlayers_newReceiverOnly) {
    // Adds a line info object with only the receiver name defined
    // Verifies that only the receiver is added.
    LineInfo li;
    li.receiver_name = "Receiver";
    playerVector->addToPlayers(li);

    int nrOfPlayers = 0;
    for (const auto& player : *playerVector) {
        nrOfPlayers++;
        EXPECT_EQ("Receiver", player.getName());
    }
    EXPECT_EQ(1, nrOfPlayers);
}

TEST_F(PlayerVectorTest, addToPlayers_newDealerAndReceiver) {
    // Adds a line info object with both dealer and receiver names defined.
    // Verifies that they are unique and that the total nr of players is 2.
    LineInfo li;
    li.dealer_name = "Dealer";
    li.receiver_name = "Receiver";
    playerVector->addToPlayers(li);

    int nrOfPlayers = 0;
    bool isDealerFound = false;
    bool isReceiverFound = false;
    for (const auto& player : *playerVector) {
        nrOfPlayers++;
        if (player.getName() == "Dealer" && !isDealerFound) {
            isDealerFound = true;
        }
        else if (player.getName() == "Receiver" && !isReceiverFound) {
            isReceiverFound = true;
        }
    }

    EXPECT_EQ(2, nrOfPlayers);
    EXPECT_TRUE(isDealerFound);
    EXPECT_TRUE(isReceiverFound);
}

TEST_F(PlayerVectorTest, addToPlayers_existingDealer) {
    // Adds an identical line info object, with dealer name defined, twice.
    // Verifies that the dealer info is added to the existing dealer.
    // Also verifies that the added players add method gets called twice.
    LineInfo li;
    li.receiver_name = "Dealer";
    playerVector->addToPlayers(li);
    playerVector->addToPlayers(li);

    int nrOfPlayers = 0;
    for (const auto& player : *playerVector) {
        nrOfPlayers++;
        EXPECT_EQ("Dealer", player.getName());
        EXPECT_EQ(2, player.callsToAdd);
    }
    EXPECT_EQ(1, nrOfPlayers);
}

TEST_F(PlayerVectorTest, addToPlayers_existingReceiver) {
    // Adds an identical line info object, with receiver name defined, twice.
    // Verifies that the receiver info is added to the existing receiver.
    // Also verifies that the added players add method gets called twice.
    LineInfo li;
    li.receiver_name = "Dealer";
    playerVector->addToPlayers(li);
    playerVector->addToPlayers(li);

    int nrOfPlayers = 0;
    for (const auto& player : *playerVector) {
        nrOfPlayers++;
        EXPECT_EQ("Dealer", player.getName());
        EXPECT_EQ(2, player.callsToAdd);
    }
    EXPECT_EQ(1, nrOfPlayers);
}

TEST_F(PlayerVectorTest, addToPlayers_existingDealerAndReceiver) {
    // Adds an identical line info object, with receiver and dealer name
    // defined, twice.
    // Verifies that the info is added to the existing players.
    // Also verifies that the added players add method gets called twice.
    LineInfo li;
    li.dealer_name = "Dealer";
    li.receiver_name = "Receiver";
    playerVector->addToPlayers(li);
    playerVector->addToPlayers(li);

    int nrOfPlayers = 0;
    bool isDealerFound = false;
    bool isReceiverFound = false;
    for (const auto& player : *playerVector) {
        nrOfPlayers++;
        if (player.getName() == "Dealer" && !isDealerFound) {
            isDealerFound = true;
        }
        else if (player.getName() == "Receiver" && !isReceiverFound) {
            isReceiverFound = true;
        }
    }
    EXPECT_EQ(2, nrOfPlayers);
    EXPECT_TRUE(isDealerFound);
    EXPECT_TRUE(isReceiverFound);
}

TEST_F(PlayerVectorTest, getLongestNameLength) {
    // Adds eigth players and gets the longest name length.
    LineInfo li1;
    LineInfo li2;
    LineInfo li3;
    LineInfo li4;
    li1.dealer_name = "Name";
    li1.receiver_name = "Name2";
    li2.dealer_name = "AName2";
    li2.receiver_name = "AName";
    li3.dealer_name = "A Really Long Name Of Someone";
    li3.receiver_name = "AReallyReallyLongName OfSomeone";
    li4.dealer_name = "ALongName";
    li4.receiver_name = "AReallyReallyLongNameOfSomeone";
    playerVector->addToPlayers(li1);
    playerVector->addToPlayers(li2);
    playerVector->addToPlayers(li3);
    playerVector->addToPlayers(li4);

    EXPECT_EQ(li3.receiver_name.length(), playerVector->getLongestNameLength());
}

TEST_F(PlayerVectorTest, getPlayer) {
    // Adds players and uses the getPlayer method to return them.
    // Verifies that they are correct by comparing their names.
    LineInfo li1;
    LineInfo li2;
    li1.dealer_name = "Dealer";
    li1.receiver_name = "Receiver";
    li2.dealer_name = "Dealer2";
    li2.receiver_name = "Receiver2";
    playerVector->addToPlayers(li1);
    playerVector->addToPlayers(li2);

    EXPECT_EQ("Dealer", playerVector->getPlayer("Dealer")->getName());
    EXPECT_EQ("Dealer2", playerVector->getPlayer("Dealer2")->getName());
    EXPECT_EQ("Receiver", playerVector->getPlayer("Receiver")->getName());
    EXPECT_EQ("Receiver2", playerVector->getPlayer("Receiver2")->getName());
}
