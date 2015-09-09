#ifndef AFFECTED_PLAYER_H
#define AFFECTED_PLAYER_H


#include "affected_player_interface.h"
#include "damage.h"
#include "heal.h"
#include "nano.h"

#include <map>
#include <string>
#include <vector>


class LineInfo;

class AffectedPlayer : public virtual AffectedPlayerInterface {
public:
	AffectedPlayer(std::string name);

    void add(LineInfo& lineInfo);

    std::string getName() const {return name;}

    Damage getTotalDamage() const;
    Damage getDamagePerDamageType(const std::string damageType) const;
    std::vector<std::pair<std::string, Damage>> getAllDamage() const;

    const Heal& getHeal() const;
    const Nano& getNano() const;

private:
    std::string name;

    void addDamage(LineInfo& li);
    void addHeal(LineInfo& li);
    void addNano(LineInfo& li);

    std::map<std::string, Damage> damage;
    Heal heal;
    Nano nano;
};


#endif  // AFFECTED_PLAYER_H
