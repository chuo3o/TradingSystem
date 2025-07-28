#pragma once
#pragma once
#include "IStockBroker.h"
#include <memory>

enum class BrokerType {
	KIWER,
	NEMO
};

class StockBrokerManager {
private:
	IStockBroker* broker = nullptr;

public:
	StockBrokerManager(BrokerType borkerType, IStockBroker *broker);
	void login(const std::string &id, const std::string &pw);
	void buy(const std::string &stockCode, int count, int price);
	void sell(const std::string &stockCode, int count, int price);
	int currentPrice(const std::string &stockCode);
};

