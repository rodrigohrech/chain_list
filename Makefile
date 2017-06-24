CC=gcc
BIN_DIR=./bin/
SRC_DIR=./
CFLAGS=-c

all: main

main: chain_list.o
	$(CC) $(SRC_DIR)main.c -Wall -o main $(BIN_DIR)chain_list.o

chain_list.o: clean chain_list.c
	$(CC) $(CFLAGS) $(SRC_DIR)chain_list.c -Wall -o $(BIN_DIR)chain_list.o

clean:
	find $(BIN_DIR) -type f ! -name '*.c' ! -name 'Makefile' -delete
