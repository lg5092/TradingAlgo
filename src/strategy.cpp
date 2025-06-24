#include "strategy.h"
#include <numeric> 

SMAStrategy::SMAStrategy(int shortP, int longP)
    : shortPeriod(shortP), longPeriod(longP) {}

void SMAStrategy::updatePrice(double price) {
    priceHistory.push_back(price);
    if (priceHistory.size() > longPeriod) {
        priceHistory.erase(priceHistory.begin());
    }
}

double SMAStrategy::calculateSMA(int period) {
    if (priceHistory.size() < period) {
        return 0.0;
    }

    double sum = std::accumulate(
        priceHistory.end() - period,
        priceHistory.end(),
        0.0
    );
    return sum / period;
}

// Determine whether to buy, sell, or hold
std::string SMAStrategy::evaluate() {
    if (priceHistory.size() < longPeriod) {
        return "hold"; 
    }

    double shortSMA = calculateSMA(shortPeriod);
    double longSMA = calculateSMA(longPeriod);

    static std::string lastAction = "hold";

    if (shortSMA > longSMA && lastAction != "buy") {
        lastAction = "buy";
        return "buy";
    } else if (shortSMA < longSMA && lastAction != "sell") {
        lastAction = "sell";
        return "sell";
    }

    return "hold";
}
