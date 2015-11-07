#ifndef PLAYER_H
#define PLAYER_H


#include "affected_player_vector.h"
#include "player_interface.h"
#include "my_time_interface.h"
#include "nano_programs.h"
#include "xp.h"

#include <ctime>
#include <gtest/gtest_prod.h>
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

class Player : public virtual PlayerInterface {
// TODO: Se if Player can be split into smaller classes.
public:
    Player(std::string name);
    Player(std::string name, std::shared_ptr<MyTimeInterface> myTime);
    Player(std::string name,
           AffectedPlayerVector<AffectedPlayer*>* pv,
           MyTimeInterface* myTime);
    ~Player();
    Player(const Player& other);
    Player(Player&& other) NOEXCEPT;
    Player& operator=(Player rhs);
    friend void swap(Player& first, Player& second);

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

    /* Time. TODO: Move into its own class */
    std::time_t getTimeActive() const;
    std::time_t getPauseDuration() const;
    std::time_t getStartTime() const {return startTime;}
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
    std::shared_ptr<MyTimeInterface> myTime = nullptr;
	NanoPrograms nanoPrograms;
    XP xp;
};


#endif
