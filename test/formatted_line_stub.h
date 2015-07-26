#ifndef FORMATTED_LINE_STUB_H
#define FORMATTED_LINE_STUB_H


#include <vector>
#include <string>
#include "formatted_line_interface.h"


class FormattedLineStub : public FormattedLineInterface{
public:
    FormattedLineStub() {}
    FormattedLineStub(std::string description, std::string message) {
        formattedLine = { "", description, "", "", message};
    }
    FormattedLineStub(std::string description, std::string message, std::string sender) {
        formattedLine = { "", description, sender, "", message};
    }

    const std::vector<std::string>& getFullLine() const {return formattedLine;}
    const std::string& getMessage() const {return formattedLine[4];}
    const std::string& getSender() const {return formattedLine[2];}
    const std::string& getDescription() const {return formattedLine[1];}
    const std::string& getOriginalLine() const {return originalLine;}

    bool isFormatted() const {return true;}

private:
    std::string originalLine = "";
    std::vector<std::string> formattedLine;
};


#endif  // FORMATTED_LINE_STUB_H
