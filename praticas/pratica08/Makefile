CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = main
SRC = agm.c main.c

all: $(TARGET)

$(TARGET): $(SRC) agm.h
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) main.exe *.o

.PHONY: all run clean
