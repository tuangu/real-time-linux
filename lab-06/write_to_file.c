#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Please specify the file descriptor.\n");
        exit(1);
    }

    int fd = atoi(argv[1]);
    FILE *logfile = fdopen(fd, "a+");
    
    if (logfile == NULL) {
        printf("Can not open file.\n");
        exit(1);
    }
    
    const char content[6] = "AAAAA";
    int ret;
    for (int i = 0; i < 5; i++) {
        ret = fputs(content, logfile);
        if (ret == EOF) {
            printf("Cannot write to file.\n");
            exit(1);
        }
        sleep(1);
    }
    
    printf("Child is terminating.\n");
    fclose(logfile);
    _exit(0);
}
