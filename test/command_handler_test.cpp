#include "command_handler.h"
#include "configuration.h"
#include "damage_writer.h"
#include "my_time.h"
#include "nano_program_writer.h"
#include "player_vector.h"
#include "stat_writer.h"
#include "xp_writer.h"

#include <gtest/gtest.h>
#include <ostream>


TEST(CommandHandlerTest, splitAndMergeQuotedText) {
    std::ofstream file;
    PlayerVector<Player*> playerVector("playerRunningProgram");
    Configuration config;
    NanoProgramWriter nanoProgramWriter(playerVector, config, file);
    DamageWriter damageWriter(playerVector, config, file);
    XPWriter xpWriter(playerVector, config, file);
    StatWriter statWriter(playerVector,
                          config,
                          nanoProgramWriter,
                          damageWriter,
                          xpWriter,
                          file);
    MyTime myTime;
    CommandHandler commandHandler(statWriter, playerVector, myTime);

    std::string text1 = "pdd \"Player1\" \"Player 2\" s";
    std::vector<std::string> expected1 = {"pdd", "Player1", "Player 2", "s"};
    std::vector<std::string> commandParts = commandHandler.splitCommand(text1);
    std::vector<std::string> result1 = commandHandler.mergeQuotedText(commandParts);
    EXPECT_EQ(expected1, result1);

    std::string text2 = "pdd \"Player1\" \" Player 2\" s";
    std::vector<std::string> expected2 = {"pdd", "Player1", "\" Player 2", "s"};
    std::vector<std::string> commandParts2 = commandHandler.splitCommand(text2);
    std::vector<std::string> result2 = commandHandler.mergeQuotedText(commandParts2);
    EXPECT_EQ(expected2, result2);


    std::string text3 = "hello \"quote1\" \"quote2\"";
    std::vector<std::string> expected3 = {"hello", "quote1", "quote2"};
    std::vector<std::string> commandParts3 = commandHandler.splitCommand(text3);
    std::vector<std::string> result3 = commandHandler.mergeQuotedText(commandParts3);
    EXPECT_EQ(expected3, result3);

    std::string text4 = "hello \"this is some quoted text\" and this is not";
    std::vector<std::string> expected4 = {"hello", "this is some quoted text",
                                          "and", "this", "is", "not"};
    std::vector<std::string> commandParts4 = commandHandler.splitCommand(text4);
    std::vector<std::string> result4 = commandHandler.mergeQuotedText(commandParts4);
    EXPECT_EQ(expected4, result4);

    std::string text5 = "hello \"this is some quotedtext \" and this is not";
    std::vector<std::string> expected5 = {"hello", "this is some quotedtext \"",
                                          "and", "this", "is", "not"};
    std::vector<std::string> commandParts5 = commandHandler.splitCommand(text5);
    std::vector<std::string> result5 = commandHandler.mergeQuotedText(commandParts5);
    EXPECT_EQ(expected5, result5);
}