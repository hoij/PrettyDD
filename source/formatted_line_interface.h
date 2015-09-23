#ifndef FORMATTED_LINE_INTERFACE_H
#define FORMATTED_LINE_INTERFACE_H


#include <ctime>
#include <string>
#include <vector>


class FormattedLineInterface {
public:
    virtual ~FormattedLineInterface() {};

    virtual const std::string& getDescription() const = 0;
    virtual const std::string& getDescriptionCode() const = 0;
    virtual const std::string& getMessage() const = 0;
    virtual const std::string& getOriginalLine() const = 0;
    virtual const std::string& getSender() const = 0;
    virtual std::time_t getTime() const = 0;

    virtual bool isFormatted() const = 0;
};


#endif  // FORMATTED_LINE_INTERFACE_H
