#ifndef PLAYER_VECTOR_H
#define PLAYER_VECTOR_H


#include "player_interface.h"
#include "player_factory_interface.h"

#include <memory>
#include <string>
#include <vector>


class Damage;
class Heal;
class LineInfo;

class PlayerVector {
public:
    PlayerVector(std::string playerRunningProgram,
                 std::unique_ptr<PlayerFactoryInterface> playerFactory);
    ~PlayerVector() {}

    void addToPlayers(LineInfo& lineInfo);
    PlayerInterface* getPlayer(std::string name);
    std::vector<std::unique_ptr<PlayerInterface>>::size_type size() {
        return players.size();
    }
    size_t getLongestNameLength() const;

    std::vector<std::pair<std::string, Damage>>
    getTotalDamageDealtPerPlayer() const;

    std::vector<std::pair<std::string, Damage>>
    getTotalDamageReceivedPerPlayer() const;

    std::vector<std::pair<std::string, Damage>>
    getDamageDealtPerType(std::string playerName) const;

    std::vector<std::pair<std::string, Damage>>
    getDamageReceivedPerType(std::string playerName) const;

    std::vector<std::pair<std::string, Damage>>
    getDamageDealtPerType(std::string playerName,
                          std::string opponentName) const;

    std::vector<std::pair<std::string, Damage>>
    getDamageDealtPerOpponent(std::string playerName) const;

    std::vector<std::pair<std::string, Damage>>
    getDamageReceivedPerOpponent(std::string playerName) const;


    std::string renameIfSelf(std::string name) const;
    void startLogging();
    void stopLogging();
    void reset();

    typedef std::vector<std::unique_ptr<PlayerInterface>>::iterator PlayerVectorIterator;
    PlayerVectorIterator begin() {return players.begin();}
    PlayerVectorIterator end() {return players.end();}
    typedef std::vector<std::unique_ptr<PlayerInterface>>::const_iterator const_PlayerVectorIterator;
    const_PlayerVectorIterator begin() const {return players.begin();}
    const_PlayerVectorIterator end() const {return players.end();}

private:
    bool log = false;
    std::string playerRunningProgram;
    std::unique_ptr<PlayerFactoryInterface> playerFactory = nullptr;
    std::vector<std::unique_ptr<PlayerInterface>> players;
};


#endif  // PLAYER_VECTOR_H
