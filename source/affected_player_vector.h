#ifndef AFFECTED_PLAYER_VECTOR_H
#define AFFECTED_PLAYER_VECTOR_H


#include "affected_player.h"
#include "base_vector.h"
#include "damage.h"
#include "heal.h"
#include "line_info.h"
#include "logger.h"
#include "my_time.h"

#include <algorithm>
#include <iterator>
#include <string>
#include <utility>
#include <vector>


template<class C>
class AffectedPlayerVector : public BaseVector<C> {
public:
    AffectedPlayerVector() {}
    virtual ~AffectedPlayerVector() {};
    AffectedPlayerVector(const AffectedPlayerVector<C>& other) : BaseVector<C>(other) {}

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

private:
    void addToVector(
        std::vector<std::pair<std::string, Damage>>& allDamageTypes,
        std::vector<std::pair<std::string, Damage>>& apsDamageTypes) const;

    static bool compareNanoDealt(const std::pair<std::string, Nano>& p1,
                                 const std::pair<std::string, Nano>& p2);
    static bool comparePotentialHeal(const std::pair<std::string, Heal>& p1,
                                     const std::pair<std::string, Heal>& p2);
};


template<class C>
Damage
AffectedPlayerVector<C>::getTotalDamageReceivedFromPlayer(std::string callerName) const {
    /* Sums all AffectedPlayers damage to get the total damage
    received from the owning Player.
    The damage belonging to owning Player (caller) should not be
    included in the sum because it already exists in the other
    AffectedPlayers damage stats. If it is added it will incorrectly
    increase the values. */

    Damage d;
    for (const C ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            d += ap->getTotalDamageReceivedFromPlayer();
        }
    }
    return d;
}

template<class C>
Damage
AffectedPlayerVector<C>::getTotalDamageDealtOnPlayer(std::string callerName) const {
    /* Sums all AffectedPlayers damage to get the total damage dealt
    on the owning Player.
    The damage belonging to owning Player (caller) should not be
    included in the sum because it already exists in the other
    AffectedPlayers damage stats. If it is added it will incorrectly
    increase the values. */

    Damage d;
    for (const C ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            d += ap->getTotalDamageDealtOnPlayer();
        }
    }
    return d;
}

template<class C>
std::vector<std::pair<std::string, Damage>>
AffectedPlayerVector<C>::getTotalDamageReceivedFromPlayerPerAffectedPlayer(
    std::string callerName) const {
    /* Returns a vector of pairs containing the Affected players name and their
    total damage (in the form of the Damage class). */
    std::vector<std::pair<std::string, Damage>> totalDamagePerPlayer;
    for (const C ap : this->players) {
        Damage d = ap->getTotalDamageReceivedFromPlayer();
        if (ap->getName() != callerName) {  // If not owner of the vector
            totalDamagePerPlayer.emplace_back(ap->getName(), d);
        }
    }
    return totalDamagePerPlayer;
}

template<class C>
std::vector<std::pair<std::string, Damage>>
AffectedPlayerVector<C>::getTotalDamageDealtOnPlayerPerAffectedPlayer(
    std::string callerName) const {
    /* Returns a vector of pairs containing the Affected players name and their
    total damage (in the form of the Damage class). */
    std::vector<std::pair<std::string, Damage>> totalDamagePerPlayer;
    for (const C ap : this->players) {
        Damage d = ap->getTotalDamageDealtOnPlayer();
        if (ap->getName() != callerName) {  // If not owner of the vector
            totalDamagePerPlayer.emplace_back(ap->getName(), d);
        }
    }
    return totalDamagePerPlayer;
}

template<class C>
std::vector<std::pair<std::string, Damage>>
AffectedPlayerVector<C>::getTotalDamageReceivedFromPlayerPerDamageType(
    std::string callerName) const {
    /* Returns a vector of pairs containing the damage type name
    and it's summed damage. The Damage includes all damage types for both
    dealt and received damage. */
    // TODO: Write a test

    std::vector<std::pair<std::string, Damage>> allDamageTypes;
    for (const C ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            std::vector<std::pair<std::string, Damage>> apsDamageTypes =
                ap->getDamageReceivedFromPlayer();
            addToVector(allDamageTypes, apsDamageTypes);
        }
    }
    return allDamageTypes;
}

template<class C>
std::vector<std::pair<std::string, Damage>>
AffectedPlayerVector<C>::getTotalDamageDealtOnPlayerPerDamageType(
    std::string callerName) const {
    /* Returns a vector of pairs containing the damage type name
    and it's summed damage. The Damage includes all damage types for both
    dealt and received damage. */
    // TODO: Write a test

    std::vector<std::pair<std::string, Damage>> allDamageTypes;
    for (const C ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            std::vector<std::pair<std::string, Damage>> apsDamageTypes =
                ap->getDamageDealtOnPlayer();
            addToVector(allDamageTypes, apsDamageTypes);
        }
    }
    return allDamageTypes;
}

