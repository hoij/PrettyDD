#include <ostream>
#include "line_info.h"

LineInfo::LineInfo(std::string dealer_name,
                   std::string receiver_name,
                   std::string type,
                   std::string subtype,
                   std::string command = "",
                   int amount = 0,
                   bool crit = false,
                   bool deflect = false,
                   bool miss = false,
                   bool nanobots = false,
                   bool hasStats = true,
                   std::string nanoProgramName = "") :
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
    hasStats(hasStats),
    nanoProgramName(nanoProgramName) {
}

LineInfo::LineInfo(const LineInfo& li) {
    // TODO: Make a swap function
    dealer_name = li.dealer_name;
    receiver_name = li.receiver_name;
    type = li.type;
    subtype = li.subtype;
    command = li.command;
    amount = li.amount;
    crit = li.crit;
    deflect = li.deflect;
    miss = li.miss;
    nanobots = li.nanobots;
    hasStats = li.hasStats;
    nanoProgramName = li.nanoProgramName;
}

LineInfo& LineInfo::operator=(LineInfo li) {
    if (this != &li) {
        dealer_name = li.dealer_name;
        receiver_name = li.receiver_name;
        type = li.type;
        subtype = li.subtype;
        command = li.command;
        amount = li.amount;
        crit = li.crit;
        deflect = li.deflect;
        miss = li.miss;
        nanobots = li.nanobots;
        hasStats = li.hasStats;
        nanoProgramName = li.nanoProgramName;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const LineInfo& li) {
        os << "dealer = " << li.dealer_name <<
        ", receiver = " << li.receiver_name <<
        ", type = " << li.type <<
        ", subtype = " << li.subtype <<
        ", command = " << li.command <<
        ", amount = " << li.amount <<
        ", crit = " << li.crit <<
        ", deflect = " << li.deflect <<
        ", miss = " << li.miss <<
        ", nanobots = " << li.nanobots <<
        ", hasStats = " << li.hasStats <<
        ", nanoProgramName = " << li.nanoProgramName <<
        std::endl;
        return os;
}
