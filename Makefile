CC = g++
CFLAGS = -std=c++17 -O2 -Wall
MYSQL_FLAGS := $(shell mysql_config --cflags --libs)
SRC = duanyao.cpp tiles.cpp yifan.cpp erfan.cpp sanliufan.cpp count.cpp dianshu.cpp
OUT = duanyao
LOGIN_SRC = login.cpp
LOGIN_OUT = login

.PHONY: all run clean

all: $(OUT) $(LOGIN_OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

$(LOGIN_OUT): $(LOGIN_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(MYSQL_FLAGS)

run: $(OUT)
	@./$(OUT)

clean:
	rm -f $(OUT)