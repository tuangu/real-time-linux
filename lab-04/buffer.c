#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

int main(int argc, char *argv[]) {
    int testMode = 0;
    if (argc > 1)
        testMode = atoi(argv[1]);
    
    char buffer[5];
    if (setvbuf(stdout, buffer, _IOFBF, 5) != 0) {
        printf("Error: cannot set buffer\n");
        return -1;
    }

    for (int i = 0; i < 13; i++) {
        putchar('a');
        sleep(1);
    }
    
    switch (testMode) {
    case 0:
        fflush(stdout);
        return 0;
    case 1:
        exit(0);
    case 2:
    default:
        return 0;
    }
}
