CC = gcc
	
8a_1: multiplexed_io.c MultiSensorSimulatorEdu.o
	$(CC) -g -o out multiplexed_io.c MultiSensorSimulatorEdu.o -lrt

8a_2: async_io.c MultiSensorSimulatorEdu.o
	$(CC) -g -o out async_io.c MultiSensorSimulatorEdu.o -lrt

clean:
	@rm out 2> /dev/null || true

.PHONY: clean
