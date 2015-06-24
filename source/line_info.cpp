#include "line_info.h"


LineInfo::LineInfo() {
    nanoProgram = new NanoProgram;
}

LineInfo::~LineInfo() {
    delete nanoProgram;
}
