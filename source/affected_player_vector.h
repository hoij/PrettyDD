#ifndef AFFECTED_PLAYER_VECTOR_H
#define AFFECTED_PLAYER_VECTOR_H


#include "affected_player.h"
#include "base_vector.h"
#include "damage.h"
#include "heal.h"
#include "line_info.h"
#include "logger.h"

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

    virtual Damage getTotalDamage(std::string callerName, bool nanobots);
    virtual Damage getTotalDamagePerDamageType(std::string callerName,
                                               const std::string damageType,
                                               bool nanobots);
    virtual std::vector<std::pair<std::string, Damage>> getTotalDamageForEachPlayer(std::string callerName) const;
    virtual const std::map<std::string, Damage>& getNanobotsDamagePerAffectedPlayer(std::string name) const;
    virtual const std::map<std::string, Damage>& getRegularDamagePerAffectedPlayer(std::string name) const;

    virtual Heal getTotalHeals(std::string callerName);
    std::vector<std::pair<std::string, Heal>> getHealsForEachAffectedPlayer() const;

    Nano getTotalNano(std::string callerName) const;
    std::vector<std::pair<std::string, Nano>> getNanoForEachAffectedPlayer(std::string callerName) const;

private:
    static bool comparePotentialHeal(const std::pair<std::string, Heal>& p1,
                                     const std::pair<std::string, Heal>& p2);
};


template<class C>
Damage AffectedPlayerVector<C>::getTotalDamage(std::string callerName, bool nanobots) {
    /* The damage belonging to the caller should not be included in the sum
    because it already exists in the other players damage stats. If it  is
    added it will incorrectly increase the values. */

    Damage d;
    for (const C ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            d += ap->getTotalDamage(nanobots);
        }
    }
    return d;
}

template<class C>
Damage AffectedPlayerVector<C>::getTotalDamagePerDamageType(std::string callerName, const std::string damageType, bool nanobots) {
    Damage d;
    for (const C ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            d += ap->getTotalDamagePerDamageType(damageType, nanobots);
        }
    }
    return d;
}

template<class C>
std::vector<std::pair<std::string, Damage>> AffectedPlayerVector<C>::getTotalDamageForEachPlayer(std::string callerName) const {
    // Returns a sorted vector of pairs containing the players name and their
    // total damage (in the form of the Damage class).
    std::vector<std::pair<std::string, Damage>> totalDamagePerPlayer;
    for (const C ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            totalDamagePerPlayer.push_back(
                std::make_pair(ap->getName(), ap->getTotalDamage()));
        }
    }
    std::sort(totalDamagePerPlayer.begin(),
              totalDamagePerPlayer.end(),
              this->compareTotalReceivedFromPlayer);
    return totalDamagePerPlayer;
}

template<class C>
const std::map<std::string, Damage>& AffectedPlayerVector<C>::getNanobotsDamagePerAffectedPlayer(std::string name) const {
    for (const C ap : this->players) {
        if (ap->getName() == name) {
            return ap->getNanobotsDamage();
        }
    }
    // TODO: Catch and log this error somwhere.
    throw std::invalid_argument("\"" + name + "\" was not found in the " +
                                "affected players");
}

template<class C>
const std::map<std::string, Damage>& AffectedPlayerVector<C>::getRegularDamagePerAffectedPlayer(std::string name) const {
    for (const C ap : this->players) {
        if (ap->getName() == name) {
            return ap->getRegularDamage();
        }
    }
    // TODO: Catch and log this error somwhere.
    throw std::invalid_argument("\"" + name + "\" was not found in the " +
                                "affected players list");
}

template<class C>
Heal AffectedPlayerVector<C>::getTotalHeals(std::string callerName) {
    Heal h;
    for (const C ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            h += ap->getHeal();
        }
    }
    return h;
}

template<class C>
std::vector<std::pair<std::string, Heal>> AffectedPlayerVector<C>::getHealsForEachAffectedPlayer() const {
    // TODO: This will include the owning player as well. Do I want that?
    std::vector<std::pair<std::string, Heal>> healsPerPlayer;
    for (const C ap : this->players) {
        healsPerPlayer.push_back(std::make_pair(ap->getName(), ap->getHeal()));
    }
    std::sort(healsPerPlayer.begin(),
              healsPerPlayer.end(),
              comparePotentialHeal);
    return healsPerPlayer;
}

template<class C>
Nano AffectedPlayerVector<C>::getTotalNano(std::string callerName) const {
    Nano n;
    for (const AffectedPlayer* ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            n += ap->getNano();
        }
    }
    return n;
}

template<class C>
std::vector<std::pair<std::string, Nano>> AffectedPlayerVector<C>::getNanoForEachAffectedPlayer(std::string callerName) const {
    std::vector<std::pair<std::string, Nano>> nanoPerPlayer;
    for (const C ap : this->players) {
        if (ap->getName() != callerName) {
            nanoPerPlayer.push_back(std::make_pair(ap->getName(), ap->getNano()));
        }
    }
    return nanoPerPlayer;
}

template<class C>
bool AffectedPlayerVector<C>::comparePotentialHeal(const std::pair<std::string, Heal>& p1,
                                                   const std::pair<std::string, Heal>& p2) {
    return p1.second.getPotentialDealt() >
           p2.second.getPotentialDealt();
}


#endif // AFFECTED_PLAYER_VECTOR_H
