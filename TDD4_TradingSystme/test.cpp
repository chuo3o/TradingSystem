#include <iostream>
#include <string>
#include <stdexcept>
#include "gmock/gmock.h"
#include "kiwer_api.cpp"
#include "nemo_api.cpp"
#include "IStockBroker.h"
#include "StockBrokerManager.h"

using namespace testing;
//using namespace std;
using std::string;

class StockBrokerMock: public IStockBroker {
public:
	MOCK_METHOD(void, login, (const std::string &id, const std::string &pw), (override));
	MOCK_METHOD(void, buy, (const std::string &stockCode, int price, int qty), (override));
	MOCK_METHOD(void, sell, (const std::string &stockCode, int price, int qty), (override));
	MOCK_METHOD(int, getPrice, (const std::string &stockCode), (override));
};

class TradingSystemFixture: public Test {
public:
	std::ostringstream oss;

	std::string login(const std::string &whichTradingSystem, bool isSuccess, const std::string &id, const std::string &pw) {
		oss.str(""); oss.clear();

		auto oldCoutStreamBuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

		StockBrokerMock stockBrokerMock;
		StockBrokerManager *stockBrokerManager;
		if(whichTradingSystem == "KiwerAPI") {
			stockBrokerManager = new StockBrokerManager(BrokerType::KIWER, &stockBrokerMock);
		}
		else if(whichTradingSystem == "NemoAPI") {
			stockBrokerManager = new StockBrokerManager(BrokerType::NEMO, &stockBrokerMock);
		}

		EXPECT_CALL(stockBrokerMock, login)
			.WillOnce(
				[&]() {
					if(isSuccess) {
						std::cout << id << " login success\n";
					}
				}
			);

		stockBrokerManager->login(id, pw);



		std::cout.rdbuf(oldCoutStreamBuf);
		return oss.str();
	}

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
	//const std::string id = "dteam";
	//const std::string pw = "passwordyo~";
	//std::string expected = id + " login success\n";
	//std::string actual = login("KiwerAPI", id, pw);
	//EXPECT_EQ(expected, actual);
}

TEST_F(TradingSystemFixture, Login_Nemo_Fail) {

	// EXPECT_THROW( login(), runtime_error);
}

TEST_F(TradingSystemFixture, Login_Nemo_Success) {

	// EXPECT_EQ(login(), "Nemo Login Success");
}
