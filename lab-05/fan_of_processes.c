#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>

void print_max_processes();

int main(int argc, char *argv[]) {
    pid_t pid;
    int i = 0;

    print_max_processes();
    
    // Create 5 child processes
    while (i < 5) {
        pid = fork();
        i++;

        // Child process
        if (pid == 0) {
            char c = 'A' + i - 1;
            for (int j = 0; j < 5; j++) {
                printf("%c", c);
                fflush(stdout);
                sleep(1);
            }

            exit(0);
        }
    }
    
    // Wait for all children to terminate
    while(wait(NULL) > 0);
    exit(0);
}

void print_max_processes() {
    struct rlimit processLimit;

    int ret = getrlimit(RLIMIT_NPROC, &processLimit);
    
    if (ret == 0)
        printf("The maximum number of processes is %lo\n", processLimit.rlim_max);
    else 
        printf("ERROR: Cannot get the maximum number of processes.\n");
}
