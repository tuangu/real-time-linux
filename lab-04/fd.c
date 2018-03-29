#include <stdio.h>

int OpenRandomGenerator();

int main(int argc, char *argv[]) {

    int fd = OpenRandomGenerator();
    FILE *frandom = fdopen(fd, "r");
    
    const char *fmt = "%d";
    int num;
    int count = 0;

    printf("Lotto numbers:\n");
    while (fscanf(frandom, fmt, &num) != EOF) {
        if (num < 20)
            count++;

        printf("%d \n", num);
    }
    
    printf("Below 20 count: %d\n", count);

    return 0;
}
