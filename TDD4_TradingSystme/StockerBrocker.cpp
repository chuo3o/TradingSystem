#pragma once
#include <iostream>
#include <string>
#include <stdexcept>

using std::string;

class IStockBroker
{
public:
    const string KIWER = "KIWER";
    const string NEMO = "NEMO";

    virtual void selectStockBrocker(string id) = 0;
    virtual bool login(string id, string pw) = 0;
    virtual bool buy(int stockCode, int price, int qty) = 0;
    virtual bool sell(int stockCode, int price, int qty) = 0;
    virtual bool getPrice(int stockCode) = 0;
    virtual string getID() = 0;
};

class BrokerManager : public IStockBroker
{

public:
    void selectStockBrocker(string id) { 

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
    bool sell(int stockCode, int price, int qty) {
        if (stockCode == 0 || price <= 0 || qty <= 0) return false;

        if (ID == KIWER) {
            std::cout << stockCode << " : Sell stock ( " << price << " * " << qty << ")\n";
        }
        else if (ID == NEMO) {
            std::cout << "[NEMO]" << stockCode << " sell stock ( price : " << price << " ) * ( count : " << qty << ")\n";
        }
        return true;
    }
    bool getPrice(int stockCode) { return false; }
    string getID() {
        return ID;
    }

private:
    string ID;

};