template<class C>
std::vector<std::pair<std::string, Damage>>
AffectedPlayerVector<C>::getDamageReceivedFromPlayer(
    std::string affectedPlayerName) const {

    for (const C ap : this->players) {
        if (ap->getName() == affectedPlayerName) {
            return ap->getDamageReceivedFromPlayer();
        }
    }

    // An affected player may return an empty vector if no damage has
    // been done to it. So to differentiate from that case and the case of
    // a non-existing player a pair with the key "empty" is returned.
    Damage d;
    std::vector<std::pair<std::string, Damage>> emptyVector;
    emptyVector.emplace_back("empty", d);
    return emptyVector;
}

template<class C>
std::vector<std::pair<std::string, Damage>>
AffectedPlayerVector<C>::getDamageDealtOnPlayer(
    std::string affectedPlayerName) const {

    for (const C ap : this->players) {
        if (ap->getName() == affectedPlayerName) {
            return ap->getDamageDealtOnPlayer();
        }
    }

    // An affected player may return an empty vector if no damage has
    // been done to it. So to differentiate from that case and the case of
    // a non-existing player a pair with the key "empty" is returned.
    Damage d;
    std::vector<std::pair<std::string, Damage>> emptyVector;
    emptyVector.emplace_back("empty", d);
    return emptyVector;
}

template<class C>
Heal
AffectedPlayerVector<C>::getTotalHeals(std::string callerName) {
    Heal h;
    for (const C ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            h += ap->getHeal();
        }
    }
    return h;
}

template<class C>
std::vector<std::pair<std::string, Heal>>
AffectedPlayerVector<C>::getHealsPerAffectedPlayer() const {
    // TODO: This will include the owning player as well. Do I want that?
    std::vector<std::pair<std::string, Heal>> healsPerPlayer;
    for (const C ap : this->players) {
        healsPerPlayer.emplace_back(ap->getName(), ap->getHeal());
    }
    std::sort(healsPerPlayer.begin(),
              healsPerPlayer.end(),
              comparePotentialHeal);
    return healsPerPlayer;
}

template<class C>
Heal
AffectedPlayerVector<C>::getHeal(
    std::string affectedPlayerName) const {

    for (const C ap : this->players) {
        if (ap->getName() == affectedPlayerName) {
            return ap->getHeal();
        }
    }

    // Return empty Heal if affected player was not found.
    Heal h;
    return h;
}

template<class C>
Nano
AffectedPlayerVector<C>::getTotalNano(std::string callerName) const {
    Nano n;
    for (const C ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            n += ap->getNano();
        }
    }
    return n;
}

template<class C>
std::vector<std::pair<std::string, Nano>>
AffectedPlayerVector<C>::getNanoPerAffectedPlayer() const {
    std::vector<std::pair<std::string, Nano>> nanoPerPlayer;
    for (const C ap : this->players) {
            nanoPerPlayer.emplace_back(ap->getName(), ap->getNano());
    }
    std::sort(nanoPerPlayer.begin(),
              nanoPerPlayer.end(),
              compareNanoDealt);
    return nanoPerPlayer;
}

template<class C>
Nano
AffectedPlayerVector<C>::getNano(
    std::string affectedPlayerName) const {

    for (const C ap : this->players) {
        if (ap->getName() == affectedPlayerName) {
            return ap->getNano();
        }
    }

    // Return empty Nano if affected player was not found.
    Nano n;
    return n;
}

template<class C>
void
AffectedPlayerVector<C>::addToVector(
    std::vector<std::pair<std::string, Damage>>& allDamageTypes,
    std::vector<std::pair<std::string, Damage>>& apsDamageTypes) const {
    /* Adds a string/Damage pair in apsDamageTypes to allDamageTypes */

    for (const auto& damagePair : apsDamageTypes) {
        // Check if the damage type already exists in the vector
        auto it = std::find_if(
            allDamageTypes.begin(),
            allDamageTypes.end(),
            [&damagePair](const std::pair<std::string, Damage> p) {
                return damagePair.first == p.first;});

        if (it != allDamageTypes.end() ) {
            // If it exists, add it to the existing one
            it->second += damagePair.second;
        }
        else {
            // Otherwise create a new one.
            allDamageTypes.push_back(damagePair);
        }
    }
}

template<class C>
bool
AffectedPlayerVector<C>::compareNanoDealt(
    const std::pair<std::string, Nano>& p1,
    const std::pair<std::string, Nano>& p2) {

    return p1.second.getTotalDealtOnPlayer() >
           p2.second.getTotalDealtOnPlayer();
}

template<class C>
bool
AffectedPlayerVector<C>::comparePotentialHeal(
    const std::pair<std::string, Heal>& p1,
    const std::pair<std::string, Heal>& p2) {

    return p1.second.getPotentialDealtOnPlayer() >
           p2.second.getPotentialDealtOnPlayer();
}


#endif // AFFECTED_PLAYER_VECTOR_H
