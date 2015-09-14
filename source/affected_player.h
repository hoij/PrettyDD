#ifndef AFFECTED_PLAYER_H
#define AFFECTED_PLAYER_H


#include "affected_player_interface.h"
#include "damage.h"
#include "heal.h"
#include "nano.h"

#include <ctime>
#include <map>
#include <string>
#include <vector>


class LineInfo;
class MyTime;

class AffectedPlayer : public virtual AffectedPlayerInterface {
public:
	AffectedPlayer(std::string name, MyTime* myTime);

    void add(LineInfo& lineInfo);

    std::string getName() const {return name;}

    Damage getTotalDamage() const;
    Damage getDamagePerDamageType(const std::string damageType) const;
    std::vector<std::pair<std::string, Damage>> getAllDamage() const;

    const Heal& getHeal() const;
    const Nano& getNano() const;

    time_t getStartTime() {return startTime;}
    void incrementPauseDuration(const std::time_t& duration);
    std::time_t getPauseDuration() {return pauseDuration;}

private:
    std::string name;
    std::time_t startTime = 0;
    std::time_t pauseDuration = 0;

    void addDamage(LineInfo& li);
    void addHeal(LineInfo& li);
    void addNano(LineInfo& li);

    std::map<std::string, Damage> damage;
    Heal heal;
    Nano nano;

    MyTime* myTime = nullptr;
};


#endif  // AFFECTED_PLAYER_H
