#ifndef LOG_LINE_H
#define LOG_LINE_H


#include <vector>
#include <string>
#include "line_info.h"


class FormattedLine {
public:
    FormattedLine(std::string& line);

    const std::vector<std::string>& getFullLine() const {return formattedLine;}
    const std::string& getMessage() const {return formattedLine[4];}
    const std::string& getSender() const;
    const std::string& getDescription();
    const std::string& getOriginalLine() const {return originalLine;}

    bool isFormatted() {return formatted;}

private:
    bool format(std::string& line);
    std::vector<std::string>& split(std::string& line, char delim,
                                    std::vector<std::string>& formattedLine);
    void cleanup(std::vector<std::string>& formattedLine);

    bool formatted = false;
    std::string originalLine;
    std::vector<std::string> formattedLine;
};


#endif  // LOG_LINE_H
