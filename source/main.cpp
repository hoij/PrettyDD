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
    errorLog.write("Program started at: ", false);
    errorLog.write(myTime.currentTimeString());

    Configuration config;
    if(!config.read()) {
        return 1;
    }
    Parser parser(config.getplayerRunningProgram());
    PlayerVector<Player*> playerVector;
    StatWriter statWriter(playerVector);
    CommandHandler commandHandler(statWriter, playerVector);

    std::ifstream logstream("../../test/example_lines.txt");

    // TODO: Remove when done:
    playerVector.startLogging();

    if (logstream.is_open()) {
        // Go to the end of the file
        //logstream.seekg(0, logstream.end);
        //logstream.close();

        // Copying a log file with all examples for testing purposes
        //ifstream inlogstream("test_damage.txt");
        //ofstream outlogstream("log.txt");
        //while (getline(inlogstream, line)) {
        //	outlogstream << line << endl;
        //}
        //outlogstream.close();
        //inlogstream.close();

        //ifstream logstream("log.txt");

        std::string logLine;
        bool is_running = true;
        while (is_running) {
            while (getline(logstream, logLine)) {
                std::cout << logLine << std::endl;
                FormattedLine formattedLine(logLine);
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
            }
            if (!logstream.eof()) {  // Why did I check for this?
                std::cout << "Error: Not EOF!" << std::endl;
                is_running = false;
            }
            logstream.clear();
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
    }
    else {
        std::cerr << "Could not open the log file." << std::endl;
    }

    errorLog.write("");
    errorLog.write("Program ended at: ", false);
    errorLog.write(myTime.currentTimeString());

    std::getchar();
    return 0;
}
