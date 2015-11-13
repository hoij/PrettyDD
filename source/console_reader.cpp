#include "console_reader.h"
#include "line_info.h"

#include <iostream>


LineInfo ConsoleReader::read(std::istream& input) {
    LineInfo li;
    std::cout << ">>> ";
    std::getline(input, li.command);
    
    // Insert pdd if the command does not contain it.
    // The command handler needs a "pdd" first to interpret
    // the commands following it.
    if (!li.command.empty() &&
        li.command.compare(0, 3, "pdd") != 0) {
        li.command.insert(0, "pdd ");
    }
    
    li.time = myTime.currentTime();
    
    return li;
}