CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -O2
LDLIBS=-lregex
TARGET=TMC
SRC=TMC.c machine.c input_handler.c util.c
MACHINE=machine.in
INPUT=input.in

.PHONY: all build run clean reload

all: build

build:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDLIBS)

run:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDLIBS)
	./$(TARGET) $(MACHINE) $(INPUT)

reload: clean run

clean:
	rm -f $(TARGET)
