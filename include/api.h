#ifndef API_H
#define API_H

#include <string>

void getAccountInfo(const std::string& apiKey, const std::string& secretKey);
void placeOrder(const std::string& apiKey, const std::string& secretKey,
                const std::string& symbol, int qty, const std::string& side);
double getLatestPrice(const std::string& apiKey, const std::string& secretKey, const std::string& symbol);
void downloadHistoricalBars(const std::string& apiKey, const std::string& secretKey,
                             const std::string& symbol,
                             const std::string& startISO, const std::string& endISO,
                             const std::string& outputCSVPath);


#endif
