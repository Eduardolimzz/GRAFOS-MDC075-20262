CC = gcc
CFLAGS = -O2 -Wall -Wextra -Werror -std=c11
TARGET = main
SRC = hamiltoniano.c tps.c main.c

all: $(TARGET)

$(TARGET): $(SRC) hamiltoniano.h tps.h
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) main.exe *.o

.PHONY: all run clean
