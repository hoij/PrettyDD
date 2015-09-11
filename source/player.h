#ifndef PLAYER_H
#define PLAYER_H


#include "affected_player_vector.h"
#include "damage.h"
#include "heal.h"
#include "nano.h"
#include "nano_program.h"
#include "player_interface.h"
#include "xp.h"

#include <map>
#include <string>
#include <vector>


class LineInfo;


class Player : public virtual PlayerInterface {
public:
    Player(std::string name);
    Player(std::string name, AffectedPlayerVector<AffectedPlayer*>* pv);
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

    size_t getLongestAffectedPlayerNameLength() const;
    typename std::vector<AffectedPlayer*>::size_type nrOfAffectedPlayers() {
        return affectedPlayers->size();
    }

private:
//    static bool compareTotalDealtToPlayer(std::pair<std::string, Damage>& p1,
//                                          std::pair<std::string, Damage>& p2);
    std::string name;

    void addNanoProgram(std::string name, std::string subtype);
    void addXp(LineInfo& li);
    Damage sumDamage(bool nanobots);
    Damage sumDamageType(const std::string damageType, bool nanobots);

    AffectedPlayerVector<AffectedPlayer*>* affectedPlayers = nullptr;
    std::string nameOfLastNanoProgramCasted;
	std::vector<NanoProgram> nanoPrograms;
    XP xp;
};


#endif
