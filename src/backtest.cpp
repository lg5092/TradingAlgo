#include "backtest.h"
#include "strategy.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>

void runBacktest(const std::string& csvFilePath, int shortPeriod, int longPeriod) {
    SMAStrategy strategy(shortPeriod, longPeriod);

    std::ifstream file(csvFilePath);
    if (!file) {
        std::cerr << "Failed to open CSV file: " << csvFilePath << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Skip header

    int shares = 0;
    double cash = 10000.0;
    int totalTrades = 0;
    int wins = 0;
    int losses = 0;
    double lastBuyPrice = 0.0;

    std::vector<double> tradePnLs;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string timestamp, priceStr;
        std::getline(ss, timestamp, ',');
        std::getline(ss, priceStr);

        double price = std::stod(priceStr);
        strategy.updatePrice(price);
        std::string action = strategy.evaluate();

        if (action == "buy" && cash >= price) {
            lastBuyPrice = price;
            cash -= price;
            shares += 1;
            totalTrades++;
            std::cout << "[BUY]  " << timestamp << " @ $" << price << " | Cash: $" << cash << " Shares: " << shares << std::endl;
        } 
        else if (action == "sell" && shares > 0) {
            cash += price;
            shares -= 1;
            totalTrades++;
            double pnl = price - lastBuyPrice;
            tradePnLs.push_back(pnl);
            if (pnl > 0) wins++;
            else losses++;
            std::cout << "[SELL] " << timestamp << " @ $" << price << " | Cash: $" << cash << " Shares: " << shares << std::endl;
        }
    }

    // If holding at end, calculate portfolio value based on last price
    double finalPrice = strategy.calculateSMA(shortPeriod); // or just use last read price
    double finalPortfolio = cash + shares * finalPrice;
    double netPL = finalPortfolio - 10000.0;

    std::cout << "\n--- Backtest Summary ---" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Final Portfolio Value: $" << finalPortfolio << std::endl;
    std::cout << "Net P&L: $" << netPL << (netPL >= 0 ? " ✅" : " ❌") << std::endl;
    std::cout << "Return: " << (netPL / 10000.0) * 100 << "%" << std::endl;
    std::cout << "Total Trades: " << totalTrades << std::endl;
    std::cout << "Winning Trades: " << wins << std::endl;
    std::cout << "Losing Trades: " << losses << std::endl;
    std::cout << "Win Rate: " << (wins + losses > 0 ? (wins * 100.0 / (wins + losses)) : 0.0) << "%" << std::endl;
}
