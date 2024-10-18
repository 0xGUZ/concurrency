#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define N 13  // tamanho da janela definido pelo maximo 2*n+1 com n = 10

char buffer1[N];            
char buffer2[N * 5];    // esse buffer tem que ser ligeiramente maior pra comportar os '\n' 
int eofReached = 0;     // flag para quando thread 1 acabar de ler o arquivo de entrada
int dataInBuffer1 = 0;  // numero de caracteres lidos pela thread 1

// femáforos
sem_t semLeitura, semProcessamento, semImpressao;

// lê do arquivo e envia para buffer1
void *thread1(void *arg) {
    FILE *fileIn = (FILE *)arg;

    while (1) {
        sem_wait(&semLeitura);

        int charsRead = fread(buffer1, sizeof(char), N, fileIn);
        if (charsRead <= 0) {
            eofReached = 1;  // marca fim do arquivo
            sem_post(&semProcessamento);  // libera a thread 2 para encerrar que por sua vez libera a thread 3 e volta
            break;
        }

        dataInBuffer1 = charsRead;
        sem_post(&semProcessamento);
    }

    fclose(fileIn);
    pthread_exit(NULL);
}

// processa buffer1 e coloca no buffer2
void *thread2(void *arg) {
    int n = 0, i = 0, count = 0; // n é o numero de linhas que ja foram do desenho da saida, i é posicao no buffer2, count num de chars desde ultimo '\n'

    while (1) {
        sem_wait(&semProcessamento);  

        if (eofReached && dataInBuffer1 == 0) {
            sem_post(&semImpressao); // se deu EOF e buffer1 vazio libera a thread 3 pra encerrar
            break;
        }

        // processa cada caractere do buffer1
        for (int j = 0; j < dataInBuffer1; j++) {
            buffer2[i++] = buffer1[j];  // copia char de buffer1 para buffer2
            count++;

            // logica de inserção de '\n'
            if ((n < 10 && count == 2 * n + 1) || (n >= 10 && count == 10)) { // se n menor q 10 e numero de chars desde ultimo '\n' for 2n+1 add, tambem se for >= 10
                buffer2[i++] = '\n';  // adiciona nova linha
                n = (n < 10) ? n + 1 : n;  // incrementa n até 10
                count = 0;  // reinicia contagem desde ultimo '\n'
            }
        }

        buffer2[i] = '\0';  // pro printf parar de ler nesse char
        dataInBuffer1 = 0;  // marca buffer1 como vazio
        
        i=0; // reinicia posicao desse buffer

        sem_post(&semImpressao);  

    pthread_exit(NULL);
}

// imprime o buffer2
void *thread3(void *arg) {
    while (1) {
        sem_wait(&semImpressao); 

        // se EOF e buffer1 e buffer 2 vazios pode encerrar
        if (eofReached && strlen(buffer2) == 0) {
            break;
        }

        printf("%s", buffer2);  // imprime buffer2
        fflush(stdout);  // limpa buffer do stdout

        memset(buffer2, '\0', N * 5);  // zera buffer2

        sem_post(&semLeitura); 
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    FILE *fileIn = fopen(argv[1], "r");
    if (!fileIn) {
        perror("Erro ao abrir o arquivo de entrada");
        return 1;
    }

    // inicializa os semáforos
    sem_init(&semLeitura, 0, 1);       // thread 1 pode começar a ler
    sem_init(&semProcessamento, 0, 0); // thread 2 espera dados para processar
    sem_init(&semImpressao, 0, 0);     // thread 3 espera para imprimir

    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, thread1, fileIn);
    pthread_create(&tid2, NULL, thread2, NULL);
    pthread_create(&tid3, NULL, thread3, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    sem_destroy(&semLeitura);
    sem_destroy(&semProcessamento);
    sem_destroy(&semImpressao);

    return 0;
}