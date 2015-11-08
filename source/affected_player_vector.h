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
    AffectedPlayerVector(AffectedPlayerFactoryInterface* affectedPlayerFactory) :
        affectedPlayerFactory(affectedPlayerFactory) {}
    virtual ~AffectedPlayerVector() {}
    AffectedPlayerVector(const AffectedPlayerVector& other);
    AffectedPlayerVector(AffectedPlayerVector&& other);
    AffectedPlayerVector& operator=(AffectedPlayerVector rhs);
    friend void swap(AffectedPlayerVector& first, AffectedPlayerVector& second);

    std::vector<AffectedPlayerInterface*>::size_type size() {return players.size();}

    virtual void addToPlayers(LineInfo& lineInfo);
    virtual size_t getLongestNameLength() const;
    virtual std::shared_ptr<AffectedPlayerInterface> getPlayer(std::string name);

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
    void createPlayer(std::string name, LineInfo& lineInfo);

    void addToVector(
        std::vector<std::pair<std::string, Damage>>& allDamageTypes,
        std::vector<std::pair<std::string, Damage>> apsDamageTypes) const;

    static bool compareNanoDealt(const std::pair<std::string, Nano>& p1,
                                 const std::pair<std::string, Nano>& p2);
    static bool comparePotentialHeal(const std::pair<std::string, Heal>& p1,
                                     const std::pair<std::string, Heal>& p2);

    AffectedPlayerFactoryInterface* affectedPlayerFactory = nullptr;
    std::vector<std::shared_ptr<AffectedPlayerInterface>> players;
};


#endif // AFFECTED_PLAYER_VECTOR_H
