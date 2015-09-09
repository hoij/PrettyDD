#ifndef PLAYER_INTERFACE_H
#define PLAYER_INTERFACE_H


#include <map>
#include <string>
#include <vector>


class Damage;
class Heal;
class Nano;
class NanoProgram;
class LineInfo;
class XP;


class PlayerInterface {
public:
    virtual ~PlayerInterface() {};

    virtual void add(LineInfo& lineInfo) = 0;

    virtual Damage getTotalDamage() const = 0;
    virtual Damage getTotalDamagePerDamageType(std::string damageType) const = 0;
    virtual std::vector<std::pair<std::string, Damage>> getTotalDamageForAllAffectedPlayers() const = 0;
    virtual std::vector<std::pair<std::string, Damage>> getAllDamageFromAffectedPlayer(std::string name) const = 0;

    virtual Heal getTotalHeals() const = 0;
    virtual std::vector<std::pair<std::string, Heal>> getHealsForAllAffectedPlayers() const = 0;
    virtual const Heal& getHealFromAffectedPlayer(std::string name) const = 0;

    virtual Nano getTotalNano() const = 0;
    virtual std::vector<std::pair<std::string, Nano>> getNanoForAllAffectedPlayers() const = 0;
    virtual const Nano& getNanoFromAffectedPlayer(std::string name) const = 0;

    virtual const std::vector<NanoProgram>& getNanoPrograms() const = 0;
    virtual const XP& getXp() = 0;

    virtual size_t getLongestAffectedPlayerNameLength() const = 0;
};


#endif  //PLAYER_INTERFACE_H

