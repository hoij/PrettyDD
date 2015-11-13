#include "console_reader.h"
#include "line_info.h"
#include "my_time.h"

#include <gtest/gtest.h>

#include <iostream>
#include <istream>
#include <sstream>

TEST(ConsoleReader, read) {
    MyTime myTime;
    ConsoleReader consoleReader(myTime);
    LineInfo result;
    std::stringstream testInput;
    std::string command;
    std::string expected;
    
    // Longer string with spaces. "pdd " should be added.
    command = "dtypes Sgtcuddle Shesmine";
    testInput.clear();
    testInput.str(command);
    expected = "pdd dtypes Sgtcuddle Shesmine";
    result = consoleReader.read(testInput);
    EXPECT_EQ(expected, result.command);
    
    // "pdd" should NOT be added
    command = "pdd dtypes Sgtcuddle Shesmine";
    testInput.clear();
    testInput.str(command);
    expected = command;
    result = consoleReader.read(testInput);
    EXPECT_EQ(expected, result.command);
    
    // "pdd" should NOT be added
    command = "pddhelp";
    testInput.clear();
    testInput.str(command);
    expected = command;
    result = consoleReader.read(testInput);
    EXPECT_EQ(expected, result.command);

    // "pdd" should NOT be added
    command = "pdd pdd dtop";
    testInput.clear();
    testInput.str(command);
    expected = command;
    result = consoleReader.read(testInput);
    EXPECT_EQ(expected, result.command);
    
    // "pdd" should NOT be added
    command = "";
    testInput.clear();
    testInput.str(command);
    expected = command;
    result = consoleReader.read(testInput);
    EXPECT_EQ(expected, result.command);
    
    // "pdd " should be added
    command = " ";
    testInput.clear();
    testInput.str(command);
    expected = "pdd  ";
    result = consoleReader.read(testInput);
    EXPECT_EQ(expected, result.command);
    
    // "pdd" should not be added
//     command = "pdd help\n pdd top";
//     testInput.clear();
//     testInput.str(command);
//     expected = command;
//     result = consoleReader.read(testInput);
//     EXPECT_EQ(expected, result.command);
}