#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "parse.h"
#include "line_info.h"

using std::cout;
using std::endl;


// This is a fixture
// More test can be added by
// TEST_F(MyTest, testName) {
//   // actual testing
// }
class MyTest : public testing::Test {
protected:
	MyTest() {}
	~MyTest() {}

	virtual void SetUp() {
		cout << "Running SetUp" << endl;
		ifstream logStream("me_cast_nano.txt");
		if (!logStream.is_open()) {
            // Fail the test using some Gtest function.
		}
	}
	virtual void TearDown() {
		cout << "Running TearDown" << endl;
		logStream.close();
	}
};


class TestSubject {
public:
  int getI() {return i;}
private:
  int i = 3;
};

TEST(test_cases, me_cast_nano) {
    std::vector<LineInfo> expected;
    LineInfo expected_li;

    exp_li.nanoProgram.setName() = "Improved Mongo Crush!";
    exp_li.dealer_name = "You";
    exp_li.type = "nano_cast";
    exp_li.subtype = "execute";
    exp_li.nanoProgram.addStat("execute", 1);


    while (getline(logStream, line)) {
        LineInfo li;
        cout << line << endl;
        split(line, ',', words);
        cleanup(words);
        find_values(words, li);
        EXPECT_EQ();
        EXPECT_STREQ(exp_li.nanoProgram.getName(), li.nanoProgram.getName(); )
    }

  EXPECT_EQ(3, ts.getI());
}

