#ifndef STRATEGY_H
#define STRATEGY_H

#include <vector>
#include <string>

class SMAStrategy {
public:
    SMAStrategy(int shortPeriod, int longPeriod);
    void updatePrice(double price);
    std::string evaluate(); // Returns "buy", "sell", or "hold"
    double calculateSMA(int period);


private:
    int shortPeriod;
    int longPeriod;
    std::vector<double> priceHistory;

};

#endif
