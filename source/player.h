#ifndef PLAYER_H
#define PLAYER_H


#include "affected_player_vector.h"
#include "damage.h"
#include "heal.h"
#include "nano.h"
#include "nano_programs.h"
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

    void addXp(LineInfo& li);
    Damage sumDamage(bool nanobots);
    Damage sumDamageType(const std::string damageType, bool nanobots);

    AffectedPlayerVector<AffectedPlayer*>* affectedPlayers = nullptr;
    MyTimeInterface* myTime = nullptr;
	NanoPrograms nanoPrograms;
    XP xp;
};


#endif
