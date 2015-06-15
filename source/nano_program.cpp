#include "nano_program.h"

void NanoProgram::createProgram(const LineInfo& li) {
    name = li.nanoProgram->getName();

}

NanoProgram& NanoProgram::operator=(const NanoProgram& np) {
    name = np.getName();
    stats = np.getStats();
    return *this;
}

NanoProgram& NanoProgram::operator+=(NanoProgram& np) {
    name = np.getName();
    for (std::map<std::string, int>::iterator it = np.stats.begin(); it != np.stats.end(); it++) {
        stats[it->first] += it->second;
    }
    return *this;
}

void NanoProgram::addStat(std::string stat, int value) {
    stats[stat] += value;
}

const std::string& NanoProgram::getName() const {return name;}
void NanoProgram::setName(std::string name) {this->name = name;}
const std::map<std::string, int>& NanoProgram::getStats() const {return stats;}
