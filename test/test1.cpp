#include <iostream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "player.h"

using std::cout;
using std::endl;


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
	static const int i = 3;

};


TEST_F(MyTest, i_equals_3) {
	EXPECT_THAT(i, testing::Eq(3));
}

