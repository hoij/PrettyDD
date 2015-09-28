#ifndef PLAYER_H
#define PLAYER_H


#include "affected_player_vector.h"
#include "damage.h"
#include "heal.h"
#include "nano.h"
#include "nano_program.h"
#include "player_interface.h"
#include "my_time.h"
#include "xp.h"

#include <gtest/gtest_prod.h>
#include <map>
#include <string>
#include <vector>


#ifdef WIN32
#pragma warning(disable : 4100)
#define NOEXCEPT
#else
#define NOEXCEPT noexcept
#endif


class LineInfo;

class Player : public virtual PlayerInterface {
// TODO: Se if Player can be split into smaller classes.
public:
    Player(std::string name);
    Player(std::string name, MyTimeInterface* myTime);
    Player(std::string name,
           AffectedPlayerVector<AffectedPlayer*>* pv,
           MyTimeInterface* myTime);
    ~Player();
    Player(const Player& other);
    Player(Player&& other) NOEXCEPT;
    Player& operator=(Player rhs);
    friend void swap(Player& first, Player& second);

    std::string getName() const {return name;}

    void add(LineInfo& lineInfo);

    Damage getTotalDamage() const;
    Damage getTotalDamagePerDamageType(std::string damageType) const;

    std::vector<std::pair<std::string, Damage>> getTotalDamageForEveryDamageType() const;
    std::vector<std::pair<std::string, Damage>> getTotalDamageForEveryDamageTypeReceivedFromPlayer() const;
    std::vector<std::pair<std::string, Damage>> getTotalDamageForEveryDamageTypeDealtOnPlayer() const;
    std::vector<std::pair<std::string, Damage>> getTotalDamageForAllAffectedPlayers() const;
    std::vector<std::pair<std::string, Damage>> getAllDamageFromAffectedPlayer(std::string name) const;
    std::vector<std::pair<std::string, Damage>> getAllDamageTakenFromAffectedPlayer(std::string name) const;
    std::vector<std::pair<std::string, Damage>> getAllDamageDealtOnAffectedPlayer(std::string name) const;

    Heal getTotalHeals() const;
    std::vector<std::pair<std::string, Heal>> getHealsForAllAffectedPlayers() const;
    const Heal& getHealFromAffectedPlayer(std::string name) const;

    Nano getTotalNano() const;
    std::vector<std::pair<std::string, Nano>> getNanoForAllAffectedPlayers() const;
    const Nano& getNanoFromAffectedPlayer(std::string name) const;

    const std::vector<NanoProgram>& getNanoPrograms() const;
    const XP& getXp();

    std::time_t getTimeActive() const;
    std::time_t getPauseDuration() const;
    time_t getStartTime() {return startTime;}
    void stopTimer();
    void resumeTimer();

    size_t getLongestAffectedPlayerNameLength() const;
    std::vector<AffectedPlayer*>::size_type nrOfAffectedPlayers() {
        return affectedPlayers->size();
    }

private:
    FRIEND_TEST(PlayerTest, amountPerMinute);

    std::string name;
    std::time_t startTime = 0;
    std::time_t timeOfLastAction = 0;
    std::time_t stopTime = 0;
    struct Pause {
        std::time_t start;
        std::time_t stop;
    };
    std::vector<Pause> pauses;

    int amountPerMinute(int amount) const;
    void addDPM(std::vector<std::pair<std::string, Damage>>& v) const;

    void addNanoProgram(std::string name, std::string subtype);
    void addXp(LineInfo& li);
    Damage sumDamage(bool nanobots);
    Damage sumDamageType(const std::string damageType, bool nanobots);

    AffectedPlayerVector<AffectedPlayer*>* affectedPlayers = nullptr;
    MyTimeInterface* myTime = nullptr;
    std::string nameOfLastNanoProgramCasted;
	std::vector<NanoProgram> nanoPrograms;
    XP xp;
};


#endif
