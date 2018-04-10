#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/wait.h>

int OpenChatFellow();
void print_log(FILE *);

int main(int argc, char *argv[]) {
    pid_t pid1;
    int i = 0;
    int status = 0;
    FILE *fellowLog = fopen("chatlog.txt", "a+");

    pid1 = fork();
    if (pid1 < 0) {
        printf("ERROR: cannot create child process\n");
    } else if (pid1 == 0) {

        int fellowDesc = OpenChatFellow();
        FILE *fellowChat = fdopen(fellowDesc, "r");
        int counter = 0;
        
        const char *fmt = "%c";
        char c;

        while (fscanf(fellowChat, fmt, &c) == 1) {
            printf("%c \n", c);
            fflush(stdout);

            if (putc(c, fellowLog) == EOF)
                printf("ERROR: cannot write to file\n");
            else 
                counter++;

        }
        
        fclose(fellowChat);
        exit(counter);
    }   

    char c;
    while((c = getchar()) != 1) {
        printf("%c", c);
        fflush(stdout);
        
        if ((c == 'q') || (c == 'Q')) 
            break;
    }

    wait(&status) > 0;
    printf("Char written: %d", WEXITSTATUS(status));

    fclose(fellowLog);
    exit(0);
}
