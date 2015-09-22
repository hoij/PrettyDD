#include "command_handler.h"
#include "configuration.h"
#include "formatted_line.h"
#include "logger.h"
#include "my_time.h"
#include "parser.h"
#include "player_interface.h"
#include "player_vector.h"
#include "stat_writer.h"

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>


int main(void) {
    MyTime myTime;
    errorLog.write("");
    errorLog.write("Info: Program started at: ", false);
    errorLog.write(myTime.currentTimeString());

    Configuration config;
    if(!config.read()) {
        return 1;
    }

    Parser parser(config.getplayerRunningProgram());
    PlayerVector<Player*> playerVector;
    StatWriter statWriter(playerVector, config);
    CommandHandler commandHandler(statWriter, playerVector);

    // TODO: Remove when done:
    playerVector.startLogging();

    // TODO: Move into parser or new class.
    std::ifstream logstream(config.getLogFilePath());
    if (!logstream.is_open()) {
        errorLog.write("Error: Could not open the log file:");
        errorLog.write("Error: " + config.getLogFilePath());
    }
    logstream.clear();
    // Read from the end of the file if true,
    // otherwise read from the start.
    if (config.shouldParseFromEnd()) {
        logstream.seekg(0, logstream.end);
    }
    std::ios::streampos lastpos = logstream.tellg();
    std::ios::streampos endpos = logstream.tellg();

    std::string line;
    bool is_running = true;
    while (is_running) {
        if(!std::getline(logstream, line) || logstream.eof()) {
            // Check the end to see if the file has shrunk.
            // If it has, then move back to the new end.
            logstream.clear();
            logstream.seekg(0, logstream.end);
            endpos = logstream.tellg();
            logstream.clear();
            logstream.seekg((endpos < lastpos) ? endpos : lastpos);
            //sleep to get less CPU intensive
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            continue;
        }

        FormattedLine formattedLine(line);
        // TODO: Figure out some way to handling this check nicer.
        if (formattedLine.isFormatted()) {
            LineInfo lineInfo = parser.parse(formattedLine);
            if(lineInfo.hasStats) {
                playerVector.addToPlayers(lineInfo);
            }
            else if (!lineInfo.command.empty()) {
                commandHandler.execute(lineInfo.command);
            }
        }

        lastpos = logstream.tellg();
        //std::cout << "The log line: " << line << std::endl;
    }

    errorLog.write("");
    errorLog.write("Info: Program ended at: ", false);
    errorLog.write(myTime.currentTimeString());

    std::getchar();
    return 0;
}
