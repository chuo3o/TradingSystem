#pragma once
#include <string>

using std::string;

class IStockBroker
{
public:
    virtual bool selectStockBrocker(string id) = 0;
    virtual bool login(string id, string pw) = 0;
    virtual bool buy(int stockCode, int price, int qty) = 0;
    virtual bool sell(int stockCode, int price, int qty) = 0;
    virtual bool getPrice(int stockCode) = 0;
};

class BrokerManager : public IStockBroker
{

public:
    bool selectStockBrocker(string id) { return false; }
    bool login(string id, string pw) { return false; }
    bool buy(int stockCode, int price, int qty) { return false; }
    bool sell(int stockCode, int price, int qty) { return false; }
    bool getPrice(int stockCode) { return false; }
};
