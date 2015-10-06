#ifndef NANO_PROGRAM_WRITER_H
#define NANO_PROGRAM_WRITER_H


#include "configuration.h"
#include "nano_programs.h"
#include "player_vector.h"
#include "writer_helper.h"

#include <fstream>


class NanoProgramWriter : public WriterHelper {
public:
    NanoProgramWriter(PlayerVector<Player*>& playerVector,
                      Configuration& config,
                      std::ofstream& file);

    // Can only be done for "You".
    void createCastedDetailedList();

private:
    void writeDetailedListHeadings();
    std::vector<std::string>::iterator getStopIter(
        std::vector<std::string>& nanoProgramNames,
        unsigned int windowNr,
        unsigned int nanosPerWindow);
    void writeDetailedList(
        std::vector<std::string>::iterator namesStart,
        std::vector<std::string>::iterator namesStop,
        const NanoPrograms& nanoPrograms);
    void sortByExecutes(std::vector<std::string>& v,
                        const NanoPrograms& nanoPrograms);

    PlayerVector<Player*>& playerVector;
};


#endif  //NANO_PROGRAM_WRITER_H
