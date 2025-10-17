CC = g++
CFLAGS = -std=c++17 -O2 -Wall
SRC = duanyao.cpp tiles.cpp yifan.cpp erfan.cpp sanliufan.cpp count.cpp dianshu.cpp
OUT = duanyao

.PHONY: all run clean

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

run: $(OUT)
	@./$(OUT)

clean:
	rm -f $(OUT)