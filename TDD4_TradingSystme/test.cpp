#include <iostream>
#include <stdexcept>
#include "gmock/gmock.h"
#include "StockerBrocker.cpp"

using namespace testing;
using namespace std;

class StockMock : public IStockBroker {
public:
	MOCK_METHOD(void, selectStockBrocker, (string id), (override));
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
	const int STOCK_CODE = 1;
	const int PRICE = 10000;
	const int QUANTITY = 10;
	const int ZERO = 0;



	StockMock stockmock;
};

///////////////  1. Select Stock  ///////////////

/// 1.1 FAIL
TEST_F(TradingSystemFixture, Select_Stock_Fail) {

	EXPECT_CALL(stockmock, selectStockBrocker)
		.Times(1)
		.WillOnce(testing::Throw(std::invalid_argument("invalid ID")));

	EXPECT_THROW(
		{ stockmock.selectStockBrocker(INVALID); },
		invalid_argument);
}

/// 1.2 KIWER
TEST_F(TradingSystemFixture, Select_Kiwer_Success) {
	EXPECT_CALL(stockmock, selectStockBrocker)
		.Times(1);

	EXPECT_CALL(stockmock, getID)
		.Times(1)
		.WillOnce(Return("KIWER"));

	stockmock.selectStockBrocker(KIWER);
	EXPECT_EQ(stockmock.getID(), KIWER);
}

/// 1.3 NEMO
TEST_F(TradingSystemFixture, Select_Nemo_Success) {
	EXPECT_CALL(stockmock, selectStockBrocker)
		.Times(1);

	EXPECT_CALL(stockmock, getID)
		.Times(1)
		.WillOnce(Return("NEMO"));
	stockmock.selectStockBrocker(NEMO);
	EXPECT_EQ(stockmock.getID(), NEMO);
}



///////////////     2. Login     ///////////////

/// 2.1 KIWER
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

/// 2.2 NEMO
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


///////////////     3. Buying     ///////////////

/// 3.1 KIWER
TEST_F(TradingSystemFixture, Buying_Kiwer_Fail_Invalid_Code) {
	EXPECT_CALL(StockMock, buy)
		.Times(1)
		.WillOnceReturn(false);

	stock = stockbrocker.selectStockBrocker(KIWER);

	EXPECT_EQ(stock->buy(ZERO, PRICE, QUANTITY), false);
}

TEST_F(TradingSystemFixture, Buying_Kiwer_Fail_Invalid_Price) {
	EXPECT_CALL(StockMock, buy)
		.Times(1)
		.WillOnceReturn(false);

	stock = stockbrocker.selectStockBrocker(KIWER);

	EXPECT_EQ(stock->buy(STOCK_CODE, ZERO, QUANTITY), false);
}

TEST_F(TradingSystemFixture, Buying_Kiwer_Fail_Invalid_Quantity) {
	EXPECT_CALL(StockMock, buy)
		.Times(1)
		.WillOnceReturn(false);

	stock = stockbrocker.selectStockBrocker(KIWER);

	EXPECT_EQ(stock->buy(STOCK_CODE, PRICE, ZERO), false);
}

TEST_F(TradingSystemFixture, Buying_Kiwer_Success) {
	EXPECT_CALL(StockMock, buy)
		.Times(1)
		.WillOnceReturn(true);

	stock = stockbrocker.selectStockBrocker(KIWER);

	EXPECT_EQ(stock->buy(STOCK_CODE, PRICE, QUANTITY), true);
}


/// 3.2 NEMO
TEST_F(TradingSystemFixture, Buying_Nemo_Fail_Invalid_Code) {
	EXPECT_CALL(StockMock, buy)
		.Times(1)
		.WillOnceReturn(false);

	stock = stockbrocker.selectStockBrocker(KIWER);

	EXPECT_EQ(stock->buy(ZERO, PRICE, QUANTITY), false);
}

TEST_F(TradingSystemFixture, Buying_Nemo_Fail_Invalid_Price) {
	EXPECT_CALL(StockMock, buy)
		.Times(1)
		.WillOnceReturn(false);

	stock = stockbrocker.selectStockBrocker(KIWER);

	EXPECT_EQ(stock->buy(STOCK_CODE, ZERO, QUANTITY), false);
}

TEST_F(TradingSystemFixture, Buying_Nemo_Fail_Invalid_Quantity) {
	EXPECT_CALL(StockMock, buy)
		.Times(1)
		.WillOnceReturn(false);

	stock = stockbrocker.selectStockBrocker(KIWER);

	EXPECT_EQ(stock->buy(STOCK_CODE, PRICE, ZERO), false);
}

TEST_F(TradingSystemFixture, Buying_Nemo_Success) {
	EXPECT_CALL(StockMock, buy)
		.Times(1)
		.WillOnceReturn(true);

	stock = stockbrocker.selectStockBrocker(KIWER);

	EXPECT_EQ(stock->buy(STOCK_CODE, PRICE, QUANTITY), true);
}
