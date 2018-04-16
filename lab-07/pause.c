#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>

static void alarm_handler(int sig_no);

int main() {
    printf("Entering new program\n");
    
    if (signal(SIGALRM, alarm_handler) == SIG_ERR) {
        perror("Signal handler:");
        exit(1);
    }

    pause();
    exit(0);
}

void alarm_handler(int sig_no) {
    printf("Signal received\n");    
}
