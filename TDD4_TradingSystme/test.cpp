#include <iostream>
#include <string>
#include "gmock/gmock.h"
#include "kiwer_api.cpp"
#include "nemo_api.cpp"

using namespace testing;

class KiwerAPIMock: public KiwerAPI {
public:
	MOCK_METHOD(void, login_fail, (std::string ID, std::string password), ());
	MOCK_METHOD(void, login_succ, (std::string ID, std::string password), ());
};

class TradingSystemFixture : public Test {
public:
	std::ostringstream oss;

	std::string login(const std::string & whichTradingSystem, const std::string & id, const std::string & pw) {
		oss.str(""); oss.clear();

		auto oldCoutStreamBuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

		if(whichTradingSystem == "KiwerAPI") {
			KiwerAPIMock kiwerAPIMock;
			EXPECT_CALL(kiwerAPIMock, login_succ)
				.WillOnce([&]() { std::cout << id << " login success\n"; });

			//kiwerAPIMock->login_succ(id, pw);
			//KiwerAPI *kiwerAPI = new KiwerAPI();
			//kiwerAPI->login(id, pw);
			kiwerAPIMock.login_succ(id, pw);
		}
		else if(whichTradingSystem == "NemoAPI") {
			NemoAPI *nemoAPI = new NemoAPI();
			nemoAPI->certification(id, pw);
		}

		std::cout.rdbuf(oldCoutStreamBuf);
		return oss.str();
	}

};

/* Select Stock */
TEST_F(TradingSystemFixture, SelectWrongStock) {

}

TEST_F(TradingSystemFixture, SelectKiwerSuccess) {

}

TEST_F(TradingSystemFixture, SelectNemoSuccess) {

}

/* Login */
TEST_F(TradingSystemFixture, Login_Kiwer_Fail) {

}

TEST_F(TradingSystemFixture, Login_Kiwer_Success) {
	const std::string id = "dteam";
	const std::string pw = "passwordyo~";
	std::string expected = id + " login success\n";
	std::string actual = login("KiwerAPI", id, pw);
	EXPECT_EQ(expected, actual);
}

TEST_F(TradingSystemFixture, Login_Nemo_Fail) {

}

TEST_F(TradingSystemFixture, Login_Nemo_Success) {

}