CC = gcc
TARGET = weather


$(TARGET): main.c 
	$(CC) -o $(TARGET) main.c -pthread

clean:
	@rm $(TARGET) 2> /dev/null || true

.PHONY: clean
