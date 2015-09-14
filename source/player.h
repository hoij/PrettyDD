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

#include <map>
#include <string>
#include <vector>


class LineInfo;


class Player : public virtual PlayerInterface {
public:
    Player(std::string name);
    Player(std::string name, MyTime* myTime);
    Player(std::string name,
           AffectedPlayerVector<AffectedPlayer*>* pv,
           MyTime* myTime);
    ~Player();
    Player(const Player& other);
    Player(Player&& other) noexcept;
    Player& operator=(Player rhs);
    friend void swap(Player& first, Player& second);

    std::string getName() const {return name;}

    void add(LineInfo& lineInfo);

    Damage getTotalDamage() const;
    Damage getTotalDamagePerDamageType(std::string damageType) const;

    std::vector<std::pair<std::string, Damage>> getTotalDamageForEveryDamageType() const;
    std::vector<std::pair<std::string, Damage>> getTotalDamageForAllAffectedPlayers() const;
    std::vector<std::pair<std::string, Damage>> getAllDamageFromAffectedPlayer(std::string name) const;

    Heal getTotalHeals() const;
    std::vector<std::pair<std::string, Heal>> getHealsForAllAffectedPlayers() const;
    const Heal& getHealFromAffectedPlayer(std::string name) const;

    Nano getTotalNano() const;
    std::vector<std::pair<std::string, Nano>> getNanoForAllAffectedPlayers() const;
    const Nano& getNanoFromAffectedPlayer(std::string name) const;

    const std::vector<NanoProgram>& getNanoPrograms() const;
    const XP& getXp();

    std::time_t getTimeActive() const;
    time_t getStartTime() {return startTime;}
    void stopTimer();
    void resumeTimer();
    std::time_t getPauseDuration() {return pauseDuration;}

    size_t getLongestAffectedPlayerNameLength() const;
    typename std::vector<AffectedPlayer*>::size_type nrOfAffectedPlayers() {
        return affectedPlayers->size();
    }

private:
    std::string name;
    std::time_t startTime = 0;
    std::time_t stopTime = 0;
    std::time_t pauseDuration = 0;

    int amountPerMinute(int amount) const;
    void addDPM(std::vector<std::pair<std::string, Damage>>& v) const;


    void addNanoProgram(std::string name, std::string subtype);
    void addXp(LineInfo& li);
    Damage sumDamage(bool nanobots);
    Damage sumDamageType(const std::string damageType, bool nanobots);

    AffectedPlayerVector<AffectedPlayer*>* affectedPlayers = nullptr;
    MyTime* myTime = nullptr;
    std::string nameOfLastNanoProgramCasted;
	std::vector<NanoProgram> nanoPrograms;
    XP xp;
};


#endif
