#include "affected_player_vector.h"
#include "damage.h"
#include "heal.h"
#include "line_info.h"
#include "nano.h"
#include "player_vector_commons.h"

#include <algorithm>


void AffectedPlayerVector::addToPlayers(LineInfo& lineInfo) {
    playerVectorCommons::addToPlayers(lineInfo,
                                      players,
                                      affectedPlayerFactory,
                                      true);
}

void AffectedPlayerVector::createPlayer(std::string name,
                                        LineInfo& lineInfo) {
    std::shared_ptr<AffectedPlayerInterface> affectedPlayer =
        affectedPlayerFactory->createPlayer(name);
    affectedPlayer->add(lineInfo);
    players.push_back(affectedPlayer);
}

size_t AffectedPlayerVector::getLongestNameLength() const {
    return playerVectorCommons::getLongestNameLength(players);
}

AffectedPlayerInterface* AffectedPlayerVector::getPlayer(std::string name) {
    return playerVectorCommons::getPlayer<AffectedPlayerInterface>(name,
                                                                   players);
}

Damage
AffectedPlayerVector::getTotalDamageReceivedFromPlayer(
    std::string callerName) const {
    /* Sums all AffectedPlayers damage to get the total damage
    received from the owning Player.
    The damage belonging to owning Player (caller) should not be
    included in the sum because it already exists in the other
    AffectedPlayers damage stats. If it is added it will incorrectly
    increase the values. */

    Damage d;
    for (const auto& ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            d += ap->getTotalDamageReceivedFromPlayer();
        }
    }
    return d;
}

Damage
AffectedPlayerVector::getTotalDamageDealtOnPlayer(
    std::string callerName) const {
    /* Sums all AffectedPlayers damage to get the total damage dealt
    on the owning Player.
    The damage belonging to owning Player (caller) should not be
    included in the sum because it already exists in the other
    AffectedPlayers damage stats. If it is added it will incorrectly
    increase the values. */

    Damage d;
    for (const auto& ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            d += ap->getTotalDamageDealtOnPlayer();
        }
    }
    return d;
}

std::vector<std::pair<std::string, Damage>>
AffectedPlayerVector::getTotalDamageReceivedFromPlayerPerAffectedPlayer(
    std::string callerName) const {
    /* Returns a vector of pairs containing the Affected players name and their
    total damage (in the form of the Damage class). AffectedPlayers with
    an empty Damage are not included. */
    std::vector<std::pair<std::string, Damage>> totalDamagePerPlayer;
    for (const auto& ap : this->players) {
        Damage d = ap->getTotalDamageReceivedFromPlayer();
        if (ap->getName() != callerName &&  // If not owner of the vector
            d.getCount() != 0) {  // If the Damage isn't empty
            totalDamagePerPlayer.emplace_back(ap->getName(), d);
        }
    }
    return totalDamagePerPlayer;
}

std::vector<std::pair<std::string, Damage>>
AffectedPlayerVector::getTotalDamageDealtOnPlayerPerAffectedPlayer(
    std::string callerName) const {
    /* Returns a vector of pairs containing the Affected players name and their
    total damage (in the form of the Damage class). AffectedPlayers with
    an empty Damage are not included. */
    std::vector<std::pair<std::string, Damage>> totalDamagePerPlayer;
    for (const auto& ap : this->players) {
        Damage d = ap->getTotalDamageDealtOnPlayer();
        if (ap->getName() != callerName &&  // If not owner of the vector
            d.getCount() != 0) {  // If the Damage isn't empty
            totalDamagePerPlayer.emplace_back(ap->getName(), d);
        }
    }
    return totalDamagePerPlayer;
}

std::vector<std::pair<std::string, Damage>>
AffectedPlayerVector::getTotalDamageReceivedFromPlayerPerDamageType(
    std::string callerName) const {
    /* Returns a vector of pairs containing the damage type name
    and it's summed damage. The Damage includes all damage types for both
    dealt and received damage. */
    // TODO: Write a test

    std::vector<std::pair<std::string, Damage>> allDamageTypes;
    for (const auto& ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            addToVector(allDamageTypes, ap->getDamageReceivedFromPlayer());
        }
    }
    return allDamageTypes;
}

