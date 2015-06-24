#ifndef LOG_LINE_H
#define LOG_LINE_H


#include <vector>
#include <string>
#include "line_info.h"


class LogLine {
public:
    LogLine() {}
    LogLine(std::string& line);
    bool format(std::string& line);
    const std::vector<std::string>& getSplitLine() const {return splitLine;}
    const std::string& getOriginalLine() const {return originalLine;}
    LineInfo& getInfo() {return info;}

private:
    std::vector<std::string>& split(std::string& line, char delim, std::vector<std::string>& splitLine);
    void cleanup(std::vector<std::string>& splitLine);

    std::string originalLine;
    std::vector<std::string> splitLine;
    LineInfo info;
};


#endif  // LOG_LINE_H
