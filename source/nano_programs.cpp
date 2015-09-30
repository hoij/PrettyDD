#include "logger.h"
#include "nano_programs.h"


void NanoPrograms::add(LineInfo& li) {
    if (!li.nanoProgramName.empty()) {
        // When a nano program is casted there is no message about the
        // success of the cast. That success info arrives in the next
        // message. So in this case, the name is only saved.
        nameOfLastNanoProgramCasted = li.nanoProgramName;
    }
    else if (!nameOfLastNanoProgramCasted.empty()){
        // If nano casted successfully/resisted/countered/aborted/fumble,
        // the message won't contain a name. The previously saved name
        // is added with the new subtype info (the success info).
        //
        // When a proc fires or when certain nanos execute two programs on
        // one cast, the nano program status message (executed/countered etc.)
        // will appear twice or more after the "Executing Nano Program:"
        // message. To prevent incrementing the same nano again, the name of
        // the last nano program is cleared.
        addNanoProgram(li.subtype);
        nameOfLastNanoProgramCasted.clear();
    }
    else {
        // TODO: Remove this once tested that it works.
        // TODO: Check log after a raid to see that only the correct
        // messages are put here.
        errorLog.write("Warning: The NanoProgram info below should "
            "correspond to \"Nano program executed successfully.\":");
        errorLog.write("\tName: " + li.nanoProgramName);
        errorLog.write("\tType: " + li.type);
        errorLog.write("\tSubtype: " + li.subtype);
    }
}

void NanoPrograms::addNanoProgram(std::string& subtype) {
    NanoPrograms::NanoProgram& nanoProgram =
        nanoPrograms[nameOfLastNanoProgramCasted];
    nanoProgram.executes += 1;
    if (subtype == "land") {
        nanoProgram.lands += 1;
    }
    else if (subtype == "resist") {
        nanoProgram.resists += 1;
    }
    else if (subtype == "abort") {
        nanoProgram.aborts += 1;
    }
    else if (subtype == "counter") {
        nanoProgram.counters += 1;
    }
    else if (subtype == "fumble") {
        nanoProgram.fumbles += 1;
    }
}

std::vector<std::string> NanoPrograms::getNanoProgramNames() const {
    std::vector<std::string> nanoProgramNames;
    for (const auto& nanoProgram : nanoPrograms) {
        nanoProgramNames.push_back(nanoProgram.first);
    }
    return nanoProgramNames;
}

int NanoPrograms::getExecutes(std::string& name) {
    auto it = nanoPrograms.find(name);
    return (it != nanoPrograms.end()) ? it->second.executes : 0;
}
int NanoPrograms::getLands(std::string& name) {
    auto it = nanoPrograms.find(name);
    return (it != nanoPrograms.end()) ? it->second.lands : 0;
}
int NanoPrograms::getResists(std::string& name) {
    auto it = nanoPrograms.find(name);
    return (it != nanoPrograms.end()) ? it->second.resists : 0;
}
int NanoPrograms::getAborts(std::string& name) {
    auto it = nanoPrograms.find(name);
    return (it != nanoPrograms.end()) ? it->second.aborts : 0;
}
int NanoPrograms::getCounters(std::string& name) {
    auto it = nanoPrograms.find(name);
    return (it != nanoPrograms.end()) ? it->second.counters : 0;
}
int NanoPrograms::getFumbles(std::string& name) {
    auto it = nanoPrograms.find(name);
    return (it != nanoPrograms.end()) ? it->second.fumbles : 0;
}
