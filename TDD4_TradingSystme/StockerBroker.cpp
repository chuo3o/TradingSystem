#pragma once
#include <string>
#include <stdexcept>
#include <iostream>
#include "kiwer_api.cpp"
#include "nemo_api.cpp"

using std::string;

class IStockBroker
{
public:
    IStockBroker(string name) : name(name) {}
    virtual bool login(string id, string pw) = 0;
    virtual bool buy(int stockCode, int price, int qty) = 0;
    virtual bool sell(int stockCode, int price, int qty) = 0;
    virtual int getPrice(int stockCode) = 0;
    string getID() { return name; }
protected:
    string name;
};

class IManageStockBroker
{
public:
    const string KIWER = "KIWER";
    const string NEMO = "NEMO";
    const string MOCK = "MOCK";

    virtual IStockBroker* changeStockBroker(string name) = 0;
};

class MockDriver : public IStockBroker
{
public:
    MockDriver() : IStockBroker("MOCK") {};
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

class KiwerDriver : public IStockBroker
{
public:
    KiwerDriver() : IStockBroker("KIWER") {};
    bool login(string id, string pw)
    {
        api.login(id, pw);
        return true;
    }
    bool buy(int stockCode, int price, int qty)
    {
        string strCode = std::to_string(stockCode);
        api.buy(strCode, qty, price);
        return true;

    }
    bool sell(int stockCode, int price, int qty)
    {
        string strCode = std::to_string(stockCode);
        api.sell(strCode, qty, price);
        return true;

    }
    int getPrice(int stockCode)
    {
        string strCode = std::to_string(stockCode);
        return api.currentPrice(strCode);
    }

private:
    KiwerAPI api;
};

class NemoDriver : public IStockBroker
{
public:
    NemoDriver() : IStockBroker("NEMO") {};
    bool login(string id, string pw)
    {
        api.certification(id, pw);
        return true;

    }
    bool buy(int stockCode, int price, int qty)
    {
        string strCode = std::to_string(stockCode);
        api.purchasingStock(strCode, price, qty);
        return true;
    }
    bool sell(int stockCode, int price, int qty)
    {
        string strCode = std::to_string(stockCode);
        api.sellingStock(strCode, price, qty);
        return true;
    }
    int getPrice(int stockCode)
    {
        string strCode = std::to_string(stockCode);
        return api.getMarketPrice(strCode, DEFAULT_DELAY);
    }

private:
    const int DEFAULT_DELAY = 1;
    NemoAPI api;
};

class StockFactory
{
public:
    IStockBroker* GenStockDriver(string name)
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

class StockBrokerManager : public IManageStockBroker
{
public:
    StockBrokerManager() {}
    StockBrokerManager(IStockBroker* stock) : CurrentStock(stock) {}
    IStockBroker* changeStockBroker(string name) {
        StockFactory fotory;
        CurrentStock = fotory.GenStockDriver(name);
        return CurrentStock;
    }

private:
    IStockBroker* CurrentStock;

};


// app
class AutoTradingSystem
{
public:
    AutoTradingSystem(IManageStockBroker* bm) : BM(bm) {
    }
    void RunAutoTrading() { 

    }
    int buyNiceTiming(int stockCode, int price) {
    
        int Price[3];
        int retPrice = 0;
        int qty = 0;
        for (int i = 0; i < 3; i++)
        {
            Price[i] = ist->getPrice(stockCode);
        }

        if (Price[0] < Price[1] < Price[2])
        {
            qty = price / Price[2];
            if (qty != 0)
            {
                ist->buy(stockCode, price, qty);
            }
        }

        return qty;
    }
    int sellNiceTiming(int stockCode, int qty) {
    
    }
private:
    IManageStockBroker* BM;
    IStockBroker* ist;
};