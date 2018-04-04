CC = gcc
	
part_a: buffer.c 
	$(CC) -o buffer.out buffer.c 

part_b: fd.c RandomGenDebian.o
	$(CC) fd.c RandomGenDebian.o -o fd.out

extra: FILE-info.c
	$(CC) -o fileinfo.out FILE-info.c 

clean:
	rm *.out

.PHONY: clean
