#include "nano_program.h"

NanoProgram::NanoProgram(std::string name) {
    setName(name);
}

NanoProgram::NanoProgram(std::string name, std::string action) {
    setName(name);
    addAction(action);
}

NanoProgram& NanoProgram::operator=(const NanoProgram& np) {
    if (this != &np) {
        name = np.getName();
        actions = np.getActions();
    }
    return *this;
}

NanoProgram& NanoProgram::operator+=(NanoProgram& np) {
    name = np.getName();
    for (std::map<std::string, int>::iterator it = np.actions.begin(); it != np.actions.end(); it++) {
        actions[it->first] += it->second;
    }
    return *this;
}

void NanoProgram::addAction(std::string action) {
    actions[action]++;
}

const std::string& NanoProgram::getName() const {return name;}
void NanoProgram::setName(std::string name) {this->name = name;}
const std::map<std::string, int>& NanoProgram::getActions() const {return actions;}
int NanoProgram::getActionCount(std::string action) {
    return actions[action];
}
