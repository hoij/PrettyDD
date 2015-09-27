#include "affected_player.h"
#include "logger.h"
#include "line_info.h"
#include "my_time.h"


AffectedPlayer::AffectedPlayer(std::string name, MyTime* myTime) :
    name(name), myTime(myTime) {}

void AffectedPlayer::add(LineInfo& lineInfo) {
    if (startTime == 0) {
        startTime = lineInfo.time;
    }
    if (lineInfo.type == "damage") {
        addDamage(lineInfo);
    }
    else if (lineInfo.type == "heal") {
        addHeal(lineInfo);
    }
    else if (lineInfo.type == "nano") {
        addNano(lineInfo);
    }
}

void AffectedPlayer::addDamage(LineInfo& li) {
    if (li.dealer_name == getName()) {
        damage[li.subtype].addDamageDealtOnPlayer(li);
    }
    else if (li.receiver_name == getName()) {
        damage[li.subtype].addDamageReceivedFromPlayer(li);
    }
    else {
        // TODO: Exception?
    }
}

void AffectedPlayer::addHeal(LineInfo& li) {
    /*
    Heals can come it three ways:
        1. "You got healed by Player for x."
        2. "You were healed for x."
        3. "You healed Player for x."
    There will always be a receiver and it will always be "You" because heals
    for other players can not be logged. This means that no AffectedPlayer
    other than "You" will have a received heal.

    The two subtypes of heals are potential and actual.
    A potential heal is a heal where the amount is what the caster rolled
    when executing the nano program. Both 1 and 3 are potential heals. An
    actual heal is the amount received. For example, if a player heals you for
    the potential amount of 3000 but you have only lost 1 HP, then the actual
    heal will be 1 point. If the subtype is not "potential", it is "actual"
    which only happens for case 2.
    */

    if (li.subtype == "actual") {
        heal.addHealReceivedFromPlayer(li);
    }
    else if (li.subtype == "potential") {
        if (li.dealer_name == getName()) {
            heal.addHealDealtOnPlayer(li);
        }
        else if (li.receiver_name == getName()) {
            heal.addHealReceivedFromPlayer(li);
        }
        else {
            // TODO: Throw exception?
        }
    }
    else {
        // TODO: Throw exception?
    }
}

void AffectedPlayer::addNano(LineInfo& li) {
    if (li.receiver_name == getName()) {
        nano.addNanoReceivedFromPlayer(li);
    }
    else if (li.dealer_name == getName()) {
        nano.addNanoDealtOnPlayer(li);
    }
}

Damage AffectedPlayer::getTotalDamage() const {
    Damage d;
    for (const auto& type : damage) {
        d += type.second;
    }
    return d;
}

Damage AffectedPlayer::getDamagePerDamageType(const std::string damageType) const {
    auto it = damage.find(damageType);
    if (it != damage.end()) {
        return it->second;
    }
    else {
        Damage d;
        return d;  // Empty Damage
    }
}

std::vector<std::pair<std::string, Damage>> AffectedPlayer::getAllDamage() const {
    // Copy the data to a sortable container
    std::vector<std::pair<std::string, Damage>> sortableDamage;
    for (const auto& damagePair : damage) {
        sortableDamage.push_back(damagePair);
    }
    return sortableDamage;
}

std::vector<std::pair<std::string, Damage>>
AffectedPlayer::getAllDamageReceivedFromPlayer() const {
    // Gets all damage types that the affected player has received
    // from the owning Player. Stores it in a sortable container.
    std::vector<std::pair<std::string, Damage>> sortableDamage;
    for (const auto& damagePair : damage) {
        if (damagePair.second.getCountReceivedFromPlayer() > 0) {
            sortableDamage.push_back(damagePair);
        }
        // Using this opportunity to check that a Damage in the map
        // always has a positive count on either or both of
        // dealt or received.
        else if (damagePair.second.getCountDealtOnPlayer() <= 0 &&
                 damagePair.second.getCountReceivedFromPlayer() <= 0) {
            errorLog.write("Damage of type \"" + damagePair.first +
                           "\" has 0 count when it should have at least 1.");
        }
    }
    return sortableDamage;
}

std::vector<std::pair<std::string, Damage>>
AffectedPlayer::getAllDamageDealtOnPlayer() const {
    // Gets all damage types that the affected player has dealt
    // on the owning Player. Stores it in a sortable container.
    std::vector<std::pair<std::string, Damage>> sortableDamage;
    for (const auto& damagePair : damage) {
        if (damagePair.second.getCountDealtOnPlayer() > 0) {
            sortableDamage.push_back(damagePair);
        }
        // Using this opportunity to check that a Damage in the map
        // always has a positive count on either or both of
        // dealt or received.
        else if (damagePair.second.getCountDealtOnPlayer() <= 0 &&
                 damagePair.second.getCountReceivedFromPlayer() <= 0) {
            errorLog.write("Damage of type \"" + damagePair.first +
                           "\" has 0 count when it should have at least 1.");
        }
    }
    return sortableDamage;
}

const Heal& AffectedPlayer::getHeal() const {
    return heal;
}

const Nano& AffectedPlayer::getNano() const {
    return nano;
}

void AffectedPlayer::incrementPauseDuration(
    const std::time_t& duration) {

    pauseDuration += duration;
}
