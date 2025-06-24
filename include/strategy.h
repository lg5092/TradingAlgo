#ifndef STRATEGY_H
#define STRATEGY_H

#include <vector>
#include <string>

class SMAStrategy {
public:
    SMAStrategy(int shortPeriod, int longPeriod);
    void updatePrice(double price);
    std::string evaluate(); 
    double calculateSMA(int period);


private:
    int shortPeriod;
    int longPeriod;
    std::vector<double> priceHistory;

};

#endif
