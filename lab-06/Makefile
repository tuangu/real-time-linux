CC = gcc
	
phase_1: zombies.c
	$(CC) -o out zombies.c

phase_2: double_forking.c
	$(CC) -o out double_forking.c

extra_a: chain_of_processes.c
	$(CC) -o out chain_of_processes.c

clean:
	@rm out 2> /dev/null || true

.PHONY: clean
