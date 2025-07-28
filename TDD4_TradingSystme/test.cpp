#include <iostream>
#include <stdexcept>
#include "gmock/gmock.h"
#include "StockerBroker.cpp"

using namespace testing;
using namespace std;

class StockMock : public IStock {
public:
	MOCK_METHOD(bool, login, (string id, string pw), (override));
	MOCK_METHOD(bool, buy, (int stockCode, int price, int qty), (override));
	MOCK_METHOD(bool, sell, (int stockCode, int price, int qty), (override));
	MOCK_METHOD(int, getPrice, (int stockCode), (override));
};

class StockBrokerMock : public IStockBroker {
public:
	MOCK_METHOD(void, selectStockBroker, (string id), (override));
	MOCK_METHOD(IStock*, getStockBroker, (), (override));
	MOCK_METHOD(string, getID, (), (override));
};


class TradingSystemFixture : public Test {
public:
	const string INVALID = "INVALID";
	const string KIWER = "KIWER";
	const string NEMO = "NEMO";
	const string USER = "USER";
	const string PASSWORD = "PASSWORD";
	const int INVALID_CODE = 0;
	const int STOCK_CODE = 1;
	const int PRICE = 10000;
	const int QUANTITY = 10;
	const int ZERO = 0;
	const int OVER = 10000;


	NiceMock<StockBrokerMock> stockBrokermock;
	NiceMock<StockMock> stockmock;

	std::string getKiwerLoginMsg(const std::string & id, const std::string & pw) const {
		if(id == INVALID) {
			throw std::invalid_argument("[KIWER] Wrong ID");
		}
		else if(pw == INVALID) {
			throw std::invalid_argument("[KIWER] Wrong PW");
		}
		return id +" login success\n";
	}
	std::string getNemoLoginMsg(const std::string &id, const std::string & pw) const {
		if(id == INVALID) {
			throw std::invalid_argument("[NEMO] Wrong ID");
		}
		else if(pw == INVALID) {
			throw std::invalid_argument("[NEMO] Wrong PW");
		}
		return "[NEMO]" + id + " login GOOD\n";
	}

	std::string getLoginMsg(
		bool isSuccess, const std::string & brokerType, 
		const std::string & id, const std::string & pw) {

		oss.str(""); oss.clear();
		auto oldCoutStreamBuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

		//EXPECT_CALL(stockmock, selectStockBroker)
		//	.WillOnce(Return(true));

		//stockmock.selectStockBroker(brokerType);
		EXPECT_CALL(stockmock, login)
			.WillOnce(
				[&]() {
					if(brokerType == KIWER) {
						std::cout << getKiwerLoginMsg(id, pw);
					}
					else if(brokerType == NEMO) {
						std::cout << getNemoLoginMsg(id, pw);
					}
					else {
						throw std::invalid_argument("WRONG brokerType");
					}

					return isSuccess;
				}
			);
		EXPECT_EQ(stockmock.login(id, pw), isSuccess);

		std::cout.rdbuf(oldCoutStreamBuf);
		return oss.str();
	}
private:
	std::ostringstream oss;
};

///////////////  1. Select Stock  ///////////////

/// 1.1 FAIL
TEST_F(TradingSystemFixture, Select_Stock_Fail) {
	EXPECT_CALL(stockBrokermock, selectStockBroker)
		.Times(1)
		.WillOnce(testing::Throw(std::invalid_argument("invalid ID")));

	EXPECT_THROW(
		{ stockBrokermock.selectStockBroker(INVALID); },
		invalid_argument);
}

/// 1.2 KIWER
TEST_F(TradingSystemFixture, Select_Kiwer_Success) {
	EXPECT_CALL(stockBrokermock, selectStockBroker)
		.Times(1);

	EXPECT_CALL(stockBrokermock, getID)
		.Times(1)
		.WillOnce(Return("KIWER"));

	stockBrokermock.selectStockBroker(KIWER);
	EXPECT_EQ(stockBrokermock.getID(), KIWER);
}

/// 1.3 NEMO
TEST_F(TradingSystemFixture, Select_Nemo_Success) {
	EXPECT_CALL(stockBrokermock, selectStockBroker)
		.Times(1);

	EXPECT_CALL(stockBrokermock, getID)
		.Times(1)
		.WillOnce(Return("NEMO"));
	stockBrokermock.selectStockBroker(NEMO);
	EXPECT_EQ(stockBrokermock.getID(), NEMO);
}



///////////////     2. Login     ///////////////

