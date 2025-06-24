#include "api.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <curl/curl.h>
#include "json.hpp"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void getAccountInfo(const std::string& apiKey, const std::string& secretKey) {
    CURL* curl = curl_easy_init();
    if (curl) {
        std::string readBuffer;
        struct curl_slist* headers = NULL;

        headers = curl_slist_append(headers, ("APCA-API-KEY-ID: " + apiKey).c_str());
        headers = curl_slist_append(headers, ("APCA-API-SECRET-KEY: " + secretKey).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, "https://paper-api.alpaca.markets/v2/account");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        else
            std::cout << "Account Info: " << readBuffer << std::endl;

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

void placeOrder(const std::string& apiKey, const std::string& secretKey,
                const std::string& symbol, int qty, const std::string& side) {
    CURL* curl = curl_easy_init();
    if (curl) {
        std::string readBuffer;
        struct curl_slist* headers = NULL;

        headers = curl_slist_append(headers, ("APCA-API-KEY-ID: " + apiKey).c_str());
        headers = curl_slist_append(headers, ("APCA-API-SECRET-KEY: " + secretKey).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        std::string url = "https://paper-api.alpaca.markets/v2/orders";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        std::ostringstream body;
        body << "{"
             << "\"symbol\":\"" << symbol << "\","
             << "\"qty\":" << qty << ","
             << "\"side\":\"" << side << "\","
             << "\"type\":\"market\","
             << "\"time_in_force\":\"gtc\""
             << "}";

        std::string bodyStr = body.str();
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, bodyStr.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "Order failed: " << curl_easy_strerror(res) << std::endl;
        else
            std::cout << "Order Response: " << readBuffer << std::endl;

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

double getLatestPrice(const std::string& apiKey, const std::string& secretKey, const std::string& symbol) {
    CURL* curl = curl_easy_init();
    if (!curl) return -1.0;

    std::string readBuffer;
    std::string url = "https://data.alpaca.markets/v2/stocks/" + symbol + "/trades/latest";

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, ("APCA-API-KEY-ID: " + apiKey).c_str());
    headers = curl_slist_append(headers, ("APCA-API-SECRET-KEY: " + secretKey).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "Price fetch failed: " << curl_easy_strerror(res) << std::endl;
        return -1.0;
    }

    try {
        nlohmann::json json = nlohmann::json::parse(readBuffer);
        double price = json["trade"]["p"];  // "ap" = ask price
        return price;
    } catch (...) {
        std::cerr << "Error parsing price JSON: " << readBuffer << std::endl;
        return -1.0;
    }
}

void downloadHistoricalBars(const std::string& apiKey, const std::string& secretKey,
                             const std::string& symbol,
                             const std::string& startISO, const std::string& endISO,
                             const std::string& outputCSVPath) {
    CURL* curl = curl_easy_init();
    if (!curl) return;

    std::string readBuffer;
    std::string url = "https://data.alpaca.markets/v2/stocks/" + symbol +
                      "/bars?start=" + startISO +
                      "&end=" + endISO +
                      "&timeframe=1Min&limit=10000";

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, ("APCA-API-KEY-ID: " + apiKey).c_str());
    headers = curl_slist_append(headers, ("APCA-API-SECRET-KEY: " + secretKey).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "Failed to download bars: " << curl_easy_strerror(res) << std::endl;
        return;
    }

    try {
        nlohmann::json json = nlohmann::json::parse(readBuffer);
        auto bars = json["bars"];

        std::ofstream out(outputCSVPath);
        out << "timestamp,price\n";

        for (const auto& bar : bars) {
            std::string time = bar["t"];
            double price = bar["c"]; // close price
            out << time << "," << price << "\n";
        }

        std::cout << "Historical bars saved to: " << outputCSVPath << std::endl;
    } catch (...) {
        std::cerr << "Error parsing or saving bars: " << readBuffer << std::endl;
    }
}
