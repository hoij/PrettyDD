#ifndef PLAYER_H
#define PLAYER_H

#include "affected_player_vector.h"
#include "base_player.h"
#include "damage.h"
#include "heal.h"
#include "nano.h"
#include "nano_program.h"
#include "xp.h"

#include <map>
#include <string>
#include <vector>


class LineInfo;


class Player : public BasePlayer {
public:
    Player(std::string name);
    Player(std::string name, AffectedPlayerVector<AffectedPlayer*>* pv);
    virtual ~Player();
    Player(const Player& other);
    Player(Player&& other);
    Player& operator=(Player rhs);
    friend void swap(Player& first, Player& second);

    virtual void add(LineInfo& lineInfo);

    virtual Damage getTotalDamage() const;
    virtual Damage getTotalRegularDamage() const;
    virtual Damage getTotalNanobotsDamage() const;
    virtual Damage getTotalDamagePerDamageType(std::string damageType) const;
    virtual Damage getTotalRegularDamagePerDamageType(std::string damageType) const;
    virtual Damage getTotalNanobotsDamagePerDamageType(std::string damageType) const;
    std::vector<std::pair<std::string, Damage>> getTotalDamageForEachAffectedPlayer() const;
    const std::map<std::string, Damage>& getNanobotsDamagePerAffectedPlayer(std::string name) const;
    const std::map<std::string, Damage>& getRegularDamagePerAffectedPlayer(std::string name) const;

    Heal getTotalHeals() const;
    std::vector<std::pair<std::string, Heal>> getHealsForEachAffectedPlayer() const;
    const Heal& getHealsPerAffectedPlayer(std::string name) const;

    Nano getTotalNano() const;
    std::vector<std::pair<std::string, Nano>> getNanoForEachAffectedPlayer() const;
    const Nano& getNanoPerAffectedPlayer(std::string name) const;

    const std::vector<NanoProgram>& getNanoPrograms() const;
    const XP& getXp();

    size_t getLongestAffectedPlayerNameLength() const;

private:
//    static bool compareTotalDealtToPlayer(std::pair<std::string, Damage>& p1,
//                                          std::pair<std::string, Damage>& p2);
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
