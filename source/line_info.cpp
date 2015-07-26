#include <ostream>
#include "line_info.h"


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
