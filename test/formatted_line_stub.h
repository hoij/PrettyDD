#ifndef FORMATTED_LINE_STUB_H
#define FORMATTED_LINE_STUB_H


#include <vector>
#include <string>
#include "formatted_line_interface.h"


class FormattedLineStub : public FormattedLineInterface {
public:
    FormattedLineStub() {}
    FormattedLineStub(std::string description,
                      std::string message,
                      std::string time = "0") {
        formattedLine = {"", description, "", time, message};
    }
    FormattedLineStub(std::string description,
                      std::string message,
                      std::string sender,
                      std::string time = "0") {
        formattedLine = {"", description, sender, time, message};
    }


    const std::string& getDescription() const {return formattedLine[1];}
    const std::string& getDescriptionCode() const {return formattedLine[0];}
    const std::string& getMessage() const {return formattedLine[4];}
    const std::string& getOriginalLine() const {return originalLine;}
    const std::string& getSender() const {return formattedLine[2];}
    std::time_t getTime() const {return (std::time_t)std::stoi(formattedLine[3]);}

    bool isFormatted() const {return true;}

private:
    std::string originalLine = "";
    std::vector<std::string> formattedLine;
};


#endif  // FORMATTED_LINE_STUB_H
