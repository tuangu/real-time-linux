CC = gcc
	
phase_1: p1_write_1.c p1_write_2.c
	$(CC) -o w.1.out p1_write_1.c
	$(CC) -o w.2.out p1_write_2.c

phase_2: p2_write_1.c p2_write_2.c
	$(CC) -o w.1.out p2_write_1.c
	$(CC) -o w.2.out p2_write_2.c

phase_3: p3_write_1.c p3_write_2.c
	$(CC) -o w.1.out p3_write_1.c
	$(CC) -o w.2.out p3_write_2.c

ex2: ex2.c
	$(CC) -o ex2.out ex2.c

clean:
	rm *.out
	rm -f log.txt

.PHONY: clean
