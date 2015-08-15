#ifndef AFFECTED_PLAYER_VECTOR_H
#define AFFECTED_PLAYER_VECTOR_H

#include "logger.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <string>
#include <vector>

class AffectedPlayer;
class Damage;
class Heal;
class LineInfo;


class AffectedPlayerVector {
public:
    virtual ~AffectedPlayerVector() {};
    virtual void addToPlayers(LineInfo& lineInfo);
    virtual int getLongestNameLength() const;
    virtual const AffectedPlayer* getPlayer(std::string name);

    virtual Damage getTotalDamage(std::string name, bool nanobots);
    virtual Damage getTotalDamagePerDamageType(std::string name,
                                               const std::string damageType,
                                               bool nanobots);
    virtual std::vector<std::pair<std::string, Damage>> getTotalDamageForEachPlayer(std::string name) const;
    const std::map<std::string, Damage>& getNanobotsDamagePerAffectedPlayer(std::string name) const;
    const std::map<std::string, Damage>& getRegularDamagePerAffectedPlayer(std::string name) const;

    virtual Heal getTotalHeals(std::string name);
    std::vector<std::pair<std::string, Heal>> getHealsForEachAffectedPlayer() const;

    typedef typename std::vector<AffectedPlayer>::iterator AffectedPlayerVectorIterator;
    virtual AffectedPlayerVectorIterator begin() {return players.begin();}
    virtual AffectedPlayerVectorIterator end() {return players.end();}
    typedef typename std::vector<AffectedPlayer>::const_iterator const_AffectedPlayerVectorIterator;
    virtual const_AffectedPlayerVectorIterator begin() const {return players.begin();}
    virtual const_AffectedPlayerVectorIterator end() const {return players.end();}

private:
    void createPlayer(std::string name, LineInfo& lineInfo);
    static bool compareTotalReceivedFromPlayer(std::pair<std::string, Damage>& p1,
                                               std::pair<std::string, Damage>& p2);

    std::vector<AffectedPlayer> players;
};

#endif // AFFECTED_PLAYER_VECTOR_H
