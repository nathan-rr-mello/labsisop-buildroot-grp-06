// gcc sched_profiler.c -o run -lpthread
// taskset -c 0 ./run

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LENGTH 12000000

pthread_mutex_t lock;
pthread_barrier_t bar;

pthread_t* prcs;
char* buffer;
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
        if (ptr >= BUFFER_LENGTH){
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
    if (argc != 3) {
        printf("Usage: %s <number_of_threads> <number_of_processors>\n", argv[0]);
        return 1;
    }
    int nThreads = atoi(argv[1]);

    pthread_mutex_init(&lock, NULL);
	pthread_barrier_init(&bar, NULL, nThreads);
    
    long int i;
    buffer = (char *)malloc(BUFFER_LENGTH * sizeof(char));
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
    if (file_1 == NULL) 
        return 1;
    for (int j = 0; j < BUFFER_LENGTH; j++)
        fputc(buffer[j], file_1);
    fclose(file_1);

    // Conta quantas vezes diferentes o id de cada thread aparece no buffer;
    char filename_2[30] = "./logs/output-";
    strcat(filename_2, argv[2]);
    FILE *file_2 = fopen(filename_2, "w");
    int* cont = (int*) malloc(nThreads * sizeof(int));
	for (int i = 0; i < nThreads; i++)
        cont[i] = 0;
	for(int i = 0; i < BUFFER_LENGTH; i++){
        int idx = (int)(buffer[i] - 'A');
		if(i == 0){
			//printf("%c", buffer[i]);
            fputc(buffer[i], file_2);
            cont[idx] += 1;
		} else if(buffer[i] != buffer[i-1]) {
			//printf("%c", buffer[i]);
            fputc(buffer[i], file_2);
            cont[idx] += 1;
		}
	}
	//printf("\n");
    fprintf(file_2, "\n");
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