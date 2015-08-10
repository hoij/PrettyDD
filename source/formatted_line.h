#ifndef FORMATTED_LINE_H
#define FORMATTED_LINE_H


#include "formatted_line_interface.h"

#include <string>
#include <vector>


class FormattedLine : public FormattedLineInterface {
public:
    FormattedLine(std::string& line);

    const std::string& getMessage() const {return formattedLine[4];}
    const std::string& getSender() const {return formattedLine[2];}
    const std::string& getDescription() const {return formattedLine[1];}
    const std::string& getDescriptionCode() const {return formattedLine[0];}
    int getTime() const {return std::stoi(formattedLine[3]);}
    const std::string& getOriginalLine() const {return originalLine;}

    bool isFormatted() const {return formatted;}

private:
    bool format(std::string line);
    std::string findDescriptionCode(std::string& s);
    std::string findDescription(std::string& s);
    std::string findSender(std::string& s);
    std::string findTime(std::string& s);

    bool formatted = true;
    std::string originalLine;
    std::vector<std::string> formattedLine;
};


#endif  // FORMATTED_LINE_H
