#include <iostream>
#include <stdexcept>
#include "gmock/gmock.h"
#include "StockerBroker.cpp"

using namespace testing;
using namespace std;
using std::string;

class StockMock : public IStockBroker {
public:
	StockMock() : IStockBroker("StockMock") {}
	MOCK_METHOD(bool, login, (string id, string pw), (override));
	MOCK_METHOD(bool, buy, (int stockCode, int price, int qty), (override));
	MOCK_METHOD(bool, sell, (int stockCode, int price, int qty), (override));
	MOCK_METHOD(int, getPrice, (int stockCode), (override));
	MOCK_METHOD(string, getID, (), ());

};

class StockBrokerMock : public IManageStockBroker {
public:
	MOCK_METHOD(IStockBroker*, changeStockBroker, (string id), (override));
};

class MockDriverMock : public MockDriver {
public:
	MOCK_METHOD(bool, login, (string id, string pw), ());
	MOCK_METHOD(bool, buy, (int stockCode, int price, int qty), ());
	MOCK_METHOD(bool, sell, (int stockCode, int price, int qty), ());
	MOCK_METHOD(int, getPrice, (int stockCode), ());

};

class AutoTradingSystemMock : public AutoTradingSystem {
public:
};


class TradingSystemFixture : public Test {
public:
	const string INVALID = "INVALID";
	const string KIWER = "KIWER";
	const string NEMO = "NEMO";
	const string MOCK = "MOCK";
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
	NiceMock<MockDriverMock> mockDriverMock;

	string getKiwerLoginMsg(const string& id, const string& pw) const {
		if (id == INVALID) {
			throw std::invalid_argument("[KIWER] Wrong ID");
		}
		else if (pw == INVALID) {
			throw std::invalid_argument("[KIWER] Wrong PW");
		}
		return id + " login success\n";
	}
	string getNemoLoginMsg(const string& id, const string& pw) const {
		if (id == INVALID) {
			throw std::invalid_argument("[NEMO] Wrong ID");
		}
		else if (pw == INVALID) {
			throw std::invalid_argument("[NEMO] Wrong PW");
		}
		return "[NEMO]" + id + " login GOOD\n";
	}

