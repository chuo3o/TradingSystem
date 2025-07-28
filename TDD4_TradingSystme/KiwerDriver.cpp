#include "StockerBrocker.cpp"
#include "kiwer_api.cpp"

class KiwerStockBroker : public IStockBroker
{
public:
    void selectStockBrocker(string id) {}
    bool login(string id, string pw) { return false; }

    bool buy(int stockCode, int price, int qty) {
        api.buy(std::to_string(stockCode), price, qty);
        return true;
    }

    bool sell(int stockCode, int price, int qty) { return false; }
    bool getPrice(int stockCode) { return false; }
    string getID() { return ""; }

private:
    KiwerAPI api;
};