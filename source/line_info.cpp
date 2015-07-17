#include <ostream>
#include "line_info.h"
#include "nano_program.h"

LineInfo::LineInfo() {
    nanoProgram = new NanoProgram;
}

LineInfo::~LineInfo() {
    delete nanoProgram;
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
    hasCommand = li.hasCommand;
    hasStats = li.hasStats;
    *nanoProgram = *(li.nanoProgram);
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
        hasCommand = li.hasCommand;
        hasStats = li.hasStats;
        *nanoProgram = *(li.nanoProgram);
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
        ", hasCommand = " << li.hasCommand <<
        ", hasStats = " << li.hasStats <<
        //*nanoProgram = *(li.nanoProgram);
        std::endl;
        return os;
}
