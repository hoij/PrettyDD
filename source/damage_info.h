#ifndef DAMAGE_INFO_H
#define DAMAGE_INFO_H


#include <string>
#include "dine_info.h"


class DamageInfo : public LineInfo {
public:
    DamageInfo(std::string dealer_name,
               std::string receiver_name,
               std::string type,
               std::string subtype,
               int amount = 0,
               bool crit = false,
               bool deflect = false,
               bool miss = false,
               bool nanobots = false,
               bool hasStats = true) :
        dealer_name(dealer_name),
        receiver_name(receiver_name),
        type(type),
        subtype(subtype),
        command(command),
        amount(amount),
        crit(crit),
        deflect(deflect),
        miss(miss),
        nanobots(nanobots),
        hasStats(hasStats) {}

    DamageInfo(const DamageIndo& di) {
        // TODO: Make a swap function
        dealer_name = di.dealer_name;
        receiver_name = di.receiver_name;
        type = di.type;
        subtype = di.subtype;
        amount = di.amount;
        crit = di.crit;
        deflect = di.deflect;
        miss = di.miss;
        nanobots = di.nanobots;
        hasStats = di.hasStats;
        *nanoProgram = *(di.nanoProgram);
    }

    DamageInfo& DamageInfo::operator=(DamageInfo di) {
    if (this != &di) {
        dealer_name = di.dealer_name;
        receiver_name = di.receiver_name;
        type = di.type;
        subtype = di.subtype;
        amount = di.amount;
        crit = di.crit;
        deflect = di.deflect;
        miss = di.miss;
        nanobots = di.nanobots;
        hasStats = di.hasStats;
        *nanoProgram = *(di.nanoProgram);
    }
    return *this;
}

    std::string dealer_name;
    std::string receiver_name;
    std::string type;
    std::string subtype;
    std::string command;
    int amount = 0;
    bool crit = false;
    bool deflect = false;
    bool miss = false;
    bool nanobots = false;
    bool hasStats  // Probably not necessary
    NanoProgram* nanoProgram; // TODO: Make nanoProgram it's own class
}
