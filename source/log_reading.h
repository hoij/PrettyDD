#ifndef LOG_READING_H
#define LOG_READING_H


#include <fstream>


void adaptIfLogShrunk(std::ifstream& logstream,
                      std::streampos& lastpos,
                      std::streampos& endpos) {
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
}


#endif  // LOG_READING_H