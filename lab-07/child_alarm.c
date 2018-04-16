#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>

static void alarm_handler(int sig_no);

int main() {
    pid_t pid;

    if (signal(SIGALRM, alarm_handler) == SIG_ERR) {
        perror("Signal handler:");
        exit(1);
    }
    
    printf("Parent PID: %d\n", getpid());
    alarm(5);
    pid = fork();
    if (pid < 0) {
        perror("Fork:");
        exit(1);
    } else if (pid == 0) {
        printf("Child PID: %d\n", getpid());
        // alarm(3);
        pause();
        printf("Child terminating\n");
        exit(0);
    }
    
    pause();
    wait(NULL);
    exit(0);
}

void alarm_handler(int sig_no) {
    printf("PID=%d Time out\n", getpid());    
}
