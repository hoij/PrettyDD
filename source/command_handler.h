#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H


#include "player_vector.h"
#include "stat_writer.h"

#include <string>


class CommandHandler {
public:
    CommandHandler(StatWriter& statWriter,
                   PlayerVector<Player*>& playerVector) :
        statWriter(statWriter), playerVector(playerVector) {}
    void execute(std::string command);

private:
    std::vector<std::string> splitCommand(std::string command);
    std::vector<std::string> mergeQuotedText(
        std::vector<std::string> commandParts);

    //void testMerge();

    StatWriter& statWriter;
    PlayerVector<Player*> playerVector;
};


#endif  // COMMAND_HANDLER_H
