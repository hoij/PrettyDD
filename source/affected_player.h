#ifndef AFFECTED_PLAYER_H
#define AFFECTED_PLAYER_H


#include "base_player.h"
#include "damage.h"
#include "heal.h"
#include "nano.h"

#include <map>
#include <string>
#include <vector>

class Damage;
class Heal;
class Nano;


class LineInfo;

class AffectedPlayer : public BasePlayer {
public:
	AffectedPlayer(std::string name);

    void add(LineInfo& lineInfo);

    Damage getTotalDamage() const;
    Damage getTotalDamage(bool nanobots) const;
    Damage getTotalDamagePerDamageType(const std::string damageType) const;
    Damage getTotalDamagePerDamageType(const std::string damageType, bool nanobots) const;

    const std::map<std::string, Damage>& getRegularDamage() const;
    const std::map<std::string, Damage>& getNanobotsDamage() const;
    const Heal& getHeal() const;
    const Nano& getNano() const;

private:
    void addDamage(LineInfo& li);
    void addHeal(LineInfo& li);
    void addNano(LineInfo& li);

    std::map<std::string, Damage> regularDamage;
    std::map<std::string, Damage> nanobotsDamage;
    Heal heal;
    Nano nano;
};


#endif  // AFFECTED_PLAYER_H
