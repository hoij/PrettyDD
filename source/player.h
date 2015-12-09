#ifndef PLAYER_H
#define PLAYER_H


#include "affected_player_vector.h"
#include "nano_programs.h"
#include "player_interface.h"
#include "player_time.h"
#include "xp.h"

#include <ctime>
#include <map>
#include <memory>
#include <string>
#include <vector>


#ifdef WIN32
#pragma warning(disable : 4100)
#define NOEXCEPT
#else
#define NOEXCEPT noexcept
#endif


class Damage;
class Heal;
class LineInfo;
class Nano;

class Player : public PlayerInterface {
// TODO: See if Player can be split into smaller classes.
public:
    Player(std::string name,
           std::shared_ptr<AffectedPlayerVector> affectedPlayers,
           std::unique_ptr<PlayerTime> playerTime);
    ~Player() {}

    void add(LineInfo& lineInfo);
    std::string getName() const {return name;}

    /* Damage */
    Damage getTotalDamageDealt() const;
    Damage getTotalDamageReceived() const;

    std::vector<std::pair<std::string, Damage>>
    getTotalDamageDealtPerType() const;

    std::vector<std::pair<std::string, Damage>>
    getTotalDamageReceivedPerType() const;

    std::vector<std::pair<std::string, Damage>>
    getTotalDamageDealtPerAffectedPlayer() const;

    std::vector<std::pair<std::string, Damage>>
    getTotalDamageReceivedPerAffectedPlayer() const;

    std::vector<std::pair<std::string, Damage>>
    getDamageReceivedPerType(std::string affectedPlayerName) const;

    std::vector<std::pair<std::string, Damage>>
    getDamageDealtPerType(std::string affectedPlayerName) const;

    /* Heal */
    Heal getTotalHeals() const;

    std::vector<std::pair<std::string, Heal>>
    getHealsPerAffectedPlayer() const;

    Heal getHeal(std::string affectedPlayerName) const;

    /* Nano */
    Nano getTotalNano() const;

    std::vector<std::pair<std::string, Nano>>
    getNanoPerAffectedPlayer() const;

    Nano getNano(std::string affectedPlayerName) const;

    /* Nano Program */
    const NanoPrograms& getNanoPrograms() const;

    /* XP */
    const XP& getXp();

    /* Time */
    std::time_t getStartTime() const;
    std::time_t getTimeActive() const;
    void stopTimer();
    void resumeTimer();

    size_t getLongestAffectedPlayerNameLength() const;
    std::vector<AffectedPlayer*>::size_type nrOfAffectedPlayers();

private:
    std::string name;

    void addDPM(std::vector<std::pair<std::string, Damage>>& v) const;

    void addXp(LineInfo& li);
    Damage sumDamage(bool nanobots);
    Damage sumDamageType(const std::string damageType, bool nanobots);

    std::shared_ptr<AffectedPlayerVector> affectedPlayers = nullptr;
    std::unique_ptr<PlayerTime> playerTime = nullptr;
	NanoPrograms nanoPrograms;
    XP xp;
};


#endif
