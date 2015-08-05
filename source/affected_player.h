#ifndef AFFECTED_PLAYER_H
#define AFFECTED_PLAYER_H


#include <string>
#include <vector>
#include <map>
#include "base_player.h"
#include "line_info.h"
#include "heal.h"
#include "damage.h"
#include "nano.h"


class AffectedPlayer : public BasePlayer {
public:
	AffectedPlayer();
	AffectedPlayer(std::string name, LineInfo& lineInfo);
	~AffectedPlayer();
	AffectedPlayer(const AffectedPlayer& p);
	AffectedPlayer& operator=(const AffectedPlayer& p);

    void add(LineInfo& lineInfo);

    Damage getTotalDamage() const;
    Damage getTotalDamage(bool nanobots) const;
    Damage getTotalDamagePerDamageType(const std::string damageType);
    Damage getTotalDamagePerDamageType(const std::string damageType, bool nanobots);

    const Heal& getHeal() const;
    const Nano& getNano() const;

    static bool compareTotalDealt(AffectedPlayer& ap1, AffectedPlayer& ap2);

private:
    void addDamage(LineInfo& li);
    void addHeal(LineInfo& li);

    std::map<std::string, Damage> regularDamage;
    std::map<std::string, Damage> nanobotsDamage;
    Heal heal;
    Nano nano;
};


#endif  // AFFECTED_PLAYER_H
