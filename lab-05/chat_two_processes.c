#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/wait.h>

int OpenChatFellow();
void print_log(FILE *);

int main(int argc, char *argv[]) {
    pid_t pid1, pid2;
    int i = 0;
    FILE *fellowLog = fopen("chatlog.txt", "a+");

    pid1 = fork();
    if (pid1 < 0) {
        printf("ERROR: cannot create child process\n");
    } else if (pid1 == 0) { // child 1
        pid2 = fork();
        if (pid2 < 0) {
            printf("ERROR: cannot create child process\n");
        } else if (pid2 > 0) {
            exit(0); // child 1 terminated
        }

        int fellowDesc = OpenChatFellow();
        FILE *fellowChat = fdopen(fellowDesc, "r");
        
        const char *fmt = "%c";
        char c;

        while (fscanf(fellowChat, fmt, &c) == 1) {
            printf("%c \n", c);
            fflush(stdout);

            if (putc(c, fellowLog) == EOF) {
                printf("ERROR: cannot write to file\n");
            }
        }
        
        fclose(fellowChat);
        exit(0);
    }   

    char c;
    while((c = getchar()) != 1) {
        printf("%c", c);
        fflush(stdout);
        
        if ((c == 'q') || (c == 'Q')) 
            break;
    }

    wait(NULL);
    rewind(fellowLog);
    print_log(fellowLog);

    fclose(fellowLog);
    exit(0);
}

void print_log(FILE *logFile) {
    int counter = 0;
    char c;
    while ((c = fgetc(logFile)) != EOF) {
        counter++;
        int ret = putchar(c);
        if (ret == EOF) {
            printf("ERROR: cannot write to stdout\n");
            break;
        }

        if (counter > 6) {
            ret = putchar('\n');
            if (ret == EOF) {
                printf("ERROR: cannot write to stdout\n");
                break;
            }
            counter = 0; 
        }
    }
    putchar('\n');
}













