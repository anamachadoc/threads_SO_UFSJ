#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define NUM_THREADS 3
#define MAX_COUNT 10

int winner = -1;  
pthread_mutex_t mutex;  

void* count_and_print(void* arg) {
    int thread_id = *((int*)arg);  

    for (int i = 0; i <= MAX_COUNT; i++) {
        pthread_mutex_lock(&mutex);  
        
        if (winner != -1) {
            pthread_mutex_unlock(&mutex);
            return NULL; 
        }

        printf("Thread %d: %d\n", thread_id, i);

        if (i == MAX_COUNT) {
            winner = thread_id; 
            printf("Thread %d é a vencedora!\n", thread_id);
        }

        pthread_mutex_unlock(&mutex);  
        usleep(10000);
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i; 
        pthread_create(&threads[i], NULL, count_and_print, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}
