CC = gcc
	
normal: thread_sync.c 
	$(CC) -o out thread_sync.c -pthread

mutex: thread_sync.c 
	$(CC) -o out thread_sync.c -pthread -D USE_MUTEX

clean:
	@rm out 2> /dev/null || true

.PHONY: clean
