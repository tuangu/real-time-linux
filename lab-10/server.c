// A server that uses Unix domain sockets
// The addr of the server is server.sock
// Request format contain ascii string: "[c] [i]\0"
// The server send the [c] 10 times with [i] secs interval

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "server.sock"
#define LISTEN_BACKLOG 50

#define handle_error(msg) \
    do { perror(msg); exit(1); } while (0)

int socketFd;
static void SIGINT_handler(int);

int main() {
    
    // create socket
    // initialize the address struct
    // bind the address to the socket
    // listen to connection
    // wait for connection requests
    // accept returns a new socket descriptor
    
    int clientFd;
    struct sockaddr_un socketAddr;
    int pid1, pid2;

    // install signal handler
    if (signal(SIGINT, SIGINT_handler) == SIG_ERR) {
        handle_error("signal()");
    }

    // construct the address
    socketAddr.sun_family = AF_UNIX;
    strcpy(socketAddr.sun_path, SOCK_PATH);

    // create a socket
    socketFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socketFd == -1) {
        handle_error("socket()");
    } else {
        printf("Creating a new socket\n");
    }

    // bind an address to the socket
    if (bind(socketFd, (const struct sockaddr*) &socketAddr, sizeof(socketAddr)) == -1) {
        handle_error("bind()");
    } else { // bind() returns 0 on success
        printf("Binding the address to the socket\n");
    }

    // listen to connection
    if (listen(socketFd, LISTEN_BACKLOG) == -1) {
        handle_error("listen()");        
    } else {
        printf("Listening to connection\n");
    }

    while (1) {
        // wait for connection request, blocked
        clientFd = accept(socketFd, NULL, 0);

        if (clientFd == -1)
            handle_error("accept()");

        pid1 = fork();
        if (pid1 < 0) {
            handle_error("first fork()");
        } else if (pid1 == 0) {
            // child process
            // double forking
            pid2 = fork();
            if (pid2 < 0) {
                handle_error("second fork()");
            } else if (pid2 == 0) {
                printf("Sever PID: %d\n", getpid());

                //handle the connection request
                char buffer[4];
                char ch;
                int interval;

                recv(clientFd, (void *) buffer, sizeof(buffer), MSG_WAITALL);
                ch = buffer[0];
                interval = atoi((const char *) &buffer[2]);

                for (int i = 0; i < 10; i++) {
                    send(clientFd, (const void *) &ch, sizeof(ch), 0);
                    sleep(interval);
                }
                
                // send EOF
                ch = '\0';
                send(clientFd, (const void *) &ch, sizeof(ch), 0);
                
                shutdown(clientFd, SHUT_RDWR);
                printf("Done. Server %d Terminating\n", getpid());
                exit(0);
            }
            
            exit(0);
        }
        wait(NULL);
    }

    // cannot reach here
    exit(0);
}

static void SIGINT_handler(int sig_no) {
    if (sig_no == SIGINT) {
        printf("Received SIGINT. Terminating\n");
        close(socketFd);
        remove(SOCK_PATH);
    }
}
