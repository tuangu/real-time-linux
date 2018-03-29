#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void print_flags(int fd) {
    int flags = fcntl(fd, F_GETFL);

    // Access mode
    switch (flags & O_ACCMODE) {
    case O_RDONLY:
        printf("Access mode: Read Only\n");
        break;
    case O_WRONLY:
        printf("Access mode: Write Only\n");
        break;
    case O_RDWR:
        printf("Access mode: Read/Write\n");
        break;
    default:
        printf("Access mode: unknown\n");
    }

    // Blocking
    if (flags & O_NONBLOCK) 
        printf("O_NONBLOCK: ON\n");
    else
        printf("O_NONBLOCK: OFF\n");

    // Append
    if (flags & O_APPEND)
        printf("O_APPEND: ON\n");
    else
        printf("O_APPEND: OFF\n");
}

int main(int argc, char *argv[]) {
    char buf[1];
    int fd;
    ssize_t ret;
    int flags;
    int errno;
        
    // Test 1
    printf("===================================\nTest 1\n===================================\n");

    fd = open("/dev/tty", O_RDWR);
    print_flags(fd);

    ret = read(fd, buf, 1);
    if (ret == -1) {
        printf("[%s] Read failed: %s\n", argv[0], strerror(errno));
    } else if (ret == 0) {
        printf("[%s] Error: end of file\n", argv[0]);
    } else if (ret > 0) {
        printf("[%s] Read succeeded. Character was %s\n", argv[0], buf);
    }

    ret = write(fd, "A", 1);
    if (ret == -1) {
        printf("[%s] Write failed: %s\n", argv[0], strerror(errno));
    } else if (ret == 1) {
        printf("[%s] Write succeeded.\n", argv[0]);
    }

    ret = close(fd);
    if (ret == -1) {
        printf("[%s] Error: cannot close\n", argv[0]);
    } else if (ret == 0) {
        printf("[%s] Closing terminal\n", argv[0]);
    }

    // Test 2
    printf("===================================\nTest 2\n===================================\n");

    fd = open("/dev/tty", O_RDONLY);
    print_flags(fd);

    ret = read(fd, buf, 1);
    if (ret == -1) {
        printf("[%s] Read failed: %s\n", argv[0], strerror(errno));
    } else if (ret == 0) {
        printf("[%s] Error: end of file\n", argv[0]);
    } else if (ret > 0) {
        printf("[%s] Read succeeded. Character was %s\n", argv[0], buf);
    }

    ret = write(fd, "A", 1);
    if (ret == -1) {
        printf("[%s] Write failed: %s\n", argv[0], strerror(errno));
    } else if (ret == 1) {
        printf("[%s] Write succeeded.\n", argv[0]);
    }

    // Test 3
    printf("===================================\nTest 3\n===================================\n");

    flags = fcntl(fd, F_GETFL);    
    flags = flags | O_NONBLOCK | O_APPEND;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        printf("[%s] Error: cannot set file flags\n", argv[0]);
        return -1;
    } 
    print_flags(fd);
    
    ret = read(fd, buf, 1);
    if (ret == -1) {
        printf("[%s] Read failed: %s\n", argv[0], strerror(errno));
    } else if (ret == 0) {
        printf("[%s] Error: end of file\n", argv[0]);
    } else if (ret > 0) {
        printf("[%s] Read succeeded. Character was %s\n", argv[0], buf);
    }

    // Test 4
    printf("===================================\nTest 4\n===================================\n");

    flags = fcntl(fd, F_GETFL);
    flags &= ~O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        printf("[%s] Error: cannot set file flags\n", argv[0]);
        return -1;
    }
    print_flags(fd);

    ret = read(fd, buf, 1);
    if (ret == -1) {
        printf("[%s] Read failed: %s\n", argv[0], strerror(errno));
    } else if (ret == 0) {
        printf("[%s] Error: end of file\n", argv[0]);
    } else if (ret > 0) {
        printf("[%s] Read succeeded. Character was %s\n", argv[0], buf);
    }

    // Close terminal
    close(fd);
    return 0;
}