	string getLoginMsg(
		bool isSuccess, const string& brokerType,
		const string& id, const string& pw) {

		oss.str(""); oss.clear();
		auto oldCoutStreamBuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

		//EXPECT_CALL(stockmock, selectStockBroker)
		//	.WillOnce(Return(true));

		//stockmock.selectStockBroker(brokerType);
		EXPECT_CALL(stockmock, login)
			.WillOnce(
				[&]() {
					if (brokerType == KIWER) {
						std::cout << getKiwerLoginMsg(id, pw);
					}
					else if (brokerType == NEMO) {
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


///////////////  -1. AutoTrandingSystaem ///////////////
/// -1.1 BuyNiceTime fail
TEST_F(TradingSystemFixture, AutoTrandingSystaemMock_TC1)
{
	// conection mockdriver mock
	EXPECT_CALL(stockBrokermock, changeStockBroker(MOCK))
		.Times(1)
		.WillOnce(Return(&mockDriverMock));

	EXPECT_CALL(mockDriverMock, getPrice(STOCK_CODE))
		.Times(3)
		.WillOnce(Return(1234))
		.WillOnce(Return(1234))
		.WillOnce(Return(1234));

	EXPECT_CALL(mockDriverMock, buy)
		.Times(0);
	//AutoTradingSystem ats{ "MOCK" };

	//int expectedRet = 0;
	//int actualRet = ats.buyNiceTiming(STOCK_CODE, PRICE);
	//EXPECT_EQ(expectedRet, actualRet);

}

/// -1.1 BuyNiceTime success
TEST_F(TradingSystemFixture, AutoTrandingSystaemMock_TC2)
{
	// conection mockdriver mock
	EXPECT_CALL(stockBrokermock, changeStockBroker(MOCK))
		.Times(1)
		.WillOnce(Return(&mockDriverMock));

	EXPECT_CALL(mockDriverMock, getPrice(STOCK_CODE))
		.Times(3)
		.WillOnce(Return(1234))
		.WillOnce(Return(2345))
		.WillOnce(Return(9999));

	EXPECT_CALL(mockDriverMock, buy)
		.Times(1);

	//int expectedRet = (PRICE / 9999);
	//int actualRet = autoTradingSystem.buyNiceTiming(STOCK_CODE, PRICE);
	//EXPECT_EQ(expectedRet, actualRet);
}

///////////////  0. MockDriver test ///////////////
TEST_F(TradingSystemFixture, MockDriverMockTC1) 
{
	bool ret;
	StockBrokerManager bm;
	IStockBroker* ist = bm.changeStockBroker(MOCK);
	ret = ist->login("myID", "1234");
	EXPECT_EQ(ret, true);
}

///////////////  1. Select Stock  ///////////////
/// 1.1 FAIL
TEST_F(TradingSystemFixture, Select_Stock_Fail) {
	EXPECT_CALL(stockBrokermock, changeStockBroker)
		.Times(1)
		.WillOnce(testing::Throw(std::invalid_argument("invalid ID")));

	EXPECT_THROW(
		{ stockBrokermock.changeStockBroker(INVALID); },
		invalid_argument);
}

/// 1.2 KIWER
TEST_F(TradingSystemFixture, Select_Kiwer_Success) {
	EXPECT_CALL(stockmock, getID)
		.Times(1)
		.WillOnce(Return("KIWER"));

	stockBrokermock.changeStockBroker(KIWER);
	EXPECT_EQ(stockmock.getID(), KIWER);
}

/// 1.3 NEMO
TEST_F(TradingSystemFixture, Select_Nemo_Success) {
	EXPECT_CALL(stockmock, getID)
		.Times(1)
		.WillOnce(Return("NEMO"));
	stockBrokermock.changeStockBroker(NEMO);
	EXPECT_EQ(stockmock.getID(), NEMO);
}

///////////////  1-1. Select Stock (Use MockDriver)  ///////////////
/// 1.1.1 FAIL
TEST_F(TradingSystemFixture, MockDriver_Select_Stock_Fail) {

	bool ret;
	StockBrokerManager bm;
	EXPECT_THROW(
		{ bm.changeStockBroker(INVALID); },
		invalid_argument);
}

/// 1.1.2 KIWER
TEST_F(TradingSystemFixture, MockDriver_Select_Kiwer_Success) {
	bool ret;
	StockBrokerManager bm;
	IStockBroker* ist = bm.changeStockBroker(KIWER);
	string name = ist->getID();
	EXPECT_EQ(name, KIWER);
}

/// 1.1.3 NEMO
TEST_F(TradingSystemFixture, MockDriver_Select_Nemo_Success) {
	bool ret;
	StockBrokerManager bm;
	IStockBroker* ist = bm.changeStockBroker(NEMO);
	string name = ist->getID();
	EXPECT_EQ(name, NEMO);
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
	string loginMsg = getLoginMsg(true, KIWER, USER, PASSWORD);
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
	string loginMsg = getLoginMsg(true, NEMO, USER, PASSWORD);
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

	stockBrokermock.changeStockBroker(KIWER);

	EXPECT_EQ(stockmock.buy(STOCK_CODE, PRICE, ZERO), false);
}

TEST_F(TradingSystemFixture, Buying_Kiwer_Success) {
	EXPECT_CALL(stockmock, buy)
		.Times(1)
		.WillOnce(Return(true));

	stockBrokermock.changeStockBroker(KIWER);

	EXPECT_EQ(stockmock.buy(STOCK_CODE, PRICE, QUANTITY), true);
}


/// 3.2 NEMO
TEST_F(TradingSystemFixture, Buying_Nemo_Fail_Invalid_Code) {
	EXPECT_CALL(stockmock, buy)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.changeStockBroker(NEMO);

	EXPECT_EQ(stockmock.buy(ZERO, PRICE, QUANTITY), false);
}

TEST_F(TradingSystemFixture, Buying_Nemo_Fail_Invalid_Price) {
	EXPECT_CALL(stockmock, buy)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.changeStockBroker(NEMO);

	EXPECT_EQ(stockmock.buy(STOCK_CODE, ZERO, QUANTITY), false);
}

TEST_F(TradingSystemFixture, Buying_Nemo_Fail_Invalid_Quantity) {
	EXPECT_CALL(stockmock, buy)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.changeStockBroker(NEMO);

	EXPECT_EQ(stockmock.buy(STOCK_CODE, PRICE, ZERO), false);
}

TEST_F(TradingSystemFixture, Buying_Nemo_Success) {
	EXPECT_CALL(stockmock, buy)
		.Times(1)
		.WillOnce(Return(true));

	stockBrokermock.changeStockBroker(NEMO);

	EXPECT_EQ(stockmock.buy(STOCK_CODE, PRICE, QUANTITY), true);
}



///////////////     4. Selling     ///////////////

/// 4.1 KIWER
TEST_F(TradingSystemFixture, Selling_Kiwer_Fail_Invalid_Code) {
	EXPECT_CALL(stockmock, sell)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.changeStockBroker(KIWER);

	EXPECT_EQ(stockmock.sell(INVALID_CODE, PRICE, QUANTITY), false);
}

TEST_F(TradingSystemFixture, Selling_Kiwer_Fail_Invalid_Price) {
	EXPECT_CALL(stockmock, sell)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.changeStockBroker(KIWER);

	EXPECT_EQ(stockmock.sell(STOCK_CODE, ZERO, QUANTITY), false);
}

TEST_F(TradingSystemFixture, Selling_Kiwer_Fail_Invalid_Quantity_Zero) {
	EXPECT_CALL(stockmock, sell)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.changeStockBroker(KIWER);

	EXPECT_EQ(stockmock.sell(STOCK_CODE, PRICE, ZERO), false);
}

TEST_F(TradingSystemFixture, Selling_Kiwer_Fail_Invalid_Quantity_Over) {
	EXPECT_CALL(stockmock, sell)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.changeStockBroker(KIWER);

	EXPECT_EQ(stockmock.sell(STOCK_CODE, PRICE, OVER), false);
}

TEST_F(TradingSystemFixture, Selling_Kiwer_Success) {
	EXPECT_CALL(stockmock, sell)
		.Times(1)
		.WillOnce(Return(true));

	stockBrokermock.changeStockBroker(KIWER);

	EXPECT_EQ(stockmock.sell(STOCK_CODE, PRICE, QUANTITY), true);
}


/// 4.2 NEMO
TEST_F(TradingSystemFixture, Selling_Nemo_Fail_Invalid_Code) {
	EXPECT_CALL(stockmock, sell)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.changeStockBroker(NEMO);

	EXPECT_EQ(stockmock.sell(INVALID_CODE, PRICE, QUANTITY), false);
}

TEST_F(TradingSystemFixture, Selling_Nemo_Fail_Invalid_Price) {
	EXPECT_CALL(stockmock, sell)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.changeStockBroker(NEMO);

	EXPECT_EQ(stockmock.sell(STOCK_CODE, ZERO, QUANTITY), false);
}

TEST_F(TradingSystemFixture, Selling_Nemo_Fail_Invalid_Quantity_Zero) {
	EXPECT_CALL(stockmock, sell)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.changeStockBroker(NEMO);

	EXPECT_EQ(stockmock.sell(STOCK_CODE, PRICE, ZERO), false);
}

TEST_F(TradingSystemFixture, Selling_Nemo_Fail_Invalid_Quantity_Over) {
	EXPECT_CALL(stockmock, sell)
		.Times(1)
		.WillOnce(Return(false));

	stockBrokermock.changeStockBroker(NEMO);

	EXPECT_EQ(stockmock.sell(STOCK_CODE, PRICE, OVER), false);
}

TEST_F(TradingSystemFixture, Selling_Nemo_Success) {
	EXPECT_CALL(stockmock, sell)
		.Times(1)
		.WillOnce(Return(true));

	stockBrokermock.changeStockBroker(NEMO);

	EXPECT_EQ(stockmock.sell(STOCK_CODE, PRICE, QUANTITY), true);
}


///////////////     5. Get Price     ///////////////

/// 5.1 KIWER
TEST_F(TradingSystemFixture, GetPrice_Kiwer_Fail_Invalid_Code) {
	EXPECT_CALL(stockmock, getPrice)
		.Times(1)
		.WillOnce(Return(ZERO));

	stockBrokermock.changeStockBroker(KIWER);

	EXPECT_EQ(stockmock.getPrice(INVALID_CODE), ZERO);
}

TEST_F(TradingSystemFixture, GetPrice_Kiwer_Success) {
	EXPECT_CALL(stockmock, getPrice)
		.Times(1)
		.WillOnce(Return(PRICE));

	stockBrokermock.changeStockBroker(KIWER);

	EXPECT_EQ(stockmock.getPrice(STOCK_CODE), PRICE);
}


/// 5.2 NEMO
TEST_F(TradingSystemFixture, GetPrice_Nemo_Fail_Invalid_Code) {
	EXPECT_CALL(stockmock, getPrice)
		.Times(1)
		.WillOnce(Return(ZERO));

	stockBrokermock.changeStockBroker(NEMO);

	EXPECT_EQ(stockmock.getPrice(INVALID_CODE), 0);
}

TEST_F(TradingSystemFixture, GetPrice_Nemo_Success) {
	EXPECT_CALL(stockmock, getPrice)
		.Times(1)
		.WillOnce(Return(PRICE));

	stockBrokermock.changeStockBroker(NEMO);

	EXPECT_EQ(stockmock.getPrice(STOCK_CODE), PRICE);
}