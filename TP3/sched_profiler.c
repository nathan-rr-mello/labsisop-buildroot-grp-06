#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_LENGTH 1000
#define N_THREADS 5

pthread_mutex_t lock;
pthread_barrier_t bar;

pthread_t* prcs;
char* buffer;
int ptr = 0;

void *start(void *arg){
    pthread_barrier_wait(&bar);

    while(1){ 
        pthread_mutex_lock(&lock);
        if (ptr >= BUFFER_LENGTH){
            pthread_mutex_unlock(&lock);
			break;
        }
        buffer[ptr] = *((char *)arg);
        ptr++;
        printf("%c", *((char *)arg));
        pthread_mutex_unlock(&lock);
    }
    free(arg);
}

int main(int argc, char **argv)
{
    pthread_mutex_init(&lock, NULL);
	pthread_barrier_init(&bar, NULL, N_THREADS);
    
    buffer = (char *)malloc(BUFFER_LENGTH * sizeof(char));
    prcs = (pthread_t *)malloc(N_THREADS * sizeof(pthread_t));

    for (int i = 0; i < N_THREADS; i++) {
        char *thread_char = (char *)malloc(sizeof(char));
        *thread_char = 'A' + i;
        pthread_create(&prcs[i], NULL, start, (void *)thread_char);
    }

    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(prcs[i], NULL);
    }

    pthread_barrier_destroy(&bar);
    pthread_mutex_destroy(&lock);
    free(buffer);
    free(prcs);
    
    return 0;
}