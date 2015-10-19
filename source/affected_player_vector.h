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
#include <map>
#include <string>
#include <utility>
#include <vector>


template<class C>
class AffectedPlayerVector : public BaseVector<C> {
public:
    AffectedPlayerVector() {}
    virtual ~AffectedPlayerVector() {};
    AffectedPlayerVector(const AffectedPlayerVector<C>& other) : BaseVector<C>(other) {}

    virtual Damage getTotalDamage(std::string callerName) const;
    virtual Damage getTotalDamagePerDamageType(std::string callerName,
                                               std::string damageType) const;
    virtual std::vector<std::pair<std::string, Damage>> getTotalDamageForEveryDamageType(std::string callerName) const;

    std::vector<std::pair<std::string, Damage>>
    getTotalDamageForEveryDamageTypeReceivedFromPlayer(
        std::string callerName) const;

    std::vector<std::pair<std::string, Damage>>
    getTotalDamageForEveryDamageTypeDealtOnPlayer(
        std::string callerName) const;

    std::map<std::string, Damage>
    getDamageForAllAffectedPlayers(std::string callerName) const;

    virtual std::vector<std::pair<std::string, Damage>> getTotalDamageForAllAffectedPlayers(std::string callerName) const;
    virtual std::vector<std::pair<std::string, Damage>> getAllDamageFromAffectedPlayer(std::string name) const;

    std::vector<std::pair<std::string, Damage>>
    getAllDamageReceivedFromPlayerForAffectedPlayer(std::string name) const;

    std::vector<std::pair<std::string, Damage>>
    getAllDamageDealtOnPlayerForAffectedPlayer(std::string name) const;

    virtual Heal getTotalHeals(std::string callerName);
    virtual std::vector<std::pair<std::string, Heal>> getHealsForAllAffectedPlayers() const;
    virtual const Heal& getHealFromAffectedPlayer(std::string name) const;

    virtual Nano getTotalNano(std::string callerName) const;
    virtual std::vector<std::pair<std::string, Nano>> getNanoForAllAffectedPlayers() const;
    virtual const Nano& getNanoFromAffectedPlayer(std::string name) const;

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
AffectedPlayerVector<C>::getTotalDamage(std::string callerName) const {
    /* The damage belonging to the caller should not be included in the sum
    because it already exists in the other players damage stats. If it  is
    added it will incorrectly increase the values. */

    Damage d;
    for (const C ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            d += ap->getTotalDamage();
        }
    }
    return d;
}

template<class C>
Damage
AffectedPlayerVector<C>::getTotalDamagePerDamageType(
    std::string callerName,
    std::string damageType) const {

    Damage d;
    for (const C ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            d += ap->getDamagePerDamageType(damageType);
        }
    }
    return d;
}

template<class C>
std::vector<std::pair<std::string, Damage>>
AffectedPlayerVector<C>::getTotalDamageForAllAffectedPlayers(
    std::string callerName) const {
    /* Returns a vector of pairs containing the players name and their
    total damage (in the form of the Damage class). */
    std::vector<std::pair<std::string, Damage>> totalDamagePerPlayer;
    for (const C ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            totalDamagePerPlayer.emplace_back(ap->getName(),
                                              ap->getTotalDamage());
        }
    }
    return totalDamagePerPlayer;
}

template<class C>
std::map<std::string, Damage>
AffectedPlayerVector<C>::getDamageForAllAffectedPlayers(
    std::string callerName) const {
    /* Returns a map containing the affected players name and their
    total damage received from the owning Player. */

    std::map<std::string, Damage> totalDamagePerPlayer;
    for (const C ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            totalDamagePerPlayer[ap->getName()] = ap->getDamage();
        }
    }
    return totalDamagePerPlayer;
}

template<class C>
std::vector<std::pair<std::string, Damage>>
AffectedPlayerVector<C>::getTotalDamageForEveryDamageType(
    std::string callerName) const {
    /* Returns a vector of pairs containing the damage type name
    and it's summed damage. The Damage includes all damage types for both
    dealt and received damage. */
    // TODO: Write a test

    std::vector<std::pair<std::string, Damage>> allDamageTypes;
    for (const C ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            std::vector<std::pair<std::string, Damage>> apsDamageTypes =
                ap->getAllDamage();
            addToVector(allDamageTypes, apsDamageTypes);
        }
    }
    return allDamageTypes;
}

