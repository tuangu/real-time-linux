// Five threads increment the common counter. They know the upper limit that is
// 5 000 000. The main thread waits until upper limit has been reached.
// There are three problems in this program. 
// 1. The main thread wastes resources because it waits in the busy loop.
// 2. Threads at least sometimes do extra steps
// 3. The main thread cannot detect the situation when counter becomes exactly 
//    MAXCOUNT.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define N 5
#define MAXCOUNT 5000000

pthread_mutex_t mutex;
pthread_cond_t cond;
int counter;  // Common counter

void *tf(void *param){
    int i;

    while (1) {
        pthread_mutex_lock(&mutex);

        if (counter == MAXCOUNT) {
            pthread_mutex_unlock(&mutex);
            pthread_cond_signal(&cond);
            pthread_exit(NULL);
        }

        counter++;
        pthread_mutex_unlock(&mutex);
    }

    // pthread_exit(NULL); 
}

int main(void) {
    int i;
    pthread_t ti_arr[N];

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("pthread_mutex_init ");
        exit(EXIT_FAILURE);
    }

    if (pthread_cond_init(&cond, NULL) != 0) {
        perror("pthread_cond_init");
        exit(EXIT_FAILURE);
    }
    
    for (i = 0 ; i < N ; i++) {
        if (pthread_create(ti_arr+i, NULL, tf, NULL) != 0) {
            printf("Error in creating a thread %d\n", i);
            exit (0);
        }
    }
    
    // while (counter < MAXCOUNT);
    pthread_mutex_lock(&mutex);
    while (counter < MAXCOUNT)
        pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);
	printf("Counter is after the while loop %d\n", counter);


    for ( i = 0 ; i < N ; i++) {
		pthread_join(ti_arr[i], NULL);
      }
	printf("Counter is when all sub threads have finnished %d\n", counter);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}

