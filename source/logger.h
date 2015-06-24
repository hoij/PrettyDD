#ifndef LOGGER_H
#define LOGGER_H


#include <fstream>
#include <string>


class Logger {
public:
    template<typename T>
    void write(const T& message, bool newline = true) {
        std::ofstream logfile("errors.txt", std::ios::app);
        if (logfile.is_open()) {
            logfile << message;
            if (newline) {
                logfile << std::endl;
            }
        }
        else {
            std::cerr << "Could not open the errors log for writing." <<
            std::endl;
        }
    }
private:

};

extern Logger errorLog;

#endif  // LOGGER_H
