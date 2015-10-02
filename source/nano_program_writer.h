#ifndef NANO_PROGRAM_WRITER_H
#define NANO_PROGRAM_WRITER_H


#include "configuration.h"
#include "nano_programs.h"
#include "writer_helper.h"

#include <ostream>


class NanoProgramWriter : public WriterHelper {
public:
    NanoProgramWriter(Configuration& config,
                      std::ofstream& file) : WriterHelper(config, file) {}
    // Can only be done for "You".
    void createCastedDetailedList(const NanoPrograms& nanoPrograms);

private:
    std::ostream& writeDetailedListHeadings(std::ostream& os);
    std::vector<std::string>::iterator getStopIter(
        std::vector<std::string>& nanoProgramNames,
        unsigned int windowNr,
        unsigned int nanosPerWindow);
    std::ostream& writeDetailedList(
        std::vector<std::string>::iterator namesStart,
        std::vector<std::string>::iterator namesStop,
        const NanoPrograms& nanoPrograms,
        std::ostream& os);
    void sortByExecutes(std::vector<std::string>& v,
                        const NanoPrograms& nanoPrograms);
};


#endif  //NANO_PROGRAM_WRITER_H
