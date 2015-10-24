#ifndef WRITER_HELPER_H
#define WRITER_HELPER_H


#include "configuration.h"
#include "newline.h"

#include <fstream>
#include <memory>
#include <ostream>
#include <string>


class WriterHelper {
public:
    WriterHelper(Configuration& config, std::ofstream& file);
    WriterHelper& operator=(WriterHelper rhs) = delete;

    bool openFile(std::string fileName);
    void closeFile();
    void writeStartOfLink(std::string title);
    void writeEndOfLink(std::string title);
    void writePlace(int place);
    void writeName(std::string name);
    std::string appendInterval(std::string titleBase,
                               int startOffset,
                               int stopOffset);
    void createNotFoundMessage(std::string title,
                               std::string message);

    unsigned int calcNrOfWindows(unsigned int nrOfType,
                                 unsigned int typesPerWindow);
    unsigned int calcNrOfFiles(unsigned int nrOfWindows,
                                 unsigned int windowsPerFile);
    std::string percentage(int total, int part);
    std::string dblToString(const double d);

    std::string determineMin(int amount);
    std::string determineMax(int amount);

protected:
    // Colors
    const std::string lime = "#00FF00";
    const std::string lightBlue = "#3399FF";
    const std::string yellow = "#FFFF00";

    const char fillChar = '_';

    Configuration& config;
    std::ofstream& file;
    NewLine nl;
};


#endif  //WRITER_HELPER_H
