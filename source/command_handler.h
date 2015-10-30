#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H


#include "damage_writer.h"
#include "gtest/gtest_prod.h"
#include "help_writer.h"
#include "my_time_interface.h"
#include "nano_program_writer.h"
#include "player.h"
#include "player_vector.h"
#include "writer_helper.h"
#include "xp_writer.h"

#include <string>

class LineInfo;

class CommandHandler {
public:
    CommandHandler(PlayerVector<Player*>& playerVector,
                   std::ofstream& file,
                   DamageWriter& damageWriter,
                   HelpWriter& helpWriter,
                   MyTimeInterface& myTime,
                   NanoProgramWriter& nanoProgramWriter,
                   WriterHelper& writerHelper,
                   XPWriter& xpWriter);

    CommandHandler& operator=(CommandHandler rhs) = delete;
    bool execute(const LineInfo& li);

private:
    FRIEND_TEST(CommandHandlerTest, splitAndMergeQuotedText);
    std::vector<std::string> splitCommand(std::string command);
    std::vector<std::string> mergeQuotedText(
        std::vector<std::string> commandParts);

    bool wasCommandTypedLongAgo(const std::time_t& t);

    DamageWriter& damageWriter;
    HelpWriter& helpWriter;
    MyTimeInterface& myTime;
    NanoProgramWriter& nanoProgramWriter;
    PlayerVector<Player*>& playerVector;
    WriterHelper& writerHelper;
    XPWriter& xpWriter;
};


#endif  // COMMAND_HANDLER_H
