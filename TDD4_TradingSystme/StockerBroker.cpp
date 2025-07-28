#pragma once
#include <string>
#include <stdexcept>

using std::string;

class IStockBroker
{
public:
    const string KIWER = "KIWER";
    const string NEMO = "NEMO";

    virtual void selectStockBroker(string id) = 0;

    virtual bool login(string id, string pw) = 0;
    virtual bool buy(int stockCode, int price, int qty) = 0;
    virtual bool sell(int stockCode, int price, int qty) = 0;
    virtual bool getPrice(int stockCode) = 0;
    virtual string getID() = 0;
};

class BrokerManager : public IStockBroker
{
public:
    void selectStockBroker(string id) { 

        if (id == KIWER)
        {
            ID = KIWER;
        }
        else if (id == NEMO)
        {
            ID = NEMO;
        }
        else
        {
            throw std::invalid_argument("invalid ID");
        }
    }

    bool login(string id, string pw) { return false; }
    bool buy(int stockCode, int price, int qty) { return false; }
    bool sell(int stockCode, int price, int qty) { return false; }
    bool getPrice(int stockCode) { return false; }
    string getID() {
        return ID;
    }

private:
    string ID;

};
