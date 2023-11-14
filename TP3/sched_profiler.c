// gcc sched_profiler.c -o run -lpthread
// taskset -c 0 ./run

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_mutex_t lock;
pthread_barrier_t bar;

pthread_t* prcs;
char* buffer;
long bufferSize;
int ptr = 0;

// Funçaõ que cada thread irá executar;
void *start(void *arg){
    // Barreira para que cada thread esteja pronta;
    pthread_barrier_wait(&bar);

    int tid;
    tid = (int)(long int)arg;
    char id = 'A' + tid;

    while(1){ 
        pthread_mutex_lock(&lock);
        if (ptr >= bufferSize){
            pthread_mutex_unlock(&lock);
			break;
        }
        // Adiciona id da thread ao buffer;
        buffer[ptr] = id;
        ptr++;
        pthread_mutex_unlock(&lock);
    }
}

// Funçaõ pricipal;
int main(int argc, char **argv)
{
    if (argc != 4) {
        printf("Usage: %s <number_of_threads> <number_of_processors> <buffer_size>\n", argv[0]);
        return 1;
    }
    int nThreads = atoi(argv[1]);
    bufferSize = atoi(argv[3]);

    pthread_mutex_init(&lock, NULL);
	pthread_barrier_init(&bar, NULL, nThreads);
    
    long int i;
    buffer = (char *)malloc(bufferSize * sizeof(char));
    prcs = (pthread_t *)malloc(nThreads * sizeof(pthread_t));

    // Cria as threads;
    for (i = 0; i < nThreads; i++)
        pthread_create(&prcs[i], NULL, start, (void *)i);

    // Espera cada thread;
    for (int i = 0; i < nThreads; i++)
        pthread_join(prcs[i], NULL);

    // Salva a saida bruta em um arquivo de log para conferência;
    char filename_1[30] = "./logs/brute-output-";
    strcat(filename_1, argv[2]);
    FILE *file_1 = fopen(filename_1, "w");
    fprintf(file_1, "\nThreads: %s\nProcessors: %s\nBuffer size: %s\n\n", argv[1], argv[2], argv[3]);
    if (file_1 == NULL) 
        return 1;
    for (int j = 0; j < bufferSize; j++)
        fputc(buffer[j], file_1);
    fclose(file_1);

    // Conta quantas vezes diferentes o id de cada thread aparece no buffer;
    char filename_2[30] = "./logs/output-";
    strcat(filename_2, argv[2]);
    FILE *file_2 = fopen(filename_2, "w");
    fprintf(file_1, "\nThreads: %s\nProcessors: %s\nBuffer size: %s\n\n", argv[1], argv[2], argv[3]);
    int* cont = (int*) malloc(nThreads * sizeof(int));
	for (int i = 0; i < nThreads; i++)
        cont[i] = 0;
	for(int i = 0; i < bufferSize; i++){
        int idx = (int)(buffer[i] - 'A');
		if(i == 0){
            //fputc(buffer[i], file_2);
            cont[idx] += 1;
		} else if(buffer[i] != buffer[i-1]) {
            //fputc(buffer[i], file_2);
            cont[idx] += 1;
		}
	}
    //fprintf(file_2, "\n");
	for(int i = 0; i < nThreads; i++){
		//printf("%c = %d\n", i+0x41 ,cont[i]);
        fprintf(file_2, "%c = %d\n", i + 0x41, cont[i]);
	}
    fclose(file_2);

    pthread_barrier_destroy(&bar);
    pthread_mutex_destroy(&lock);
    free(buffer);
    free(prcs);
    free(cont);
    
    return 0;
}