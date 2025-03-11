#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 10

// what does volatile do?
volatile int sum = 0;
pthread_mutex_t mutex;

#define SYNC 1

// how would you write this without macros?
#if SYNC
#define T(x) x
#else
#define T(x)
#endif

void *thread_function(void *data) {
    (void)data;

    for (int i = 0; i < 10000; i++) {
        T(pthread_mutex_lock(&mutex);)

        sum++;

        T(pthread_mutex_unlock(&mutex);)
    }


    pthread_exit(NULL);
}


int main() {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

#if SYNC
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        return 1;
    }
#endif


    for (t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, thread_function, NULL);
        if (rc) {
            perror("pthread_create");
            exit(-1);
        }
    }


    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }


#if SYNC
    pthread_mutex_destroy(&mutex);
#endif


    printf("sum = %d\n", sum);
    pthread_exit(NULL);
}
