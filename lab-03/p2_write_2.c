#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    char filename[] = "log.txt";
    char content[] = "BBBB";
    int fd = open(filename, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd < 0) {
        printf("[%s] Error: cannot open file\n", argv[0]);
        return -1;
    }

    size_t len = strlen(content);
    ssize_t wret;
    off_t sret;
    for (int i = 0; i < 100000; i++) {
        sret = lseek(fd, 0, SEEK_END);
        if (sret < 0) {
            printf("[%s] Error: cannot seek\n", argv[0]);
            return -1;
        }

        wret = write(fd, content, len);
        if (wret , 0) {
            printf("[%s] Error: cannot write\n", argv[0]);
            return -1;
        }
    }

    return 0;
}