template<class C>
std::vector<std::pair<std::string, Damage>>
AffectedPlayerVector<C>::getTotalDamageForEveryDamageTypeReceivedFromPlayer(
    std::string callerName) const {
    /* Returns a vector of pairs containing the damage type name
    and it's summed damage. The Damage includes all damage types for both
    dealt and received damage. */
    // TODO: Write a test

    std::vector<std::pair<std::string, Damage>> allDamageTypes;
    for (const C ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            std::vector<std::pair<std::string, Damage>> apsDamageTypes =
                ap->getAllDamageReceivedFromPlayer();
            addToVector(allDamageTypes, apsDamageTypes);
        }
    }
    return allDamageTypes;
}

template<class C>
std::vector<std::pair<std::string, Damage>>
AffectedPlayerVector<C>::getTotalDamageForEveryDamageTypeDealtOnPlayer(
    std::string callerName) const {
    /* Returns a vector of pairs containing the damage type name
    and it's summed damage. The Damage includes all damage types for both
    dealt and received damage. */
    // TODO: Write a test

    std::vector<std::pair<std::string, Damage>> allDamageTypes;
    for (const C ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            std::vector<std::pair<std::string, Damage>> apsDamageTypes =
                ap->getAllDamageDealtOnPlayer();
            addToVector(allDamageTypes, apsDamageTypes);
        }
    }
    return allDamageTypes;
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
std::vector<std::pair<std::string, Damage>>
AffectedPlayerVector<C>::getAllDamageFromAffectedPlayer(
    std::string name) const {

    for (const C ap : this->players) {
        if (ap->getName() == name) {
            return ap->getAllDamage();
        }
    }

    // An affected player may return an empty vector if no damage has
    // been done to it. So to differentiate from that case and
    // a non-existing player a pair with the key "empty" is returned.
    Damage d;
    std::vector<std::pair<std::string, Damage>> emptyVector;
    emptyVector.emplace_back("empty", d);
    return emptyVector;
}

template<class C>
std::vector<std::pair<std::string, Damage>>
AffectedPlayerVector<C>::getAllDamageReceivedFromPlayerForAffectedPlayer(
    std::string name) const {

    for (const C ap : this->players) {
        if (ap->getName() == name) {
            return ap->getAllDamageReceivedFromPlayer();
        }
    }

    // An affected player may return an empty vector if no damage has
    // been done to it. So to differentiate from that case and
    // a non-existing player a pair with the key "empty" is returned.
    Damage d;
    std::vector<std::pair<std::string, Damage>> emptyVector;
    emptyVector.emplace_back("empty", d);
    return emptyVector;
}

template<class C>
std::vector<std::pair<std::string, Damage>>
AffectedPlayerVector<C>::getAllDamageDealtOnPlayerForAffectedPlayer(
    std::string name) const {

    for (const C ap : this->players) {
        if (ap->getName() == name) {
            return ap->getAllDamageDealtOnPlayer();
        }
    }

    // An affected player may return an empty vector if no damage has
    // been done to it. So to differentiate from that case and
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
AffectedPlayerVector<C>::getHealsForAllAffectedPlayers() const {
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
const Heal&
AffectedPlayerVector<C>::getHealFromAffectedPlayer(std::string name) const {
    for (const C ap : this->players) {
        if (ap->getName() == name) {
            return ap->getHeal();
        }
    }
    // TODO: Catch and log this error somwhere.
    throw std::invalid_argument("\"" + name + "\" was not found among the " +
                                "affected players.");
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
AffectedPlayerVector<C>::getNanoForAllAffectedPlayers() const {
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
const Nano&
AffectedPlayerVector<C>::getNanoFromAffectedPlayer(std::string name) const {
    for (const C ap : this->players) {
        if (ap->getName() == name) {
            return ap->getNano();
        }
    }
    // TODO: Change this to returning an empty instance of Nano instead?
    // If not, remember to catch the exception.
    throw std::invalid_argument("\"" + name + "\" was not found among the " +
                                "affected players.");
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