/// 2.1 KIWER
TEST_F(TradingSystemFixture, Login_Kiwer_Fail_Invalid_UserID) {
	EXPECT_THROW(
		{ getLoginMsg(false, KIWER, INVALID, PASSWORD); },
		std::invalid_argument
	);
}

TEST_F(TradingSystemFixture, Login_Kiwer_Fail_Invalid_Password) {
	EXPECT_THROW(
		{ getLoginMsg(false, KIWER, USER, INVALID); },
		std::invalid_argument
	);
}

TEST_F(TradingSystemFixture, Login_Kiwer_Success) {
	std::string loginMsg = getLoginMsg(true, KIWER, USER, PASSWORD);
	EXPECT_EQ(loginMsg, getKiwerLoginMsg(USER, PASSWORD));
}

/// 2.2 NEMO
TEST_F(TradingSystemFixture, Login_Nemo_Fail_Invalid_UserID) {
	EXPECT_THROW(
		{ getLoginMsg(false, NEMO, INVALID, PASSWORD); },
		std::invalid_argument
	);
}

TEST_F(TradingSystemFixture, Login_Nemo_Fail_Invalid_Password) {
	EXPECT_THROW(
		{ getLoginMsg(false, NEMO, USER, INVALID); },
		std::invalid_argument
	);
}

TEST_F(TradingSystemFixture, Login_Nemo_Success) {
	std::string loginMsg = getLoginMsg(true, NEMO, USER, PASSWORD);
	EXPECT_EQ(loginMsg, getNemoLoginMsg(USER, PASSWORD));
}


///////////////     3. Buying     ///////////////

/// 3.1 KIWER
TEST_F(TradingSystemFixture, Buying_Kiwer_Fail_Invalid_Code) {
	
	int stockCode = 0x1234;
	int price = 1000;
	int qty = 10l;
	EXPECT_CALL(stockmock, buy)
		.Times(1)
		.WillOnce(Return(false));
	
	EXPECT_EQ(stockmock.buy(ZERO, PRICE, QUANTITY), false);
}

TEST_F(TradingSystemFixture, Buying_Kiwer_Fail_Invalid_Price) {
	EXPECT_CALL(stockmock, buy)
		.Times(1)
		.WillOnce(Return(false));

	EXPECT_EQ(stockmock.buy(STOCK_CODE, ZERO, QUANTITY), false);
}

TEST_F(TradingSystemFixture, Buying_Kiwer_Fail_Invalid_Quantity) {
	EXPECT_CALL(stockmock, buy)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.selectStockBroker(KIWER);

	EXPECT_EQ(stockmock.buy(STOCK_CODE, PRICE, ZERO), false);
}

TEST_F(TradingSystemFixture, Buying_Kiwer_Success) {
	EXPECT_CALL(stockmock, buy)
		.Times(1)
		.WillOnce(Return(true));

	stockBrokermock.selectStockBroker(KIWER);

	EXPECT_EQ(stockmock.buy(STOCK_CODE, PRICE, QUANTITY), true);
}


/// 3.2 NEMO
TEST_F(TradingSystemFixture, Buying_Nemo_Fail_Invalid_Code) {
	EXPECT_CALL(stockmock, buy)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.selectStockBroker(NEMO);

	EXPECT_EQ(stockmock.buy(ZERO, PRICE, QUANTITY), false);
}

TEST_F(TradingSystemFixture, Buying_Nemo_Fail_Invalid_Price) {
	EXPECT_CALL(stockmock, buy)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.selectStockBroker(NEMO);

	EXPECT_EQ(stockmock.buy(STOCK_CODE, ZERO, QUANTITY), false);
}

TEST_F(TradingSystemFixture, Buying_Nemo_Fail_Invalid_Quantity) {
	EXPECT_CALL(stockmock, buy)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.selectStockBroker(NEMO);

	EXPECT_EQ(stockmock.buy(STOCK_CODE, PRICE, ZERO), false);
}

TEST_F(TradingSystemFixture, Buying_Nemo_Success) {
	EXPECT_CALL(stockmock, buy)
		.Times(1)
		.WillOnce(Return(true));

	stockBrokermock.selectStockBroker(NEMO);

	EXPECT_EQ(stockmock.buy(STOCK_CODE, PRICE, QUANTITY), true);
}



///////////////     4. Selling     ///////////////

/// 4.1 KIWER
TEST_F(TradingSystemFixture, Selling_Kiwer_Fail_Invalid_Code) {
	EXPECT_CALL(stockmock, sell)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.selectStockBroker(KIWER);

	EXPECT_EQ(stockmock.sell(INVALID_CODE, PRICE, QUANTITY), false);
}

