#include "read_console.h"

#include <gtest/gtest.h>
#include <string>


TEST(ReadConsole, prepend) {
    std::string command;
    std::string expected;
    
    // Longer string with spaces. "pdd " should be added.
    command = "dtypes Sgtcuddle Shesmine";
    expected = "pdd dtypes Sgtcuddle Shesmine";
    prepend(command);
    EXPECT_EQ(expected, command);
    
    // "pdd" should NOT be added
    command = "pdd dtypes Sgtcuddle Shesmine";
    expected = command;
    prepend(command);
    EXPECT_EQ(expected, command);
    
    // "pdd" should NOT be added
    command = "pddhelp";
    expected = command;
    prepend(command);
    EXPECT_EQ(expected, command);

    // "pdd" should NOT be added
    command = "pdd pdd dtop";
    expected = command;
    prepend(command);
    EXPECT_EQ(expected, command);
    
    // "pdd" should NOT be added
    command = "";
    expected = command;
    prepend(command);
    EXPECT_EQ(expected, command);
    
    // "pdd " should be added
    command = " ";
    expected = "pdd  ";
    prepend(command);
    EXPECT_EQ(expected, command);
}
