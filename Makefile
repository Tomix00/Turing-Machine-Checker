CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -O2
TARGET=TMC
SRC=TMC.c
MACHINE=machine.in
INPUT=input.in

.PHONY: all build run clean reload

all: build

build:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)
	clear
	./$(TARGET) $(MACHINE) $(INPUT)

reload: clean run

clean:
	rm -f $(TARGET)
