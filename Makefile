CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = shell

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	rm -f $(TARGET)

.PHONY: clean 