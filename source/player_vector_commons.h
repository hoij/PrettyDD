#ifndef PLAYER_VECTOR_COMMONS
#define PLAYER_VECTOR_COMMONS


#include "line_info.h"

#include <memory>
#include <vector>


namespace playerVectorCommons {

    template<typename TPlayerFactory, typename TSmartPointer>
    void createPlayer(std::string& name,
                      LineInfo& lineInfo,
                      std::vector<TSmartPointer>& players,
                      TPlayerFactory& playerFactory) {
        TSmartPointer player = playerFactory->createPlayer(name);
        player->add(lineInfo);
        players.push_back(std::move(player));
    }

    template<typename TPlayerFactory, typename TSmartPointer>
    void addToPlayers(LineInfo& lineInfo,
                      std::vector<TSmartPointer>& players,
                      TPlayerFactory& playerFactory,
                      bool log) {
        /* Adds the info found in a log line to dealer and receiver.
        If a player with the same name is not found, a new one is
        created by using the playerFactory. */
        bool dealerFound = false;
        bool receiverFound = false;

        if (!log) {  // Don't log when stopped.
            return;
        }

        for (auto& player : players) {
            if (player->getName() == lineInfo.dealer_name) {
                player->add(lineInfo);
                dealerFound = true;
            }
            else if (player->getName() == lineInfo.receiver_name) {
                player->add(lineInfo);
                receiverFound = true;
            }
        }

        /* Some log lines only contain dealer or receiver but not both,
        in which case the name of the non-existing one will be empty. */
        if (!dealerFound && lineInfo.dealer_name != "") {
            createPlayer(lineInfo.dealer_name, lineInfo, players, playerFactory);
        }
        if (!receiverFound && lineInfo.receiver_name != "") {
            createPlayer(lineInfo.receiver_name, lineInfo, players, playerFactory);
        }
    }

    template<typename TPlayer, typename TSmartPointer>
    TPlayer* getPlayer(std::string name,
                       const std::vector<TSmartPointer>& players) {
        /* Finds the player and returns it as a raw pointer.
        Returns a nullptr if the player isn't found.
        Reason for using a raw pointer is that the receiving end
        should not take ownership. */
        for (const auto& player : players) {
            if (player->getName() == name) {
                return player.get();
            }
        }
        return nullptr;
    }

    template<typename T>
    size_t getLongestNameLength(const std::vector<T>& players) {
        size_t longestNameLength = 0;
        for (const auto& player : players) {
            if (player->getName().length() > longestNameLength) {
                longestNameLength = player->getName().length();
            }
        }
        return longestNameLength;
    }

}


#endif  //PLAYER_VECTOR_COMMONS
