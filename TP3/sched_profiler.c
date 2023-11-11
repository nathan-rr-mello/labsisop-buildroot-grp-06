// gcc sched_profiler.c -o run -lpthread
// taskset -c 0 ./run

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

    int tid;
    tid = (int)(long int)arg;
    char id = 'A' + tid;

    while(1){ 
        pthread_mutex_lock(&lock);
        if (ptr >= BUFFER_LENGTH){
            pthread_mutex_unlock(&lock);
			break;
        }
        buffer[ptr] = id;
        ptr++;
        pthread_mutex_unlock(&lock);
    }
}

int main(int argc, char **argv)
{
    pthread_mutex_init(&lock, NULL);
	pthread_barrier_init(&bar, NULL, N_THREADS);
    
    long int i;
    buffer = (char *)malloc(BUFFER_LENGTH * sizeof(char));
    prcs = (pthread_t *)malloc(N_THREADS * sizeof(pthread_t));

    for (i = 0; i < N_THREADS; i++)
        pthread_create(&prcs[i], NULL, start, (void *)i);

    for (int i = 0; i < N_THREADS; i++)
        pthread_join(prcs[i], NULL);

    for (int j = 0; j < BUFFER_LENGTH; j++)
        putchar(buffer[j]);

    pthread_barrier_destroy(&bar);
    pthread_mutex_destroy(&lock);
    free(buffer);
    free(prcs);
    
    return 0;
}