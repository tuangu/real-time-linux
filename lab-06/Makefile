CC = gcc
	
phase_1: zombies.c
	$(CC) -o out zombies.c

phase_2: double_forking.c
	$(CC) -o out double_forking.c

extra_a: chain_of_processes.c
	$(CC) -o out chain_of_processes.c

extra_b: exec.c write_to_file.c
	$(CC) -o filewriter write_to_file.c
	$(CC) -o out exec.c

clean:
	@rm out filewriter log.txt 2> /dev/null || true

.PHONY: clean
