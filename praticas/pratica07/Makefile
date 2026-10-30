CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = main
SRC = bellman_ford.c main.c

all: $(TARGET)

$(TARGET): $(SRC) bellman_ford.h
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) main.exe *.o

.PHONY: all run clean
