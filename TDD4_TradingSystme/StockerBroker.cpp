#pragma once
#include <string>
#include <stdexcept>
#include <iostream>
#include "kiwer_api.cpp"
#include "nemo_api.cpp"

using std::string;

class IStock
{
public:
    virtual bool login(string id, string pw) = 0;
    virtual bool buy(int stockCode, int price, int qty) = 0;
    virtual bool sell(int stockCode, int price, int qty) = 0;
    virtual int getPrice(int stockCode) = 0;
};

class IStockBroker
{
public:
    const string KIWER = "KIWER";
    const string NEMO = "NEMO";
    const string MOCK = "MOCK";

    virtual void selectStockBroker(string name) = 0;
    virtual IStock* getStockBroker() = 0;
    virtual string getID() = 0;
};

class MockDriver : public IStock
{
public:
    bool login(string id, string pw)
    {
        std::cout << "[MockStock] -" << id << " login GOOD\n";
        return true;
    }
    bool buy(int stockCode, int price, int qty)
    {
        std::cout << "[MockStock] -" << stockCode << " : Buy stock ( " << price << " * " << qty << ")\n";
        return true;
    }
    bool sell(int stockCode, int price, int qty)
    {
        std::cout << "[MockStock] -" << stockCode << " : Sell stock ( " << price << " * " << qty << ")\n";
        return true;
    }
    int getPrice(int stockCode)
    {
        std::srand(std::time(NULL));
        int ret = std::rand() % 10 * 100 + 5000;
        return ret;
    }
};

class KiwerDriver : public IStock
{
public:
    bool login(string id, string pw)
    {
        api.login(id, pw);
    }
    bool buy(int stockCode, int price, int qty)
    {
        string strCode = std::to_string(stockCode);
        api.buy(strCode, qty, price);
    }
    bool sell(int stockCode, int price, int qty)
    {
        string strCode = std::to_string(stockCode);
        api.sell(strCode, qty, price);
    }
    int getPrice(int stockCode)
    {
        string strCode = std::to_string(stockCode);
        api.currentPrice(strCode);
    }

private:
    KiwerAPI api;
};

class NemoDriver : public IStock
{
public:
    bool login(string id, string pw)
    {
        api.certification(id, pw);
    }
    bool buy(int stockCode, int price, int qty)
    {
        string strCode = std::to_string(stockCode);
        api.purchasingStock(strCode, price, qty);
    }
    bool sell(int stockCode, int price, int qty)
    {
        string strCode = std::to_string(stockCode);
        api.sellingStock(strCode, price, qty);
    }
    int getPrice(int stockCode)
    {
        string strCode = std::to_string(stockCode);
        api.getMarketPrice(strCode, DEFAULT_DELAY);
    }

private:
    const int DEFAULT_DELAY = 1;
    NemoAPI api;
};

class StockFactory
{
public:
    IStock* GenStockDriver(string name)
    {
        if (name == "KIWER")
        {
            return new KiwerDriver();
        }
        else if (name == "NEMO")
        {
            return new NemoDriver();
        }
        else if (name == "MOCK")
        {
            return new MockDriver();
        }

        throw std::invalid_argument("invalid stock driver name");
    }
};

class BrokerManager : public IStockBroker
{
public:
    void selectStockBroker(string name) {
        StockFactory fotory;
        CurrentStock = fotory.GenStockDriver(name);
    }

    IStock* getStockBroker() { return CurrentStock; }

    string getID() {
        return ID;
    }

private:
    string ID;
    IStock* CurrentStock;

};
