#ifndef AFFECTED_PLAYER_INTERFACE_H
#define AFFECTED_PLAYER_INTERFACE_H


#include <map>
#include <string>
#include <vector>


class Damage;
class Heal;
class Nano;
class LineInfo;

class AffectedPlayerInterface {
public:
    virtual ~AffectedPlayerInterface() {};

    virtual void add(LineInfo& lineInfo) = 0;

    virtual Damage getTotalDamageReceivedFromPlayer() const = 0;
    virtual Damage getTotalDamageDealtOnPlayer() const = 0;

    virtual std::vector<std::pair<std::string, Damage>>
    getDamageReceivedFromPlayer() const = 0;

    virtual std::vector<std::pair<std::string, Damage>>
    getDamageDealtOnPlayer() const = 0;

    virtual const Heal& getHeal() const = 0;
    virtual const Nano& getNano() const = 0;

    virtual std::string getName() const = 0;
};


#endif  // AFFECTED_PLAYER_INTERFACE_H
