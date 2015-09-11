#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H


#include "stat_writer.h"

#include <string>


class CommandHandler {
public:
    CommandHandler(StatWriter& statWriter) : statWriter(statWriter) {}
    void execute(std::string command);

    std::vector<std::string> mergeQuotedText(
        std::vector<std::string> commandParts);

private:
    std::vector<std::string> splitCommand(std::string command);
    std::vector<std::string> splitCommand2(std::string command);

    //temp
    void testMerge();

    StatWriter& statWriter;
};


#endif  // COMMAND_HANDLER_H
