#include "strategy.h"
#include <numeric> // for std::accumulate

SMAStrategy::SMAStrategy(int shortP, int longP)
    : shortPeriod(shortP), longPeriod(longP) {}

// Add the latest price and trim history if too long
void SMAStrategy::updatePrice(double price) {
    priceHistory.push_back(price);
    if (priceHistory.size() > longPeriod) {
        priceHistory.erase(priceHistory.begin());
    }
}

// Calculate the average of the last N prices
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
        return "hold"; // Not enough data yet
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
