#ifndef NANO_PROGRAM_H
#define NANO_PROGRAM_H


#include <string>
#include <map>

class NanoProgram {
public:
    NanoProgram() {}
    NanoProgram(std::string name);
    NanoProgram(std::string name, std::string action);
    //~NanoProgram();

    NanoProgram& operator=(const NanoProgram& np);

    NanoProgram& operator+=(NanoProgram& np);

    const std::string& getName() const;
    void setName(std::string name);
    const std::map<std::string, int>& getActions() const;
    int getActionCount(std::string action);
    void addAction(std::string action);

private:
    std::string name;
    std::map<std::string, int> actions;
    // In stats: execute == number of times executed;
};


#endif  // NANO_PROGRAM_H
