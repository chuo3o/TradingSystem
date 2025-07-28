#include "StockBrokerManager.h"
#include "KiwerDriver.h"
#include "NemoDriver.h"
#include <stdexcept>

StockBrokerManager::StockBrokerManager(BrokerType brokerType, IStockBroker *broker):broker(broker) {}

void StockBrokerManager::login(const std::string &id, const std::string &pw) {
	if(!broker) throw std::runtime_error("Broker not selected");
	broker->login(id, pw);
}

