#include <iostream>
#include <stdexcept>
#include "gmock/gmock.h"
#include "StockerBroker.cpp"

using namespace testing;
using namespace std;

class StockMock : public IStockBroker {
public:
	MOCK_METHOD(void, selectStockBroker, (string id), (override));
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


TEST_F(TradingSystemFixture, Select_Stock_Fail) {

	EXPECT_CALL(stockmock, selectStockBroker)
		.Times(1)
		.WillOnce(testing::Throw(std::invalid_argument("invalid ID")));

	EXPECT_THROW(
		{ stockmock.selectStockBroker(INVALID); },
		invalid_argument);
}

TEST_F(TradingSystemFixture, Select_Kiwer_Success) {
	EXPECT_CALL(stockmock, selectStockBroker)
		.Times(1);

	EXPECT_CALL(stockmock, getID)
		.Times(1)
		.WillOnce(Return("KIWER"));

	stockmock.selectStockBroker(KIWER);
	EXPECT_EQ(stockmock.getID(), KIWER);
}

TEST_F(TradingSystemFixture, Select_Nemo_Success) {
	EXPECT_CALL(stockmock, selectStockBroker)
		.Times(1);

	EXPECT_CALL(stockmock, getID)
		.Times(1)
		.WillOnce(Return("NEMO"));
	stockmock.selectStockBroker(NEMO);
	EXPECT_EQ(stockmock.getID(), NEMO);

}



///////////////     2. Login     ///////////////
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
