#include "nano_programs.h"
#include "line_info.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>


class NanoProgramsTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        nanoPrograms = new NanoPrograms;
    }
    virtual void TearDown() {
        delete nanoPrograms;
    }

    void addNanoProgram(std::string nanoProgramName, std::string subtype);
    void verifyNanoProgram(std::string nanoProgramName,
                           int executes,
                           int lands,
                           int resists,
                           int aborts,
                           int counters,
                           int fumbles);

    NanoPrograms* nanoPrograms;
};

void NanoProgramsTest::addNanoProgram(std::string nanoProgramName,
                                      std::string subtype) {
    LineInfo li;
    li.nanoProgramName = nanoProgramName;
    li.subtype = subtype;
    nanoPrograms->add(li);
}

void NanoProgramsTest::verifyNanoProgram(std::string name,
                                         int executes,
                                         int lands,
                                         int resists,
                                         int aborts,
                                         int counters,
                                         int fumbles) {
    EXPECT_EQ(executes, nanoPrograms->getExecutes(name));
    EXPECT_EQ(lands, nanoPrograms->getLands(name));
    EXPECT_EQ(resists, nanoPrograms->getResists(name));
    EXPECT_EQ(aborts, nanoPrograms->getAborts(name));
    EXPECT_EQ(counters, nanoPrograms->getCounters(name));
    EXPECT_EQ(fumbles, nanoPrograms->getFumbles(name));
}

TEST_F(NanoProgramsTest, add) {
    /* Tests:
    Adding the same nano several times in a row.
    Two "Nano Program Name executed successfully" messages in a row ->
        The second should not be added.
    Add different nano programs.
    Add all subtypes (result messages) twice
    */
    std::string nano1 = "nano program 1";
    std::string result1 = "land";
    addNanoProgram(nano1, "execute");
    addNanoProgram("", result1);
    addNanoProgram(nano1, "execute");
    addNanoProgram("", result1);
    addNanoProgram("", result1);

    std::string nano2 = "nano2";
    addNanoProgram(nano2, "execute");
    addNanoProgram("", "resist");
    addNanoProgram(nano2, "execute");
    addNanoProgram("", "resist");

    addNanoProgram(nano1, "execute");
    addNanoProgram("", "abort");
    addNanoProgram(nano1, "execute");
    addNanoProgram("", "abort");

    addNanoProgram(nano1, "execute");
    addNanoProgram("", "counter");
    addNanoProgram(nano1, "execute");
    addNanoProgram("", "counter");

    std::string nano3 = "nano3";
    addNanoProgram(nano3, "execute");
    addNanoProgram("", "fumble");
    addNanoProgram(nano3, "execute");
    addNanoProgram("", "fumble");

    std::vector<std::string> nanos = {nano1, nano2, nano3};
    EXPECT_THAT(nanos, testing::ContainerEq(nanoPrograms->getNanoProgramNames()));
    verifyNanoProgram(nano1, 6, 2, 0, 2, 2, 0);
    verifyNanoProgram(nano2, 2, 0, 2, 0, 0, 0);
    verifyNanoProgram(nano3, 2, 0, 0, 0, 0, 2);
}


TEST_F(NanoProgramsTest, getNonexistingNanoProgram) {
    /* No nano program has been added so the getters should
    return a standardvalue */
    std::string np = "nano program";
    EXPECT_EQ(0, nanoPrograms->getExecutes(np));
    EXPECT_EQ(0, nanoPrograms->getLands(np));
    EXPECT_EQ(0, nanoPrograms->getResists(np));
    EXPECT_EQ(0, nanoPrograms->getAborts(np));
    EXPECT_EQ(0, nanoPrograms->getCounters(np));
    EXPECT_EQ(0, nanoPrograms->getFumbles(np));
}

