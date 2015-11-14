#ifndef READ_CONSOLE_H
#define READ_CONSOLE_H


#include "my_time.h"

#include <atomic>
#include <istream>
#include <string>

class CommandHandler;
class LineInfo;

void readConsole(std::istream& input,
                 CommandHandler commandHandler,
                 MyTime myTime,
                 std::atomic<bool>& isRunning);

void prepend(std::string& command);


#endif  // CONSOLE_READER_H
