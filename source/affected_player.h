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
    const Damage& getDamage() const;
    const Heal& getHeal() const;
    const Nano& getNano() const;

private:
    std::string name;

    void addDamage(LineInfo& li);
    void addHeal(LineInfo& li);
    void addNano(LineInfo& li);

    Damage damage;
    Heal heal;
    Nano nano;

    MyTime* myTime = nullptr;
};


#endif  // AFFECTED_PLAYER_H
