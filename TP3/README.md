# Trabalho 3 - Laboratório de Sistemas Operacionais
##### _Eduardo Machado Martins_
##### _Bernardo Pacheco Fiorini_
##### _Nathan dos Reis Ramos de Mello_
#
#
## Introdução

Este trabalho descreve o desenvolvimento de um programa multi-thread que cria e coordena um número definido de threads para executar uma função específica de forma concorrente. Cada thread é encarregada de adicionar um caractere designado a um buffer global. Ao término da execução, é realizado o cálculo do número de preempções ocorridas para cada thread, com base no conteúdo do buffer gerado durante a execução. A aplicação recebe como parâmetros a quantidade desejada de threads a serem geradas, o número de processadores a serem utilizados na máquina para a execução do programa, e o tamanho do buffer global.

## Desenvolvimento

O programa foi desenvolvido na linguagem de programação C. Foram definidos o seguinte conjunto de variáveis globais para o programa:
```C
pthread_mutex_t lock;   // Garante que somente uma thread escreva no buffer por vez;
pthread_barrier_t bar;  // Garante que as threads iniciem todas praticamente ao mesmo tempo;

pthread_t* prcs;        // Será a lista de threads;
char* buffer;           // Será a lista que representa o buffer;
long bufferSize;        // Variável para o tamanho do buffer;
int ptr = 0;            // Ponteiro para a posição atual do buffer;
```

A função atribuída às threads foi definida da seguinte maneira: inicialmente, uma barreira é estabelecida para que cada thread inicie a execução da função praticamente ao mesmo tempo. Posteriormente, com base no índice da thread, é gerado o caractere que a representa. Com essas definições estabelecidas, dá-se início ao loop, cuja condição de término é alcançada quando o buffer atinge seu tamanho máximo. Dentro desse loop, foram incorporados semáforos para assegurar que apenas uma thread escreva no buffer por vez.
```C
void *start(void *arg){
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
        buffer[ptr] = id;
        ptr++;
        pthread_mutex_unlock(&lock);
    }
}
```

No método principal, inicia-se verificando se foram passados argumentos suficientes para a execução, salvando esses parametros em suas respectivas variáveis e inicializando o semáforo e a barreira. Além disso é alocado espaço para as listas:
```C
int main(int argc, char **argv) {
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
```

A próxima etapa é gerar as threads e apontar para a função que elas devem executar. Em seguida espera-se cada uma delas terminar a execução com o join.
```C
    for (i = 0; i < nThreads; i++)
        pthread_create(&prcs[i], NULL, start, (void *)i);

    for (int i = 0; i < nThreads; i++)
        pthread_join(prcs[i], NULL);
```

Feito isso, para armazenar os resultados, foram utilizadas duas metodologias. A primeira, para fins de depuração e conferencia é imprimir o buffer em um arquivo que fica armazenado em uma pasta de logs. Sendo assim, foi iterado sobre o buffer resultante enquanto cada caractere era impresso on arquivo:
```C
    char filename_1[30] = "./logs/brute-output-";
    strcat(filename_1, argv[2]);
    FILE *file_1 = fopen(filename_1, "w");
    fprintf(file_1, "\nThreads: %s\nProcessors: %s\nBuffer size: %s\n\n", argv[1], argv[2], argv[3]);
    if (file_1 == NULL) 
        return 1;
    for (int j = 0; j < bufferSize; j++)
        fputc(buffer[j], file_1);
    fclose(file_1);
```

A segunda forma de verificar o resultado é com a contagem de quantas vezes cada uma das threads foi trocada. Para isso outro arquivo de log é gerado, sendo esse a saída final do programa. Esse arquivo é criado iterando novamente sobre o buffer e conta a troca de caracteres:
```C
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
                cont[idx] += 1;
    		} else if(buffer[i] != buffer[i-1]) {
                cont[idx] += 1;
    		}
    	}
    	for(int i = 0; i < nThreads; i++)
            fprintf(file_2, "%c = %d\n", i + 0x41, cont[i]);
```

Por fim o probrama destroi a barreira e o semáforo e libera os espaços alocados para as listas:
```C
    pthread_barrier_destroy(&bar);
    pthread_mutex_destroy(&lock);
    free(buffer);
    free(prcs);
    free(cont);
```

Para automatizar o processo de execução e testar diversas configurações diferentes, foi criado o seguinte ShellScrip para rodar o programa:
```sh
if [ "$#" != "3" ];then 
echo "Usage: ./run.sh <number_of_threads> <number_of_processors> <buffer_size>"
exit
fi
if [[ ! "$1" =~ ^[0-9]+$ ]] || [[ ! "$2" =~ ^[0-9]+$ ]] || [[ ! "$3" =~ ^[0-9]+$ ]];then
    echo "All the arguments must be an unsigned integer"
    exit
fi
gcc sched_profiler.c -o exe -lpthread

number_of_threads=$1
number_of_processors=$2
buffer_size=$3
for ((i=1; i<=$number_of_processors; i++)); do
    processors=$(seq -s, 0 $((i - 1)))
    taskset -c $processors ./exe $number_of_threads $i $buffer_size
    echo taskset -c $processors ./exe $number_of_threads $i $buffer_size done
done
```

## Execução

Para executar o programa deve-se utilizar o seguinte comando:
```sh
./run.sh <number_of_threads> <number_of_processors> <buffer_size>
```

As saídas serão apresentadas na pasta /logs, que ja contém exemplos de execuções feitas. Pode-se observar nestas execuções que para um processador, cada thread executa seu processo por um longo período de tempo até ser trocada. Já para as execuções com mais de um processador a concorrência fica muito mais evidente. 