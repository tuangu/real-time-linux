#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "MultiSensorSimulator.h"

#define DESC_NUM    10
#define STEPS       5

int maxFd(int *i, int);

int main() {
    int sensorDescriptors[DESC_NUM];
    fd_set fdset;
    int fdReady;
    int readRet;
    struct timeval timeout;
    Tmeas measurement;
    int count = 0;
    int fdDone = 0;
    struct timespec currentTime;
    struct timespec diffTime;
    struct timespec sumDelay;

    // fill the timeout struct
    // timeout.tv_usec = 0;
    // timeout.tv_sec = 20;

    // reset sum delay
    sumDelay.tv_nsec = 0;
    sumDelay.tv_sec = 0;

    StartSimulator(sensorDescriptors, STEPS);

    int maxfd = maxFd(sensorDescriptors, DESC_NUM) + 1;
    printf("Max fd: %d\n", maxfd);

    // reading sensor data
    while (1) {
        timeout.tv_usec = 0;
        timeout.tv_sec = 20;

        FD_ZERO(&fdset);
        for (int i = 0; i < DESC_NUM; i++) {
            // -1 mean that the descrtiptor has reached the end of file
            if (sensorDescriptors[i] != -1) {
                FD_SET(sensorDescriptors[i], &fdset);
            }
        }

        fdReady = select(maxFd(sensorDescriptors, DESC_NUM) + 1, &fdset, NULL, NULL, &timeout);
        // printf("fdReady: %d\n", fdReady);

        if (fdReady > 0) {
            for (int i = 0; i < DESC_NUM; i++) {
                if ((sensorDescriptors[i] != -1) && FD_ISSET(sensorDescriptors[i], &fdset)) {
                    readRet = read(sensorDescriptors[i], &measurement, sizeof(Tmeas));

                    // if the descriptor has reached the EOF, set it to -1
                    if (readRet == 0) {
                        sensorDescriptors[i] = -1;
                        ++fdDone;
                    } else {
                        // calculate total delay time
                        timespec_get(&currentTime, TIME_UTC);
                        diffTime = diff_timespec(&currentTime, &measurement.moment);
                        increment_timespec(&sumDelay, &diffTime);
                        ++count;
                        printf("[%d] Sensor %d: %d\n", count, i, measurement.value);
                        printf("     Moment: %ld sec %ld nsec\n", measurement.moment.tv_sec, measurement.moment.tv_nsec);
                        printf("     Diff: %ld sec %ld nsec\n", diffTime.tv_sec, diffTime.tv_nsec);
                    }
                }
            }
        } else if (fdReady < 0) {
            printf("ERROR: an error occured when calling select\n");
            exit(1);
        } else if (fdReady == 0) {
            exit(0);
        }
    }
    printf("All sensors' data received\n");
    printf("Delay: %ld s %ld ns\n", sumDelay.tv_sec, sumDelay.tv_nsec);
    exit(0);
}

int maxFd(int fd[], int n) {
    int max = fd[0];
    for (int i = 0; i < n; i++) {
        if (fd[i] > max)
            max = fd[i];
    }

    return max;
}
