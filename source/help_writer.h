#ifndef HELP_WRITER_H
#define HELP_WRITER_H


#include "configuration.h"
#include "writer_helper.h"

#include <fstream>
#include <string>


class HelpWriter : public WriterHelper {
public:
    HelpWriter(Configuration& config, std::ofstream& file);

    void createHelp();

private:
    void createCommands1();
    void createCommands2();
    void createInfo1();
    void createInfo2();
    void writeCommand(std::string command, std::string description = "");
};



#endif  //HELP_WRITER_H