// IStockBroker.h
#pragma once
#include <string>
#define interface struct

//주석은 추후 없애도록 하겠습니다.

interface IStockBroker{ //Stocker Brocker? pdf에도 그렇게 나와있는데, 주식중개인의 뜻으로는 Stock Broker인듯 해서요. 일단 이 부분 수정했습니다.
public:
	virtual ~IStockBroker() = 0;
	virtual void login(const std::string &id, const std::string &pw) = 0;
	virtual void buy(const std::string &stockCode, int price, int qty) = 0;
	virtual void sell(const std::string &stockCode, int price, int qty) = 0;
	virtual int getPrice(const std::string &stockCode) = 0;
};
