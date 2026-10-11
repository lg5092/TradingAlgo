# Alpaca Trading Bot

A C++ trading bot that uses the Alpaca API to trade a stock with a simple moving average (SMA) crossover strategy. This project was designed with two modes.

## Features

- **Backtest Mode**: downloads one day of 1-minute bars from Alpaca and simulates the testing strategy on those bars. 
- **Live Mode**: checks the latest trade prices from the past 2 seconds and sends market order to the Alpaca paper trading account while the market is currently open. 

