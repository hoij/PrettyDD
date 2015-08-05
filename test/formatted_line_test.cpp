#include "formatted_line.h"

#include <gtest/gtest.h>
#include <string>


TEST(FormattedLineTest, typicalMessage) {
    std::string originalLine = "[\"#000000004200000a#\",\"Other hit by other\",\"\",1425997610]Letter hit Reet of Paradise for 1586 points of melee damage.Critical hit!";
    FormattedLine fl(originalLine);
    EXPECT_EQ("Letter hit Reet of Paradise for 1586 points of melee damage.Critical hit!",
              fl.getMessage());
    EXPECT_EQ("", fl.getSender());
    EXPECT_EQ("Other hit by other", fl.getDescription());
    EXPECT_EQ(originalLine, fl.getOriginalLine());
    EXPECT_EQ(1425997610, fl.getTime());
    EXPECT_TRUE(fl.isFormatted());
}

TEST(FormattedLineTest, messageWithSeveralCommas) {
    std::string originalLine = "[\"#0000000040000002#\",\"Vicinity\",\"Sgtcuddle\",1436181663]This, line, has, several, commas,";
    FormattedLine fl(originalLine);
    EXPECT_EQ("This, line, has, several, commas,", fl.getMessage());
    EXPECT_EQ("Sgtcuddle", fl.getSender());
    EXPECT_EQ("Vicinity", fl.getDescription());
    EXPECT_EQ("0000000040000002", fl.getDescriptionCode());
    EXPECT_EQ(originalLine, fl.getOriginalLine());
    EXPECT_EQ(1436181663, fl.getTime());
    EXPECT_TRUE(fl.isFormatted());
}

TEST(FormattedLineTest, messageWithBrackets) {
    // TODO: Improve or remove this test as FormattedLine no longer splits on
    // brackets.
    std::string originalLine = "[\"#0000000040000002#\",\"Vicinity\",\"Panthbot\",1436181673][Pantheon]Kain: [Something insightful]";
    FormattedLine fl(originalLine);
    EXPECT_EQ("[Pantheon]Kain: [Something insightful]", fl.getMessage());
    EXPECT_EQ("Panthbot", fl.getSender());
    EXPECT_EQ("Vicinity", fl.getDescription());
    EXPECT_EQ("0000000040000002", fl.getDescriptionCode());
    EXPECT_EQ(originalLine, fl.getOriginalLine());
    EXPECT_EQ(1436181673, fl.getTime());
    EXPECT_TRUE(fl.isFormatted());
}

TEST(FormattedLineTest, orgNameWithComma) {
    std::string originalLine = "[\"#00000003000011fc#\",\"Pantheon, inc\",\"Sgtcuddle\",1436182478]test";
    FormattedLine fl(originalLine);
    EXPECT_EQ("test", fl.getMessage());
    EXPECT_EQ("Sgtcuddle", fl.getSender());
    EXPECT_EQ("Pantheon, inc", fl.getDescription());
    EXPECT_EQ("00000003000011fc", fl.getDescriptionCode());
    EXPECT_EQ(originalLine, fl.getOriginalLine());
    EXPECT_EQ(1436182478, fl.getTime());
    EXPECT_TRUE(fl.isFormatted());
}

TEST(FormattedLineTest, orgNameWithBrackets) {
    // TODO: Improve or remove this test as FormattedLine no longer splits on
    // brackets.
    std::string originalLine = "[\"#00000003000011fc#\",\"Pantheon [YOLO]\",\"Sgtcuddle\",1436182498]test";
    FormattedLine fl(originalLine);
    EXPECT_EQ("test", fl.getMessage());
    EXPECT_EQ("Sgtcuddle", fl.getSender());
    EXPECT_EQ("Pantheon [YOLO]", fl.getDescription());
    EXPECT_EQ("00000003000011fc", fl.getDescriptionCode());
    EXPECT_EQ(originalLine, fl.getOriginalLine());
    EXPECT_EQ(1436182498, fl.getTime());
    EXPECT_TRUE(fl.isFormatted());
}

TEST(FormattedLineTest, emptyMessage) {
    // It may not be possible to send an empty message in game.
    std::string originalLine = "[\"#0000000040000002#\",\"Vicinity\",\"Sgtcuddle\",1436181664]";
    FormattedLine fl(originalLine);
    EXPECT_EQ("", fl.getMessage());
    EXPECT_EQ("Sgtcuddle", fl.getSender());
    EXPECT_EQ("Vicinity", fl.getDescription());
    EXPECT_EQ("0000000040000002", fl.getDescriptionCode());
    EXPECT_EQ(originalLine, fl.getOriginalLine());
    EXPECT_EQ(1436181664, fl.getTime());
    EXPECT_TRUE(fl.isFormatted());
}

TEST(FormattedLineTest, teamChat) {
    // Team chat is special as it doesn't have a code in the same format as
    // the rest of the lines.
    std::string originalLine = "[\"Team\",\"Team\",\"Sgtcuddle\",1436182391]test";
    FormattedLine fl(originalLine);
    EXPECT_EQ("test", fl.getMessage());
    EXPECT_EQ("Sgtcuddle", fl.getSender());
    EXPECT_EQ("Team", fl.getDescription());
    EXPECT_EQ(originalLine, fl.getOriginalLine());
    EXPECT_EQ(1436182391, fl.getTime());
    EXPECT_TRUE(fl.isFormatted());
}

TEST(FormattedLineTest, emptyLine) {
    // Calling any of the getters without first checking if
    // isFormatted() == true can result in a crash if the line was not formatted.
    std::string originalLine = "";
    FormattedLine fl(originalLine);
    EXPECT_EQ(originalLine, fl.getOriginalLine());
    EXPECT_FALSE(fl.isFormatted());
}
