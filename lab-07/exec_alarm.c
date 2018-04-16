#include <unistd.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>


int main() {
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        perror("Fork:");
        exit(1);
    } else if (pid == 0) {
        alarm(5);

        printf("Starting new program\n");
        execl("pause", "./pause", (char *)0);
    }
    
    wait(NULL);
    exit(0);
}

