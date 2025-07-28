#include <iostream>
#include <stdexcept>
#include "gmock/gmock.h"

using namespace testing;
using namespace std;

class TradingSystemFixture : public Test {
public:

};

/* Select Stock */
TEST_F(TradingSystemFixture, Select_Stock_Fail) {

	// EXPECT_THROW( getStock(), runtime_error);
}

TEST_F(TradingSystemFixture, Select_Kiwer_Success) {

	// EXPECT_EQ(getStock(), string{"KIWER is selected"});
}

TEST_F(TradingSystemFixture, Select_Nemo_Success) {

	// EXPECT_EQ(getStock(), string{"NEMO is selected"});
}



/* Login */
TEST_F(TradingSystemFixture, Login_Kiwer_Fail) {

	// EXPECT_THROW( login(), runtime_error);
}

TEST_F(TradingSystemFixture, Login_Kiwer_Success) {

	// EXPECT_EQ(login(), "KIWER Login Success");
}

TEST_F(TradingSystemFixture, Login_Nemo_Fail) {

	// EXPECT_THROW( login(), runtime_error);
}

TEST_F(TradingSystemFixture, Login_Nemo_Success) {

	// EXPECT_EQ(login(), "Nemo Login Success");
}
