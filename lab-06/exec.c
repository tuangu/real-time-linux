#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    int fdLog;

    fdLog = open("log.txt", O_RDWR | O_CREAT, 
                S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    pid = fork();
    
    if (pid < 0) {
        perror("Fork:");
        exit(1);
    }
    if (pid == 0) { // child process
        char fd[5];
        snprintf(fd, 5, "%d", fdLog);
        // execute a new program
        execl("./filewriter", "filewriter", fd, (char *) 0);
        perror("Exec:");
        exit(0);
    }

    while (waitpid(pid, NULL, WNOHANG) == 0) {
        printf("Parent is working\n");
        sleep(1);
    }
    printf("Child terminated\n");
    printf("Printing log file\n");
    int c;
    FILE *logfile = fdopen(fdLog, "r");
    rewind(logfile);
    while ((c = getc(logfile)) != EOF) 
        putchar(c);
    close(fdLog);
    exit(0);
}
