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
    ssize_t ret;
    for (int i = 0; i < 100000; i++) {
        ret = write(fd, content, len);
        if (ret < 0) {
            printf("Error: cannot write to file\n");
            return -1;
        }
    }

    return 0;
}
