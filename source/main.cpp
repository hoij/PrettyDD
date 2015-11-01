#include "command_handler.h"
#include "configuration.h"
#include "damage_writer.h"
#include "help_writer.h"
#include "formatted_line.h"
#include "logger.h"
#include "my_time.h"
#include "nano_program_writer.h"
#include "parser.h"
#include "player_interface.h"
#include "player_vector.h"
#include "xp_writer.h"

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>


int main(void) {
    MyTime myTime;
    errorLog.write("");
    errorLog.write("Info: Program started at: " + myTime.currentTimeString());

    // Read config
    Configuration config;
    if(!config.read()) {
        return 1;
    }

    // Instantiate classes
    Parser parser(config.getPlayerRunningProgram());
    PlayerVector<Player*> playerVector(config.getPlayerRunningProgram());
    playerVector.startLogging();

    std::ofstream file;
    DamageWriter damageWriter(playerVector, config, file);
    HelpWriter helpWriter(config, file);
    NanoProgramWriter nanoProgramWriter(playerVector, config, file);
    WriterHelper writerHelper(config, file);
    XPWriter xpWriter(playerVector, config, file);
    CommandHandler commandHandler(playerVector,
                                  file,
                                  damageWriter,
                                  helpWriter,
                                  myTime,
                                  nanoProgramWriter,
                                  writerHelper,
                                  xpWriter);

    // Open the AO log
    // TODO: Move into parser or new class.
    std::ifstream logstream(config.getLogFilePath());
    if (!logstream.is_open()) {
        errorLog.write("Error: Could not open the log file:");
        errorLog.write("Error: " + config.getLogFilePath());
    }
    logstream.clear();

    // Create the help script on startup
    helpWriter.createHelp("pddhelp");

    // Read from the end of the file if true,
    // otherwise read from the start.
    if (config.shouldParseFromEnd()) {
        logstream.seekg(0, logstream.end);
    }
    std::ios::streampos lastpos = logstream.tellg();
    std::ios::streampos endpos = logstream.tellg();

    std::string line;
    bool isRunning = true;
    // Parse loop
    while (isRunning) {
        // If there are no more lines to read
        if(!std::getline(logstream, line) || logstream.eof()) {

            /* Handle the case when the log file shrinks (for example
            when it's deleted). */
            logstream.clear();
            // Seek to the end and save that position
            logstream.seekg(0, logstream.end);
            endpos = logstream.tellg();
            logstream.clear();
            // Compare the end with the position of the last char read
            // to see if the file has shrunk. If it has, then move back
            // to the new end.
            logstream.seekg((endpos < lastpos) ? endpos : lastpos);

            // Sleep to get less CPU intensive
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            continue;
        }

        // Format and parse the log line
        FormattedLine formattedLine(line);
        if (formattedLine.isFormatted()) {  // If successfully formatted
            LineInfo lineInfo = parser.parse(formattedLine);  // Parse it
            if(lineInfo.hasStats) {
                playerVector.addToPlayers(lineInfo);
            }
            else if (!lineInfo.command.empty()) { // Command exists
                isRunning = commandHandler.execute(lineInfo);
            }
        }

        // Save position of the last char read.
        lastpos = logstream.tellg();
    }

    errorLog.write("");
    errorLog.write("Info: Program ended at: " + myTime.currentTimeString());

    std::getchar();
    return 0;
}
