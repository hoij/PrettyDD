#ifndef PLAYER_VECTOR_H
#define PLAYER_VECTOR_H


#include "player_interface.h"
#include "player_factory_interface.h"

#include <string>
#include <vector>


class Damage;
class Heal;
class LineInfo;

class PlayerVector {
public:
    PlayerVector(std::string playerRunningProgram,
                 PlayerFactoryInterface* playerFactory) :
        playerRunningProgram(playerRunningProgram),
        playerFactory(playerFactory)
        {}
    ~PlayerVector();
    PlayerVector(const PlayerVector& other) = delete;

    void addToPlayers(LineInfo& lineInfo);
    PlayerInterface* getPlayer(std::string name);
    std::vector<PlayerInterface*>::size_type size() { return players.size(); }

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

    typedef std::vector<PlayerInterface*>::iterator PlayerVectorIterator;
    virtual PlayerVectorIterator begin() {return players.begin();}
    virtual PlayerVectorIterator end() {return players.end();}
    typedef std::vector<PlayerInterface*>::const_iterator const_PlayerVectorIterator;
    virtual const_PlayerVectorIterator begin() const {return players.begin();}
    virtual const_PlayerVectorIterator end() const {return players.end();}

private:
    void createPlayer(std::string name, LineInfo& lineInfo);

    bool log = false;
    std::string playerRunningProgram;
    PlayerFactoryInterface* playerFactory = nullptr;
    std::vector<PlayerInterface*> players;
};


#endif  // PLAYER_VECTOR_H