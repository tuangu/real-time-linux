#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define SIGNAL_NR   10 

struct timespec ptime[SIGNAL_NR] = {0};
struct timespec ctime[SIGNAL_NR] = {0};
int signal_index = 0;

struct timeinfo {
    struct timespec *head;
    int index;
}

void rt_handler(int, siginfo_t *, void *);

// display the timers' resolution
// create a new process, waits for the signals to come with a signal handler
// when the signal is received, signal handler acquire the current time value
// calculate the delay time using signal sending time from parent process
int main() {
    struct timespec time_res;
    struct timeinfo child_info = {ctime, signal_index};

    pid_t pid;
    int fd_pipe[2];

    // print time resolution
    if (clock_getres(CLOCK_REALTIME, &time_res) == -1) {
        perror("clock_getres ");
        exit(EXIT_FAILURE);
    }
    printf("time resolution: %ld s %ld ns", time_res.tv_sec, time_res.tv_nsec);


    pipe(fd_pipe);
    pid = fork();
    if (pid < 0) {
       perror("fork ");
       exit(EXIT_FAILURE);
    } else if (pid > 0) {
        
        struct sigaction sa;
        struct sigset_t mask;

        sa.sa_flags = SA_SIGINFO;
        sa.sa_sigaction = rt_handler;
        if (sigaction(SIGRTMAX, &sa, NULL) == -1) {
            perror("sigaction ");
            exit(EXIT_FAILURE);
        }

        sigemptyset(&mask);
        sigaddset(&mask, SIGRTMAX);
        
        for (int i = 0; i < SIGNAL_NR; i++) {
            if (sigsuspend(&mask) == -1) {
                perror("sigsuspend ");
                exit(EXIT_FAILURE);
            }

        }
        
        exit(EXIT_SUCCESS);
    }

    

}

void rt_handler(int signo, siginfo_t *info, void *p) {
    
}
