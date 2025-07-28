#pragma once
#include <string>

using std::string;

class IStocker
{
public:
    virtual bool login(string id, string pw) = 0;
    virtual bool buy(int stockCode, int price, int qty) = 0;
    virtual bool sell(int stockCode, int price, int qty) = 0;
    virtual bool getPrice(int stockCode) = 0;
};

class IBrocker
{
public:
    virtual IStocker* selectStockBrocker(int a) = 0;
};


class StockerBrocker : public IStocker, public IBrocker
{

public:
    virtual IStocker *selectStockBrocker(int a)=0;
    virtual bool login(string id, string pw)=0;
    virtual bool buy(int stockCode, int price, int qty)=0;
    virtual bool sell(int stockCode, int price, int qty)=0;
    virtual bool getPrice(int stockCode)=0;
};