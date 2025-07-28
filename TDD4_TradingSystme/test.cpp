#include <iostream>
#include <stdexcept>
#include "gmock/gmock.h"
#include "StockerBrocker.cpp"

using namespace testing;
using namespace std;

class StockMock : public IStockBroker {
public:
	MOCK_METHOD(bool, selectStockBrocker, (string id), (override));
	MOCK_METHOD(bool, login, (string id, string pw), (override));
	MOCK_METHOD(bool, buy, (int stockCode, int price, int qty), (override));
	MOCK_METHOD(bool, sell, (int stockCode, int price, int qty), (override));
	MOCK_METHOD(bool, getPrice, (int stockCode), (override));
	MOCK_METHOD(string, getID, (), (override));
};

class TradingSystemFixture : public Test {
public:
	const string INVALID = "INVALID";
	const string KIWER = "KIWER";
	const string NEMO = "NEMO";
	const string USER = "USER";
	const string PASSWORD = "PASSWORD";

	StockMock stockmock;
	bool ret;

};

///////////////  1. Select Stock  ///////////////

TEST_F(TradingSystemFixture, Select_Stock_Fail) {

	EXPECT_CALL(stockmock, selectStockBrocker)
		.Times(1)
		.WillOnce(testing::Throw(std::invalid_argument("invalid ID")));

	EXPECT_THROW(
		{ stockmock.selectStockBrocker(INVALID); },
		invalid_argument);
}

TEST_F(TradingSystemFixture, Select_Kiwer_Success) {
	EXPECT_CALL(stockmock, selectStockBrocker)
		.Times(1)
		.WillOnce(Return(true));

	EXPECT_CALL(stockmock, getID)
		.Times(1)
		.WillOnce(Return("KIWER"));

	ret = stockmock.selectStockBrocker(KIWER);
	EXPECT_EQ(ret, true);
	EXPECT_EQ(stockmock.getID(), KIWER);
}

TEST_F(TradingSystemFixture, Select_Nemo_Success) {
	EXPECT_CALL(stockmock, selectStockBrocker)
		.Times(1)
		.WillOnce(Return(true));

	EXPECT_CALL(stockmock, getID)
		.Times(1)
		.WillOnce(Return("NEMO"));
	ret = stockmock.selectStockBrocker(NEMO);
	EXPECT_EQ(ret, true);
	EXPECT_EQ(stockmock.getID(), NEMO);
}



///////////////     2. Login     ///////////////
TEST_F(TradingSystemFixture, Login_Kiwer_Fail_Invalid_UserID) {
	EXPECT_CALL(stockmock, login)
		.Times(1)
		.WillOnce(Return(false));
	stockmock.selectStockBrocker(KIWER);

	EXPECT_EQ(stockmock.login(INVALID, PASSWORD), false);
}

TEST_F(TradingSystemFixture, Login_Kiwer_Fail_Invalid_Password) {
	EXPECT_CALL(stockmock, login)
		.Times(1)
		.WillOnce(Return(false));


	stockmock.selectStockBrocker(KIWER);

	EXPECT_EQ(stockmock.login(USER, INVALID), false);
}

TEST_F(TradingSystemFixture, Login_Kiwer_Success) {
	EXPECT_CALL(stockmock, login)
		.Times(1)
		.WillOnce(Return(true));


	stockmock.selectStockBrocker(KIWER);

	EXPECT_EQ(stockmock.login(USER, PASSWORD), true);
}
TEST_F(TradingSystemFixture, Login_Nemo_Fail_Invalid_UserID) {
	EXPECT_CALL(stockmock, login)
		.Times(1)
		.WillOnce(Return(false));


	stockmock.selectStockBrocker(NEMO);

	EXPECT_EQ(stockmock.login(INVALID, PASSWORD), false);
}

TEST_F(TradingSystemFixture, Login_Nemo_Fail_Invalid_Password) {
	EXPECT_CALL(stockmock, login)
		.Times(1)
		.WillOnce(Return(false));


	stockmock.selectStockBrocker(NEMO);

	EXPECT_EQ(stockmock.login(USER, INVALID), false);
}

TEST_F(TradingSystemFixture, Login_Nemo_Success) {
	EXPECT_CALL(stockmock, login)
		.Times(1)
		.WillOnce(Return(true));


	stockmock.selectStockBrocker(NEMO);

	EXPECT_EQ(stockmock.login(USER, PASSWORD), true);
}
