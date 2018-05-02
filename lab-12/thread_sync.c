#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define THREAD_NR   10
#define INC_COUNT   10000000

#ifdef USE_MUTEX
  pthread_mutex_t mutex;
#endif
long counter = 0;

void *thread_func(void *arg) {
    for (int i = 0; i < INC_COUNT; i++) {
#ifdef USE_MUTEX
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
#else
        counter++;
#endif
        ++*(long *)arg;
    }

    pthread_exit(NULL);
}

int main() {
    
    pthread_t ids[THREAD_NR];
    long local_counter[THREAD_NR] = {0};

#ifdef USE_MUTEX
    // init mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("pthread_mutex_init ");
        exit(EXIT_FAILURE);
    }
#endif

    // create threads
    for (int i = 0; i < THREAD_NR; i++) {
        if (pthread_create(&ids[i], NULL, thread_func, (void *) &local_counter[i]) != 0) {
            perror("pthread_create ");
            exit(EXIT_FAILURE);
        }
    }
    
    // wait for all threads
    for (int i = 0; i < THREAD_NR; i++) {
        pthread_join(ids[i], NULL);
    }
    
    // print information
    printf("Common counter: %ld\n", counter);
    printf("Missed increments: %ld\n", THREAD_NR * INC_COUNT - counter);
    for (int i = 0; i < THREAD_NR; i++) 
        printf("Local counter %d: %ld\n", i, local_counter[i]);

#ifdef USE_MUTEX
    pthread_mutex_destroy(&mutex);
#endif

    exit(EXIT_SUCCESS);
}
