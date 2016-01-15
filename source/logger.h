#ifndef LOGGER_H
#define LOGGER_H


#include <fstream>
#include <iostream>
#include <string>


class Logger {
public:
    Logger(std::string fileName) : fileName_(fileName) {}

    void open() {
        logfile.open("pddlog.txt", std::ios::app);
        if (!logfile) {
            std::cerr << "Could not open pddlog.txt for writing." <<
                         std::endl;
        }
    }

    template<typename T>
    void write(const T& message) {
            logfile << message << '\n';
    }

private:
    std::ofstream logfile;
    const std::string fileName_;
};

extern Logger errorLog;

#endif  // LOGGER_H
