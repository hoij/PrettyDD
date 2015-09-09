#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H


#include "stat_writer.h"

#include <string>


class CommandHandler {
public:
    CommandHandler(StatWriter& statWriter) : statWriter(statWriter) {}
    void execute(std::string command);


private:
    std::vector<std::string> splitCommand(std::string command);

    StatWriter& statWriter;
};


#endif  // COMMAND_HANDLER_H
