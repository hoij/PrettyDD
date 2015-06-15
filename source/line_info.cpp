#include "line_info.h"

#include <iostream>

LineInfo::LineInfo() {
    nanoProgram = new NanoProgram;
}

LineInfo::~LineInfo() {
    delete nanoProgram;
}
