CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -I/usr/local/opt/curl/include
LDFLAGS = -L/usr/local/opt/curl/lib -lcurl
SRC = $(wildcard src/*.cpp)
TARGET = build/trading_bot

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $@ $(LDFLAGS)

clean:
	rm -f build/*