TEST_F(TradingSystemFixture, Selling_Kiwer_Fail_Invalid_Price) {
	EXPECT_CALL(stockmock, sell)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.selectStockBroker(KIWER);

	EXPECT_EQ(stockmock.sell(STOCK_CODE, ZERO, QUANTITY), false);
}

TEST_F(TradingSystemFixture, Selling_Kiwer_Fail_Invalid_Quantity_Zero) {
	EXPECT_CALL(stockmock, sell)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.selectStockBroker(KIWER);

	EXPECT_EQ(stockmock.sell(STOCK_CODE, PRICE, ZERO), false);
}

TEST_F(TradingSystemFixture, Selling_Kiwer_Fail_Invalid_Quantity_Over) {
	EXPECT_CALL(stockmock, sell)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.selectStockBroker(KIWER);

	EXPECT_EQ(stockmock.sell(STOCK_CODE, PRICE, OVER), false);
}

TEST_F(TradingSystemFixture, Selling_Kiwer_Success) {
	EXPECT_CALL(stockmock, sell)
		.Times(1)
		.WillOnce(Return(true));

	stockBrokermock.selectStockBroker(KIWER);

	EXPECT_EQ(stockmock.sell(STOCK_CODE, PRICE, QUANTITY), true);
}


/// 4.2 NEMO
TEST_F(TradingSystemFixture, Selling_Nemo_Fail_Invalid_Code) {
	EXPECT_CALL(stockmock, sell)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.selectStockBroker(NEMO);

	EXPECT_EQ(stockmock.sell(INVALID_CODE, PRICE, QUANTITY), false);
}

TEST_F(TradingSystemFixture, Selling_Nemo_Fail_Invalid_Price) {
	EXPECT_CALL(stockmock, sell)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.selectStockBroker(NEMO);

	EXPECT_EQ(stockmock.sell(STOCK_CODE, ZERO, QUANTITY), false);
}

TEST_F(TradingSystemFixture, Selling_Nemo_Fail_Invalid_Quantity_Zero) {
	EXPECT_CALL(stockmock, sell)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.selectStockBroker(NEMO);

	EXPECT_EQ(stockmock.sell(STOCK_CODE, PRICE, ZERO), false);
}

TEST_F(TradingSystemFixture, Selling_Nemo_Fail_Invalid_Quantity_Over) {
	EXPECT_CALL(stockmock, sell)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.selectStockBroker(NEMO);

	EXPECT_EQ(stockmock.sell(STOCK_CODE, PRICE, OVER), false);
}

TEST_F(TradingSystemFixture, Selling_Nemo_Success) {
	EXPECT_CALL(stockmock, sell)
		.Times(1)
		.WillOnce(Return(true));

	stockBrokermock.selectStockBroker(NEMO);

	EXPECT_EQ(stockmock.sell(STOCK_CODE, PRICE, QUANTITY), true);
}


///////////////     5. Get Price     ///////////////

/// 5.1 KIWER
TEST_F(TradingSystemFixture, GetPrice_Kiwer_Fail_Invalid_Code) {
	EXPECT_CALL(stockmock, getPrice)
		.Times(1)
		.WillOnce(Return(ZERO));

	stockBrokermock.selectStockBroker(KIWER);

	EXPECT_EQ(stockmock.getPrice(INVALID_CODE), ZERO);
}

TEST_F(TradingSystemFixture, GetPrice_Kiwer_Success) {
	EXPECT_CALL(stockmock, getPrice)
		.Times(1)
		.WillOnce(Return(PRICE));

	stockBrokermock.selectStockBroker(KIWER);

	EXPECT_EQ(stockmock.getPrice(STOCK_CODE), PRICE);
}


/// 5.2 NEMO
TEST_F(TradingSystemFixture, GetPrice_Nemo_Fail_Invalid_Code) {
	EXPECT_CALL(stockmock, getPrice)
		.Times(1)
		.WillOnce(Return(ZERO));

	stockBrokermock.selectStockBroker(NEMO);

	EXPECT_EQ(stockmock.getPrice(INVALID_CODE), 0);
}

TEST_F(TradingSystemFixture, GetPrice_Nemo_Success) {
	EXPECT_CALL(stockmock, getPrice)
		.Times(1)
		.WillOnce(Return(PRICE));

	stockBrokermock.selectStockBroker(NEMO);

	EXPECT_EQ(stockmock.getPrice(STOCK_CODE), PRICE);
}