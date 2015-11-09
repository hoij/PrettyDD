#ifndef AFFECTED_PLAYER_H
#define AFFECTED_PLAYER_H


#include "affected_player_interface.h"
#include "damage.h"
#include "heal.h"
#include "nano.h"

#include <map>
#include <memory>
#include <string>
#include <vector>


class LineInfo;
class MyTimeInterface;

class AffectedPlayer : public virtual AffectedPlayerInterface {
public:
    AffectedPlayer(std::string name, std::shared_ptr<MyTimeInterface> myTime);

    void add(LineInfo& lineInfo);

    std::string getName() const {return name;}

    Damage getTotalDamageReceivedFromPlayer() const;
    Damage getTotalDamageDealtOnPlayer() const;

    std::vector<std::pair<std::string, Damage>>
    getDamageReceivedFromPlayer() const;

    std::vector<std::pair<std::string, Damage>>
    getDamageDealtOnPlayer() const;

    const Heal& getHeal() const;
    const Nano& getNano() const;

private:
    std::string name;

    void addDamage(LineInfo& li);
    void addHeal(LineInfo& li);
    void addNano(LineInfo& li);

    // Map contains ame of the type and it's Damage.
    std::map<std::string, Damage> damageReceivedFromPlayer;
    std::map<std::string, Damage> damageDealtOnPlayer;

    Heal heal;
    Nano nano;

    std::shared_ptr<MyTimeInterface> myTime = nullptr;
};


#endif  // AFFECTED_PLAYER_H
