#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    pid_t pid;

    int i;
    for (i = 5; i >= 0; i--) {
        pid = fork();

        if (pid < 0) {
            perror("Fork: ");
            exit(1);
        }
        if (pid > 0) {
            int status;
            wait(&status);
            sleep(1);
            printf("Exit status: %d\n", WEXITSTATUS(status));
            exit(WEXITSTATUS(status) + 1);
        }
    }
    sleep(1);
    exit(0);
}
