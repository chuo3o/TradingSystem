#include <iostream>
#include "gmock/gmock.h"

using namespace testing;

class TradingSystemFixture : public Test {
public:

};


TEST_F(TradingSystemFixture, TC1) {
	EXPECT_EQ(1, 1);
}