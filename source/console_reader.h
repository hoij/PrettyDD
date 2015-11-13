#ifndef CONSOLE_READER_H
#define CONSOLE_READER_H


#include "my_time.h"

#include <istream>
#include <string>

class LineInfo;

class ConsoleReader {
public:
    ConsoleReader(MyTimeInterface& myTime) :
        myTime(myTime) {}
    
    LineInfo read(std::istream& input);
 
private:
    MyTimeInterface& myTime;
};


#endif  // CONSOLE_READER_H