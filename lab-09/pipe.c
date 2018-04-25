#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int fd_arr[2];
    int num;

    pipe(fd_arr);
// create a child process
    pid = fork();
    if (pid < 0) {
        perror("fork: ");
        exit(1);
    } else if (pid == 0) {
        // close read end of the pipe
        close(fd_arr[0]);    
        
        // write to pipe digit 0,1,..,9
        for (int i = 0; i < 10; i++) {
            write(fd_arr[1], &i, sizeof(int));
            sleep(1);
        }
        close(fd_arr[1]);
        exit(0);
    }

    // parent read and display the digit
    close(fd_arr[1]); 
    while (read(fd_arr[0], &num, sizeof(int)) > 0)
        printf("%d\n", num);
    wait(NULL);
    close(fd_arr[0]);
    exit(0);
}
