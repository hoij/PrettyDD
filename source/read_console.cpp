#ifdef WIN32
// warning C4127: conditional expression is constant
#pragma warning(disable : 4127)
#endif

#include "command_handler.h"
#include "configuration.h"
#include "read_console.h"
#include "line_info.h"

#include <chrono>
#include <iostream>
#include <limits>
#include <thread>


void readConsole(std::istream& input,
                 Configuration config,
                 MyTime myTime,
                 LineInfo& lineInfo,
                 std::mutex& lineInfoMutex,
                 std::atomic<bool>& isInitialParsingDone) {

    // Do not accept any commands while performing the
    // initial parsing.
    while (!isInitialParsingDone) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    // Print a done message if there was any initial parsing.
    if (!config.shouldParseFromEnd()) {
        std::cout << "Done." << std::endl;
    }

    std::string command;
    while (true) {
        std::cout << ">>> ";
        std::getline(input, command);
        std::lock_guard<std::mutex> lineInfoLock(lineInfoMutex);
        lineInfo.command = command;
        // Add "pdd" to the command if needed.
        prepend(lineInfo.command);
        // Set the time of the command
        lineInfo.time = myTime.currentTime();
    }
}

void prepend(std::string& command) {
    /* Insert pdd if the command does not contain it.
    The command handler needs a "pdd" first to interpret
    the commands following it. */

    if (!command.empty() &&
        command.compare(0, 3, "pdd") != 0) {
        command.insert(0, "pdd ");
    }
}
