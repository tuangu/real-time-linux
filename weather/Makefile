CC = gcc
TARGET = weather
TEST = http_test
TARGET_TEST = $(TEST)


$(TARGET): main.c parson/parson.c
	$(CC) -o $(TARGET) main.c parson/parson.c -Iparson -g

$(TEST): $(TEST).c parson/parson.c
	$(CC) -o $(TARGET_TEST) $(TEST).c parson/parson.c -Iparson -g

clean:
	@rm $(TARGET) $(TARGET_TEST) 2> /dev/null || true

.PHONY: clean
