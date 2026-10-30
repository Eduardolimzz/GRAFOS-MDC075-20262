CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = main
SRC = cobertura.c main.c

all: $(TARGET)

$(TARGET): $(SRC) cobertura.h
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) main.exe *.o

.PHONY: all run clean
