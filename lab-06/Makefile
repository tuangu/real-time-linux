CC = gcc
	
phase_1: zombies.c
	$(CC) -o out zombies.c

phase_2: double_forking.c
	$(CC) -o out double_forking.c

clean:
	@rm out 2> /dev/null || true

.PHONY: clean
