#ifndef PLAYER_INTERFACE_H
#define PLAYER_INTERFACE_H


#include <ctime>
#include <map>
#include <string>
#include <vector>


class AffectedPlayer;
class Damage;
class Heal;
class Nano;
class NanoPrograms;
class LineInfo;
class XP;


class PlayerInterface {
public:
    virtual ~PlayerInterface() {}

    virtual void add(LineInfo& lineInfo) = 0;

    virtual std::string getName() const = 0;

    /* Damage */
    virtual Damage getTotalDamageDealt() const = 0;
    virtual Damage getTotalDamageReceived() const = 0;

    virtual std::vector<std::pair<std::string, Damage>>
        getTotalDamageDealtPerType() const = 0;

    virtual std::vector<std::pair<std::string, Damage>>
        getTotalDamageReceivedPerType() const = 0;

    virtual std::vector<std::pair<std::string, Damage>>
        getTotalDamageDealtPerAffectedPlayer() const = 0;

    virtual std::vector<std::pair<std::string, Damage>>
        getTotalDamageReceivedPerAffectedPlayer() const = 0;

    virtual std::vector<std::pair<std::string, Damage>>
        getDamageReceivedPerType(std::string affectedPlayerName) const = 0;

    virtual std::vector<std::pair<std::string, Damage>>
        getDamageDealtPerType(std::string affectedPlayerName) const = 0;

    /* Heal */
    virtual Heal getTotalHeals() const = 0;

    virtual std::vector<std::pair<std::string, Heal>>
    getHealsPerAffectedPlayer() const = 0;

    virtual Heal getHeal(std::string name) const = 0;

    /* Nano */
    virtual Nano getTotalNano() const = 0;

    virtual std::vector<std::pair<std::string, Nano>>
    getNanoPerAffectedPlayer() const = 0;

    virtual Nano getNano(std::string name) const = 0;

    /* Nano Program */
    virtual const NanoPrograms& getNanoPrograms() const = 0;

    /* XP */
    virtual const XP& getXp() = 0;


    /* Time. TODO: Move into its own class */
    virtual std::time_t getTimeActive() const = 0;
    virtual std::time_t getPauseDuration() const = 0;
    virtual std::time_t getStartTime() const = 0;
    virtual void stopTimer() = 0;
    virtual void resumeTimer() = 0;

    virtual size_t getLongestAffectedPlayerNameLength() const = 0;
    virtual std::vector<AffectedPlayer*>::size_type nrOfAffectedPlayers() = 0;
};


#endif  //PLAYER_INTERFACE_H

