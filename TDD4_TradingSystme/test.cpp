#include <iostream>
#include <stdexcept>
#include "gmock/gmock.h"
#include "StockerBrocker.cpp"
#include "NemoDriver.cpp"
#include "KiwerDriver.cpp"

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

	StockMock stockmock;
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
		.Times(1);

	EXPECT_CALL(stockmock, getID)
		.Times(1)
		.WillOnce(Return("KIWER"));

	stockmock.selectStockBrocker(KIWER);
	EXPECT_EQ(stockmock.getID(), KIWER);
}

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

TEST_F(TradingSystemFixture, BuyFromMockSuccess) {
	EXPECT_CALL(stockmock, buy)
		.WillRepeatedly(Return(true));

	EXPECT_EQ(true, stockmock.buy(1, 100, 1));
	EXPECT_EQ(true, stockmock.buy(2, 1000, 10));
	EXPECT_EQ(true, stockmock.buy(3, 50, 100));
}

TEST_F(TradingSystemFixture, BuyFromMockFail) {
	EXPECT_CALL(stockmock, buy)
		.WillRepeatedly(Return(false));

	EXPECT_EQ(false, stockmock.buy(-1, 100, 1));
	EXPECT_EQ(false, stockmock.buy(2, 1000000000, 10));
	EXPECT_EQ(false, stockmock.buy(3, 50, 1000000000));
}

TEST_F(TradingSystemFixture, BuyFromNemoSuccess) {
	NemoStockBroker stock;

	EXPECT_EQ(true, stock.buy(1, 100, 1));
	EXPECT_EQ(true, stock.buy(2, 1000, 10));
	EXPECT_EQ(true, stock.buy(3, 50, 100));
}

TEST_F(TradingSystemFixture, BuyFromNemoFail) {
	NemoStockBroker stock;

	//EXPECT_EQ(false, stock.buy(-1, 100, 1));
	//EXPECT_EQ(false, stock.buy(2, 1000000000, 10));
	//EXPECT_EQ(false, stock.buy(3, 50, 1000000000));
}

TEST_F(TradingSystemFixture, BuyFromKiwerSuccess) {
	KiwerStockBroker stock;

	EXPECT_EQ(true, stock.buy(1, 100, 1));
	EXPECT_EQ(true, stock.buy(2, 1000, 10));
	EXPECT_EQ(true, stock.buy(3, 50, 100));
}

TEST_F(TradingSystemFixture, BuyFromKiwerFail) {
	KiwerStockBroker stock;

	//EXPECT_EQ(false, stock.buy(-1, 100, 1));
	//EXPECT_EQ(false, stock.buy(2, 1000000000, 10));
	//EXPECT_EQ(false, stock.buy(3, 50, 1000000000));
}
