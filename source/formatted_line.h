#ifndef FORMATTED_LINE_H
#define FORMATTED_LINE_H


#include <vector>
#include <string>
#include "formatted_line_interface.h"


class FormattedLine : public FormattedLineInterface{
public:
    FormattedLine() {};
    FormattedLine(std::string& line);

    const std::string& getMessage() const {return formattedLine[4];}
    const std::string& getSender() const;
    const std::string& getDescription() const {return formattedLine[1];}
    const std::string& getDescriptionCode() const {return formattedLine[0];}
    const std::string& getOriginalLine() const {return originalLine;}

    bool isFormatted() const {return formatted;}

private:
    bool format(std::string& line);
    std::vector<std::string>& split(std::string& line, char delim,
                                    std::vector<std::string>& formattedLine);
    void cleanup(std::vector<std::string>& formattedLine);

    bool formatted = false;
    std::string originalLine;
    std::vector<std::string> formattedLine;
};


#endif  // FORMATTED_LINE_H
