# Alpaca Trading Bot

A C++ trading bot that uses the Alpaca API to trade a stock with a simple moving average (SMA) crossover strategy. This project was designed with two modes.

## Features

- **Backtest Mode**: downloads one day of 1-minute bars from Alpaca and simulates the testing strategy on those bars. 
- **Live Mode**: checks the latest trade prices from the past 2 seconds and sends market order to the Alpaca paper trading account while the market is open.

## Strategy 
The bot retains a shorted SMA (5 adjustable periods) and a long SMA (10 adjustable periods) of recent prices:

- When the short SMA crosses **above** the long SMA, it buys shares. This indicates a sign of a short term momentum.
- When the short SMA crosses **below** the long SMA, it sells shares. This indicates a a sign of a break in the short term momentum.

The trading bot does not send a signal until is has collect enough price data to fulfill the long SMA window. 

## Project structure

```
├── include/
│   ├── api.h          # Alpaca REST API calls
│   ├── backtest.h     # Backtesting engine
│   ├── strategy.h     # SMA crossover strategy
│   └── json.hpp       # nlohmann/json (single-header)
├── src/
│   ├── main.cpp       # Entry point: picks the mode, runs the loop
│   ├── api.cpp        # Account info, latest price, orders, historical bars
│   ├── backtest.cpp   # Runs the strategy over CSV data and prints stats
│   └── strategy.cpp   # SMA calculation and buy/sell/hold signals
├── data/              # Historical price CSVs (timestamp,price)
└── Makefile
```
### Backtest 

This particular project downloads AAPL 1-minutes bars for March 1, 2024 into a CSV file, then runs the strategy on them using a $10,000 starting cash demo. It prints each trade that gets executed and then returns a summary of the statistics of the backtest. The data samples are hard-coded but can be easily swapped. 

Example: 
```
--- Backtest Summary ---
Final Portfolio Value: $...
Net P&L: $...
Return: ...%
Total Trades: ...
Winning Trades: ...
Losing Trades: ...
Win Rate: ...%
```
### Live 

The trading bot checks the latest AAPL trade price every 2 seconds, updates the strategy in real-time, and sends a market order to the trading account when a buy or sell signal appears. To stop the continuous loop, 'Ctrl+C' stops the program. 
