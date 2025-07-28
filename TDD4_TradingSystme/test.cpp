#include <iostream>
#include <stdexcept>
#include "gmock/gmock.h"
#include "StockerBrocker.cpp"

using namespace testing;
using namespace std;

class StockMock : public StockerBrocker {
public:
	MOCK_METHOD(IStocker*, selectStockBrocker, (string id), (override));
	MOCK_METHOD(bool, login, (string id, string pw), (override));
	MOCK_METHOD(bool, buy, (int stockCode, int price, int qty), (override));
	MOCK_METHOD(bool, sell, (int stockCode, int price, int qty), (override));
	MOCK_METHOD(bool, getPrice, (int stockCode), (override));
};
class TradingSystemFixture : public Test {
public:
	const string INVALID = "INVALID";
	const string KIWER = "KIWER";
	const string NEMO = "NEMO";
	const string USER = "USER";
	const string PASSWORD = "PASSWORD";

	StockMock stockmock;
	StockerBrocker stockbrocker;
	IStocker* stock;
};

///////////////  1. Select Stock  ///////////////

TEST_F(TradingSystemFixture, Select_Stock_Fail) {
	EXPECT_CALL(StockMock, selectStockBrocker)
		.Times(1)
		.WillReturnOnce(nullptr);

	EXPECT_THROW(
		{ stock = stockbrocker.selectStockBrocker(INVALID); },
		runtime_error);
}

TEST_F(TradingSystemFixture, Select_Kiwer_Success) {
	EXPECT_CALL(StockMock, selectStockBrocker)
		.Times(1);

	stock = stockbrocker.selectStockBrocker(KIWER);
	EXPECT_EQ(stock->getID(), KIWER);
}

TEST_F(TradingSystemFixture, Select_Nemo_Success) {
	EXPECT_CALL(StockMock, selectStockBrocker)
		.Times(1);

	stock = stockbrocker.selectStockBrocker(NEMO);
	EXPECT_EQ(stock->getID(), NEMO);
}



///////////////     2. Login     ///////////////
TEST_F(TradingSystemFixture, Login_Kiwer_Fail_Invalid_UserID) {
	EXPECT_CALL(StockMock, login)
		.Times(1)
		.WillOnceReturn(false);

	stock = stockbrocker.selectStockBrocker(KIWER);

	EXPECT_EQ(stock->login(INVALID, PASSWORD), false);
}

TEST_F(TradingSystemFixture, Login_Kiwer_Fail_Invalid_Password) {
	EXPECT_CALL(StockMock, login)
		.Times(1)
		.WillOnceReturn(false);

	stock = stockbrocker.selectStockBrocker(KIWER);

	EXPECT_EQ(stock->login(USER, INVALID), false);
}

TEST_F(TradingSystemFixture, Login_Kiwer_Success) {
	EXPECT_CALL(StockMock, login)
		.Times(1)
		.WillOnceReturn(true);

	stock = stockbrocker.selectStockBrocker(KIWER);

	EXPECT_EQ(stock->login(USER, PASSWORD), true);
}
TEST_F(TradingSystemFixture, Login_Nemo_Fail_Invalid_UserID) {
	EXPECT_CALL(StockMock, login)
		.Times(1)
		.WillOnceReturn(false);

	stock = stockbrocker.selectStockBrocker(NEMO);

	EXPECT_EQ(stock->login(INVALID, PASSWORD), false);
}

TEST_F(TradingSystemFixture, Login_Nemo_Fail_Invalid_Password) {
	EXPECT_CALL(StockMock, login)
		.Times(1)
		.WillOnceReturn(false);

	stock = stockbrocker.selectStockBrocker(NEMO);

	EXPECT_EQ(stock->login(USER, INVALID), false);
}

TEST_F(TradingSystemFixture, Login_Nemo_Success) {
	EXPECT_CALL(StockMock, login)
		.Times(1)
		.WillOnceReturn(true);

	stock = stockbrocker.selectStockBrocker(NEMO);

	EXPECT_EQ(stock->login(USER, PASSWORD), true);
}
