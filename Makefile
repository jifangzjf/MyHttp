.PHONY: clean all
CC = g++
CPPFLAGS = -Wall -g -pthread -std=c++11 -lrt
BIN = MyHttpd testClient
SOURCES = $(BIN.=.cpp)

all: $(BIN)
$(BIN): $(SOURCES) Socket.cpp threadpool.cpp condition.cpp commen.cpp

clean:
	-rm -rf $(BIN) core
