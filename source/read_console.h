#ifndef READ_CONSOLE_H
#define READ_CONSOLE_H


#include "my_time.h"

#include <atomic>
#include <istream>
#include <mutex>
#include <string>

class Configuration;
class LineInfo;

void readConsole(std::istream& input,
                 Configuration config,
                 MyTime myTime,
                 LineInfo& lineInfo,
                 std::mutex& lineInfoMutex,
                 std::atomic<bool>& isInitialParsingDone);

void prepend(std::string& command);


#endif  // CONSOLE_READER_H
