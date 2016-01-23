#ifndef AFFECTED_PLAYER_VECTOR_H
#define AFFECTED_PLAYER_VECTOR_H


#include "affected_player_factory_interface.h"
#include "affected_player_interface.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>


class Damage;
class Heal;
class Nano;

class AffectedPlayerVector {
public:
    AffectedPlayerVector(std::unique_ptr<AffectedPlayerFactoryInterface> affectedPlayerFactory) :
        affectedPlayerFactory(std::move(affectedPlayerFactory)) {}
    virtual ~AffectedPlayerVector() {}

    std::vector<AffectedPlayerInterface*>::size_type size() {return players.size();}

    virtual void addToPlayers(LineInfo& lineInfo);
    virtual size_t getLongestNameLength() const;
    virtual AffectedPlayerInterface* getPlayer(std::string name);

    /* Damage */
    virtual Damage
    getTotalDamageReceivedFromPlayer(std::string callerName) const;

    virtual Damage
    getTotalDamageDealtOnPlayer(std::string callerName) const;

    virtual std::vector<std::pair<std::string, Damage>>
    getTotalDamageReceivedFromPlayerPerDamageType(
        std::string callerName) const;

    virtual std::vector<std::pair<std::string, Damage>>
    getTotalDamageDealtOnPlayerPerDamageType(
        std::string callerName) const;

    virtual std::vector<std::pair<std::string, Damage>>
    getTotalDamageReceivedFromPlayerPerAffectedPlayer(
        std::string callerName) const;

    virtual std::vector<std::pair<std::string, Damage>>
    getTotalDamageDealtOnPlayerPerAffectedPlayer(
        std::string callerName) const;

    virtual std::vector<std::pair<std::string, Damage>>
    getDamageReceivedFromPlayer(std::string affectedPlayerName) const;

    virtual std::vector<std::pair<std::string, Damage>>
    getDamageDealtOnPlayer(std::string affectedPlayerName) const;

    /* Heal */
    virtual Heal getTotalHeals(std::string callerName);

    virtual std::vector<std::pair<std::string, Heal>>
    getHealsPerAffectedPlayer() const;

    virtual Heal
    getHeal(std::string affectedPlayerName) const;

    /* Nano */
    virtual Nano getTotalNano(std::string callerName) const;

    virtual std::vector<std::pair<std::string, Nano>>
    getNanoPerAffectedPlayer() const;

    virtual Nano
    getNano(std::string affectedPlayerName) const;

    typedef std::vector<std::shared_ptr<AffectedPlayerInterface>>::iterator PlayerVectorIterator;
    virtual PlayerVectorIterator begin() { return players.begin(); }
    virtual PlayerVectorIterator end() { return players.end(); }
    typedef std::vector<std::shared_ptr<AffectedPlayerInterface>>::const_iterator const_PlayerVectorIterator;
    virtual const_PlayerVectorIterator begin() const { return players.begin(); }
    virtual const_PlayerVectorIterator end() const { return players.end(); }

private:
    void addToVector(
        std::vector<std::pair<std::string, Damage>>& allDamageTypes,
        std::vector<std::pair<std::string, Damage>> apsDamageTypes) const;

    std::unique_ptr<AffectedPlayerFactoryInterface> affectedPlayerFactory = nullptr;
    std::vector<std::shared_ptr<AffectedPlayerInterface>> players;
};


#endif // AFFECTED_PLAYER_VECTOR_H
