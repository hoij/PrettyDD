#include "command_handler.h"
#include "read_console.h"
#include "line_info.h"

#include <iostream>


void readConsole(std::istream& input,
                 CommandHandler commandHandler,
                 MyTime myTime,
                 std::atomic<bool>& isRunning) {

    LineInfo li;
    while (isRunning) {

        std::cout << ">>> ";
        std::getline(input, li.command);

        prepend(li.command);

        // Set the time of the command
        li.time = myTime.currentTime();

        isRunning = commandHandler.execute(li);
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
