#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H


#include "my_time_interface.h"
#include "player_vector.h"
#include "stat_writer.h"

#include "gtest/gtest_prod.h"

#include <string>

class LineInfo;

class CommandHandler {
public:
    CommandHandler(StatWriter& statWriter,
                   PlayerVector<Player*>& playerVector,
                   MyTimeInterface& myTime) :
        statWriter(statWriter),
        playerVector(playerVector),
        myTime(myTime) {}
    CommandHandler& operator=(CommandHandler rhs) = delete;
    bool execute(const LineInfo& li);

private:
    FRIEND_TEST(CommandHandlerTest, splitAndMergeQuotedText);
    std::vector<std::string> splitCommand(std::string command);
    std::vector<std::string> mergeQuotedText(
        std::vector<std::string> commandParts);

    bool wasCommandTypedLongAgo(const std::time_t& t);

    StatWriter& statWriter;
    PlayerVector<Player*>& playerVector;
    MyTimeInterface& myTime;
};


#endif  // COMMAND_HANDLER_H
