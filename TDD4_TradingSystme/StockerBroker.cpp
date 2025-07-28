#pragma once
#include <string>
#include <stdexcept>

using std::string;

struct Driver {
  virtual int getPrice(int stockCode) = 0;
};

class IStockBroker
{
public:
    const string KIWER = "KIWER";
    const string NEMO = "NEMO";

    virtual void selectStockBroker(string id) = 0;

    virtual bool login(string id, string pw) = 0;
    virtual bool buy(int stockCode, int price, int qty) = 0;
    virtual bool sell(int stockCode, int price, int qty) = 0;
    virtual int getPrice(int stockCode) = 0;
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

    int getPrice(int stockCode) {
      return driver->getPrice(stockCode);
    }

    string getID() {
        return ID;
    }

    void setDriver(Driver* driver) {
      this->driver = driver;
    }

private:
    string ID;
    Driver* driver;
};
