CC = gcc
	
7a_phase_1: child_alarm.c
	$(CC) -o out child_alarm.c

7a_phase_2: pause.c exec_alarm.c
	$(CC) -o pause pause.c
	$(CC) -o out exec_alarm.c

clean:
	@rm out pause 2> /dev/null || true

.PHONY: clean
