#include "line_info.h"


LineInfo::LineInfo() {
    nanoProgram = new NanoProgram;
}

LineInfo::~LineInfo() {
    delete nanoProgram;
}

LineInfo::LineInfo(const LineInfo& li) {
    *nanoProgram = *(li.nanoProgram);
}

LineInfo& LineInfo::operator=(LineInfo li) {
    *nanoProgram = *(li.nanoProgram);
    return *this;
}
