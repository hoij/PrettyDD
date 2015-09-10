#ifndef PLAYER_VECTOR_H
#define PLAYER_VECTOR_H


#include "base_vector.h"
#include "logger.h"
#include "player.h"

#include <algorithm>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

class Damage;
class Heal;
class LineInfo;

template<class C>
class PlayerVector : public BaseVector<C> {
public:
    PlayerVector() {}
    virtual ~PlayerVector() {};
    PlayerVector(const PlayerVector<C>& other) : BaseVector<C>(other) {}

    virtual Damage getTotalDamage() const;
    virtual Damage getTotalDamagePerDamageType(std::string damageType) const;
    virtual std::vector<std::pair<std::string, Damage>> getTotalDamageForEachPlayer() const;
    virtual Heal getTotalHeals() const;
};

template<class C>
Damage PlayerVector<C>::getTotalDamage() const {
    Damage d;
    for (const C p : this->players) {
        d += p->getTotalDamage();
    }
    return d;
}

template<class C>
Damage PlayerVector<C>::getTotalDamagePerDamageType(std::string damageType) const {
    Damage d;
    for (const C p : this->players) {
        d += p->getTotalDamagePerDamageType(damageType);
    }
    return d;
}

template<class C>
std::vector<std::pair<std::string, Damage>> PlayerVector<C>::getTotalDamageForEachPlayer() const {
    /* Returns a vector of pairs containing the players name and their
    total damage (in the form of the Damage class). */
    std::vector<std::pair<std::string, Damage>> totalDamagePerPlayer;
    for (const C p : this->players) {
        totalDamagePerPlayer.push_back(
            std::make_pair(p->getName(), p->getTotalDamage()));
    }
//    std::sort(totalDamagePerPlayer.begin(),
//              totalDamagePerPlayer.end(),
//              this->compareTotalReceivedFromPlayer);
    return totalDamagePerPlayer;
}

template<class C>
Heal PlayerVector<C>::getTotalHeals() const {
    Heal h;
    for (const C p : this->players) {
        h += p->getTotalHeals();
    }
    return h;
}


#endif // PLAYER_VECTOR_H
