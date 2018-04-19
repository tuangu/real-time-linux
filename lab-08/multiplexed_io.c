#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>

#include "MultiSensorSimulator.h"

#define DESC_NUM 10

int maxFd(int *i, int);

int main() {
    int sensorDescriptors[DESC_NUM];
    fd_set fdset;
    int fdReady;
    int readRet;
    struct timeval timeout;
    Tmeas measurement;
    int count = 0;

    // fill the timeout struct
    timeout.tv_usec = 0;
    timeout.tv_sec = 20;

    // Start the simulator
    // if (StartSimulator(sensorDescriptors, 5) != 10) {
    //    printf("Cannot start simulator\n");
    //    exit(1);
    // }
    int n = StartSimulator(sensorDescriptors, 5);
    printf("Start %d sensors\n", n);
    sleep(1);

    int maxfd = maxFd(sensorDescriptors, DESC_NUM) + 1;
    printf("Max fd: %d\n", maxfd);

    // reading sensor data
    while (1) {
        FD_ZERO(&fdset);
        for (int i = 0; i < DESC_NUM; i++) {
            // -1 mean that the descrtiptor has reached the end of file
            if (sensorDescriptors[i] != -1) {
                FD_SET(sensorDescriptors[i], &fdset);
            }
        }

        fdReady = select(maxfd, &fdset, NULL, NULL, &timeout);
        printf("fdReady: %d\n", fdReady);

        if (fdReady > 0) {
            for (int i = 0; i < DESC_NUM; i++) {
                if ((sensorDescriptors[i] != 1) && FD_ISSET(sensorDescriptors[i], &fdset)) {
                    readRet = read(sensorDescriptors[i], &measurement, sizeof(Tmeas));

                    // if the descriptor has reached the EOF, set it to -1
                    if (readRet == 0) {
                        sensorDescriptors[i] = -1;
                    } else {
                        ++count;
                        printf("[%d] Sensor %d: %d\n", count, i, measurement.value);
                    }
                }
            }
        } else if (fdReady == 0) {
            printf("Received all sensors' values\n");
            exit(0);
        } else {
            printf("ERROR: an error occured when calling select\n");
            exit(1);
        }
    }
    
    exit(0);
}

int maxFd(int fd[], int n) {
    int max = 0;
    for (int i = 0; i < n; i++) {
        printf("fd%d: %d\n", i, fd[i]);
        if (fd[i] > max)
            max = fd[i];
    }

    return max;
}
