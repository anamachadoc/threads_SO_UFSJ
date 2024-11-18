#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define NUM_THREADS 5
#define MAX_COUNT 100

int winner = -1, threads_created = 0;  
pthread_mutex_t mutex;  
pthread_cond_t cond;  

bool is_prime(int num) {
    if (num <= 1) {
        return false;
    }
    if (num == 2) {
        return true;
    }
    if (num % 2 == 0) {
        return false;
    }
    for (int i = 3; i * i <= 100; i += 2) {
        if (num % i == 0) {
            return false;  
        }
    }
    return true;  
}

void* count_and_print(void* arg) {
    int thread_id = *((int*)arg);  

    pthread_mutex_lock(&mutex);
    threads_created++;

    while (threads_created < NUM_THREADS) {
        pthread_cond_wait(&cond, &mutex);
        //printf("%d threads esperando a criação de todas as threads", &threads_created);
    }

    pthread_cond_broadcast(&cond); 
    pthread_mutex_unlock(&mutex);

    for (int i = 0; i <= MAX_COUNT; i++) {
        pthread_mutex_lock(&mutex);  // Garante acesso exclusivo ao winner

        if (winner != -1) {
            pthread_mutex_unlock(&mutex);
            return NULL;  // Se alguém já ganhou, sai
        }

        int rand_num = rand() % 101; 
        if (rand_num % 5 == 0) {
            printf("Thread %d encontrou um multiplo de cinco e perdeu sua vez\n", thread_id);
            i--;
        } else if (is_prime(rand_num)){
            printf("Thread %d encontrou um numero primo e esta em %d\n", thread_id, i + 1);
            i++;
        } else{
            printf("Thread %d estah em %d\n", thread_id, i);
        }

        if (i >= MAX_COUNT) {
            winner = thread_id;  // Marca o vencedor
            printf("Thread %d venceu!\n", thread_id);
        }

        pthread_mutex_unlock(&mutex);  // Libera o mutex
        usleep(10000); 
    }
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL); 

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i; 
        pthread_create(&threads[i], NULL, count_and_print, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
      pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond); 

    return 0;
}
