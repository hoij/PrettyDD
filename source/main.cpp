#include "command_handler.h"
#include "configuration.h"
#include "damage_writer.h"
#include "formatted_line.h"
#include "help_writer.h"
#include "line_info.h"
#include "log_reading.h"
#include "logger.h"
#include "my_time.h"
#include "nano_program_writer.h"
#include "parser.h"
#include "player_factory.h"
#include "player_factory_interface.h"
#include "player_interface.h"
#include "player_vector.h"
#include "read_console.h"
#include "xp_writer.h"

#include <atomic>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <mutex>
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

    // Open global errorLog
    errorLog.open();

    // Instantiate classes
    Parser parser(config.getPlayerRunningProgram());
    PlayerVector playerVector(
        config.getPlayerRunningProgram(),
        std::unique_ptr<PlayerFactoryInterface>(new PlayerFactory()));
    playerVector.startLogging();

    std::ofstream file;
    DamageWriter damageWriter(playerVector, config, file);
    HelpWriter helpWriter(config, file);
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

    // Open the AO log
    // TODO: Move into parser or new class.
    std::ifstream logstream(config.getLogFilePath());
    if (!logstream.is_open()) {
        errorLog.write("Error: Could not open the log file:");
        errorLog.write("Error: " + config.getLogFilePath());
    }
    logstream.clear();
	std::cout << "Opened: " << config.getLogFilePath() << '\n';

    // Create the help script on startup
    helpWriter.createHelp("pddhelp");

    // Create a thread to handle console input.
    // This thread is the only one reading from cin.
    std::atomic<bool> isInitialParsingDone(false);
    LineInfo lineInfo;  // Shared between threads.
    std::mutex lineInfoMutex;
    std::thread cinThread(readConsole,
        std::ref(std::cin),
        config,
        myTime,
        std::ref(lineInfo),
        std::ref(lineInfoMutex),
        std::ref(isInitialParsingDone));
    cinThread.detach();

    // Read from the end of the file if true,
    // otherwise read from the start.

    if (config.shouldParseFromEnd()) {
		std::cout << "Parsing the log from the end.\n";
        logstream.seekg(0, logstream.end);
        isInitialParsingDone = true;
    }
    else {
		std::cout << "Parsing the log from the start.\n";
    }

    // Parse loop
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::streampos lastpos = logstream.tellg();
    std::streampos endpos = logstream.tellg();
    bool isRunning = true;
    std::string line;
    while (isRunning) {
        // If there are no more lines to read
        if (!std::getline(logstream, line) || logstream.eof()) {
            if (!isInitialParsingDone) {
                std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                std::cout << "Parse time: " <<
                    std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() <<
                    " ms." << std::endl;
            }
            // Set to true to signal to the console thread that parsing
            // is done and that it can start reading commands from the
            // console.
            isInitialParsingDone = true;

            adaptIfLogShrunk(logstream, lastpos, endpos);

            // Check for command
            {  // New scope to let lock expire on exit
                std::lock_guard<std::mutex> lineInfoLock(lineInfoMutex);
                isRunning = commandHandler.process(lineInfo);
            }

            // Sleep to get less CPU intensive
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        // Format and parse the log line
        FormattedLine formattedLine;
        formattedLine.format(line);
        {  // New scope to let lock expire on exit
            std::lock_guard<std::mutex> lineInfoLock(lineInfoMutex);
            if (formattedLine.isFormatted()) {  // If successfully formatted
                lineInfo = parser.parse(formattedLine);
                if (lineInfo.hasStats) {
                    playerVector.addToPlayers(lineInfo);
                }
            }
            // Check for command
            isRunning = commandHandler.process(lineInfo);
        }

        // Save position of the last char read.
        lastpos = logstream.tellg();
    }

    errorLog.write("");
    errorLog.write("Info: Program ended at: " + myTime.currentTimeString());

    return 0;
}