std::vector<std::pair<std::string, Damage>>
AffectedPlayerVector::getTotalDamageDealtOnPlayerPerDamageType(
    std::string callerName) const {
    /* Returns a vector of pairs containing the damage type name
    and it's summed damage. The Damage includes all damage types for both
    dealt and received damage. */
    // TODO: Write a test

    std::vector<std::pair<std::string, Damage>> allDamageTypes;
    for (const auto& ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            addToVector(allDamageTypes, ap->getDamageDealtOnPlayer());
        }
    }
    return allDamageTypes;
}

std::vector<std::pair<std::string, Damage>>
AffectedPlayerVector::getDamageReceivedFromPlayer(
    std::string affectedPlayerName) const {

    for (const auto& ap : this->players) {
        if (ap->getName() == affectedPlayerName &&
            ap->getDamageReceivedFromPlayer().size() != 0) {
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

std::vector<std::pair<std::string, Damage>>
AffectedPlayerVector::getDamageDealtOnPlayer(
    std::string affectedPlayerName) const {

    for (const auto& ap : this->players) {
        if (ap->getName() == affectedPlayerName &&
            ap->getDamageDealtOnPlayer().size() != 0) {
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

Heal
AffectedPlayerVector::getTotalHeals(std::string callerName) {
    Heal h;
    for (const auto& ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            h += ap->getHeal();
        }
    }
    return h;
}

std::vector<std::pair<std::string, Heal>>
AffectedPlayerVector::getHealsPerAffectedPlayer() const {
    // TODO: This will include the owning player as well. Do I want that?
    std::vector<std::pair<std::string, Heal>> healsPerPlayer;
    for (const auto& ap : this->players) {
        healsPerPlayer.emplace_back(ap->getName(), ap->getHeal());
    }
    std::sort(healsPerPlayer.begin(), healsPerPlayer.end(),
        [](const std::pair<std::string, Heal>& p1,
           const std::pair<std::string, Heal>& p2) {
               return p1.second.getPotentialDealtOnPlayer() >
                      p2.second.getPotentialDealtOnPlayer();
        });

    return healsPerPlayer;
}

Heal
AffectedPlayerVector::getHeal(
    std::string affectedPlayerName) const {

    for (const auto& ap : this->players) {
        if (ap->getName() == affectedPlayerName) {
            return ap->getHeal();
        }
    }

    // Return empty Heal if affected player was not found.
    Heal h;
    return h;
}

Nano
AffectedPlayerVector::getTotalNano(std::string callerName) const {
    Nano n;
    for (const auto& ap : this->players) {
        if (ap->getName() != callerName) {  // If not owner of the vector
            n += ap->getNano();
        }
    }
    return n;
}

std::vector<std::pair<std::string, Nano>>
AffectedPlayerVector::getNanoPerAffectedPlayer() const {
    std::vector<std::pair<std::string, Nano>> nanoPerPlayer;
    for (const auto& ap : this->players) {
        nanoPerPlayer.emplace_back(ap->getName(), ap->getNano());
    }
    std::sort(nanoPerPlayer.begin(), nanoPerPlayer.end(),
        [](const std::pair<std::string, Nano>& p1,
           const std::pair<std::string, Nano>& p2) {
               return p1.second.getTotalDealtOnPlayer() >
                      p2.second.getTotalDealtOnPlayer();
        });

    return nanoPerPlayer;
}

Nano
AffectedPlayerVector::getNano(
    std::string affectedPlayerName) const {

    for (const auto& ap : this->players) {
        if (ap->getName() == affectedPlayerName) {
            return ap->getNano();
        }
    }

    // Return empty Nano if affected player was not found.
    Nano n;
    return n;
}

void
AffectedPlayerVector::addToVector(
    std::vector<std::pair<std::string, Damage>>& allDamageTypes,
    std::vector<std::pair<std::string, Damage>> apsDamageTypes) const {
    /* Adds a string/Damage pair in apsDamageTypes to allDamageTypes */

    for (const auto& damagePair : apsDamageTypes) {
        // Check if the damage type already exists in the vector
        auto it = std::find_if(
            allDamageTypes.begin(),
            allDamageTypes.end(),
            [&damagePair](const std::pair<std::string, Damage> p) {
            return damagePair.first == p.first; });

        if (it != allDamageTypes.end()) {
            // If it exists, add it to the existing one
            it->second += damagePair.second;
        }
        else {  // Otherwise create a new one.
            allDamageTypes.push_back(damagePair);
        }
    }
}

