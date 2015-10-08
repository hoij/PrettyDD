#ifndef NEWLINE_H
#define NEWLINE_H

#include "configuration.h"

#include <ostream>


class NewLine {
public:
    NewLine(Configuration& config) : config(config) {}
    NewLine& operator=(NewLine rhs) = delete;
    friend std::ostream& operator<<(std::ostream& os, NewLine& nl) {
        if (nl.config.shouldWriteReadable()) {
            os << std::endl;
        }
        return os;
    }
private:
    Configuration& config;
};

#endif  //NEWLINE_H