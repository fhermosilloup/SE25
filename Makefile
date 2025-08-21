CC = gcc
CFLAGS = -Wall -pthread
LDFLAGS = -lpigpio -lrt
TARGET = main
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)
