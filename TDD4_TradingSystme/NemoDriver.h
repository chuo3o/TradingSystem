#pragma once
#include "IStockBroker.h"
#include <iostream>

class NemoDriver: public IStockBroker {
public:
	NemoDriver() = default;
	~NemoDriver() override = default;
	void login(const std::string &id, const std::string &pw) override;
	void buy(const std::string& stockCode, int price, int count) override;
	void sell(const std::string& stockCode, int price, int count) override;
	int getPrice(const std::string& stockCode) override;
};
