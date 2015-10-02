#ifndef WRITER_HELPER_H
#define WRITER_HELPER_H


#include "configuration.h"

#include <fstream>
#include <memory>
#include <ostream>
#include <string>


class WriterHelper {
public:
    WriterHelper(Configuration& config, std::ofstream& file);
    //std::unique_ptr<std::ofstream> openFile();
    bool openFile();
    void closeFile();
    void writeStartOfLink(std::string title);
    void writeEndOfLink(std::string title);

    void writePlace(int place);
    void writeName(std::string name);
    std::string appendInterval(std::string titleBase,
                               unsigned int windowNr,
                               unsigned int nanosPerWindow);
    void createNotFoundMessage(std::string title,
                               std::string message,
                               std::ostream& os);

    unsigned int calcNrOFWindows(unsigned int nrOfType,
                                 unsigned int typesPerWindow);
    std::string percentage(int total, int part);
    std::string dblToString(const double d);

protected:
    // Colors
    const std::string lime = "#00FF00";
    const std::string lightBlue = "#3399FF";
    const std::string yellow = "#FFFF00";

    const char fillChar = '_';

    Configuration& config;
    std::ofstream& file;
};


#endif  //WRITER_HELPER_H
