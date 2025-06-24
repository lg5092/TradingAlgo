#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include "json.hpp"
#include "api.h"
#include "strategy.h"
#include "backtest.h"

int main() {
    std::ifstream file("config.json");
    if (!file) {
        std::cerr << "Error: config.json not found" << std::endl;
        return 1;
    }

    nlohmann::json config;
    file >> config;

    std::string apiKey = config["API_KEY"];
    std::string secretKey = config["SECRET_KEY"];
    std::string symbol = "AAPL";
    std::string mode;
    std::cout << "Enter mode (backtest/live): ";
    std::cin >> mode;

    if (mode == "backtest") {
        std::cout << "\nDownloading data from Alpaca...\n";

        // Date range
        std::string start = "2024-03-01T09:30:00Z";
        std::string end   = "2024-03-01T16:00:00Z";
        std::string outputFile = "data/AAPL_bars.csv";
        downloadHistoricalBars(apiKey, secretKey, symbol, start, end, outputFile);

        std::cout << "\n Running backtest...\n";
        runBacktest(outputFile, 5, 10);
    } 
    else if (mode == "live") {
        std::cout << "\n Starting live trading bot for: " << symbol << "\n";
        SMAStrategy strategy(5, 10);

        while (true) {
            double price = getLatestPrice(apiKey, secretKey, symbol);
            if (price <= 0) {
                std::cerr << "Error due to bad price.\n";
            } else {
                std::cout << "Price: $" << price << std::endl;
                strategy.updatePrice(price);
                std::string action = strategy.evaluate();

                if (action == "buy" || action == "sell") {
                    std::cout << "Action: " << action << " 1 share of " << symbol << std::endl;
                    placeOrder(apiKey, secretKey, symbol, 1, action);
                } else {
                    std::cout << "Action: hold\n";
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    } 
    else {
        std::cerr << "Invalid mode selected.\n";
        return 1;
    }

    return 0;
}
