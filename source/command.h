#ifndef COMMAND_H
#define COMMAND_H


#include <string>
#include "line_info.h"


class Command : public LineInfo {
public:
    Command(std::string command) : command(command) {
        hasCommand = true;
    }

    Command(const LineInfo& li) {
        // TODO: Make a swap function
        command = li.command;
        hasCommand = li.hasCommand;
    }

    Command& operator=(LineInfo li) {
        if (this != &li) {
            command = li.command;
            hasCommand = li.hasCommand;
        }
        return *this;
    }

    bool hasCommand;  // Probably not necessary
    std::string command;
}


#endif // COMMAND_H
