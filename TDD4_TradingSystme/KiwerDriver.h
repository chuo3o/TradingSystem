#pragma once
#include "IStockBroker.h"
#include <iostream>

class KiwerDriver: public IStockBroker {
public:
	~KiwerDriver() override = default;
	void login(const std::string &id, const std::string &pw) override;
	void buy(const std::string &stockCode, int price, int count) override;
	void sell(const std::string &stockCode, int price, int count) override;
	int getPrice(const std::string &stockCode) override;
};

