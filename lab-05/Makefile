CC = gcc
	
5a_a: fan_of_processes.c
	$(CC) -o fop.out fan_of_processes.c

5a_b_p1: chat_two_processes.c InputGenDebian.o
	$(CC) -o chat.out chat_two_processes.c InputGenDebian.o

clean:
	@rm *.out chatlog.txt 2> /dev/null || true

.PHONY: clean
