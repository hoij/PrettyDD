#include <iostream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "player.h"

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
	}
	virtual void TearDown() {
		cout << "Running TearDown" << endl;
	}
};


class TestSubject {
public:
  int getI() {return i;}
private:
  int i = 3;
};

TEST(test_cases, i_equals_3) {
  TestSubject ts;
  EXPECT_EQ(3, ts.getI());
}

