#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Specify the content\n");
        return -1;
    }

    while (1) {
        write(STDOUT_FILENO, argv[1], 2);
        sleep(1);
    }

    return 0;
}
