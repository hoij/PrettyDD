#ifndef FORMATTED_LINE_INTERFACE_H
#define FORMATTED_LINE_INTERFACE_H


#include <vector>
#include <string>


class FormattedLineInterface {
public:
    FormattedLineInterface() {};

    virtual const std::vector<std::string>& getFullLine() const = 0;
    virtual const std::string& getMessage() const = 0;
    virtual const std::string& getSender() const = 0;
    virtual const std::string& getDescription() const = 0;
    virtual const std::string& getOriginalLine() const = 0;

    virtual bool isFormatted() const = 0;
};


#endif  // FORMATTED_LINE_INTERFACE_H
