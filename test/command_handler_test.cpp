#include "command_handler.h"
#include "configuration.h"
#include "damage_writer.h"
#include "help_writer.h"
#include "my_time.h"
#include "nano_program_writer.h"
#include "player.h"
#include "player_factory.h"
#include "player_vector.h"
#include "writer_helper.h"
#include "xp_writer.h"

#include <gtest/gtest.h>
#include <ostream>


TEST(CommandHandlerTest, splitAndMergeQuotedText) {
    std::ofstream file;
    PlayerVector playerVector(
        "playerRunningProgram",
        std::make_unique<PlayerFactory>());
    Configuration config;
    DamageWriter damageWriter(playerVector, config, file);
    HelpWriter helpWriter(config, file);
    MyTime myTime;
    NanoProgramWriter nanoProgramWriter(playerVector, config, file);
    WriterHelper writerHelper(config, file);
    XPWriter xpWriter(playerVector, config, file);
    CommandHandler commandHandler(damageWriter,
                                  helpWriter,
                                  myTime,
                                  nanoProgramWriter,
				                  playerVector,
                                  writerHelper,
                                  xpWriter);

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
