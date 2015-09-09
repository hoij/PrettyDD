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

    virtual Damage getTotalDamage() const = 0;
    virtual Damage getDamagePerDamageType(const std::string damageType) const = 0;
    virtual std::vector<std::pair<std::string, Damage>> getAllDamage() const = 0;

    virtual const Heal& getHeal() const = 0;
    virtual const Nano& getNano() const = 0;

    virtual std::string getName() const = 0;
};


#endif  // AFFECTED_PLAYER_INTERFACE_H
