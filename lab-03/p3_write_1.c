#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    char filename[] = "log.txt";
    char content[] = "AAAA";
    int fd = open(filename, O_RDWR | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd < 0) {
        printf("[%s] Error: cannot open file\n", argv[0]);
        return -1;
    }

    size_t len = strlen(content);
    ssize_t wret;
    for (int i = 0; i < 100000; i++) {
        wret = write(fd, content, len);
        if (wret , 0) {
            printf("[%s] Error: cannot write\n", argv[0]);
            return -1;
        }
    }

    return 0;
}
