#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "list_int.h"
#include "timer.h"

#define QTDE_OPS 10000000  // quantidade total de operacoes sobre a lista
#define QTDE_INI 100       // quantidade de insercoes iniciais na lista
#define MAX_VALUE 100      // valor maximo a ser inserido

// lista compartilhada iniciada
struct list_node_s* head_p = NULL;
// qtde de threads no programa
int nthreads;

// variáveis de controle
int readers = 0;          // Número de leitores ativos
int writers_waiting = 0;  // Número de escritores esperando
int writer_active = 0;    // Indica se um escritor está ativo

// mutex e variáveis de condição
pthread_mutex_t mutex;
pthread_cond_t readers_cond, writers_cond;

// inicia a leitura com prioridade para escritores, isto é não se le se o
// escritor está esperando ou ativo
void start_read() {
  pthread_mutex_lock(&mutex);

  // bloqueia novos leitores se houver escritores esperando ou um escritor ativo
  while (writers_waiting > 0 || writer_active) {
    pthread_cond_wait(&readers_cond, &mutex);
  }

  readers++;  // novo leitor começa
  pthread_mutex_unlock(&mutex);
}

// função para finalizar a leitura
void end_read() {
  pthread_mutex_lock(&mutex);
  readers--;
  if (readers == 0) {
    pthread_cond_signal(
        &writers_cond);  // acorda escritores, se não houver mais leitores
  }
  pthread_mutex_unlock(&mutex);
}

// função para iniciar a escrita com prioridade
void start_write() {
  pthread_mutex_lock(&mutex);
  writers_waiting++;  // escritor entra na fila

  // espera até que todos os leitores tenham terminado e nenhum outro escritor
  // esteja ativo
  while (readers > 0 || writer_active) {
    pthread_cond_wait(&writers_cond, &mutex);
  }

  writers_waiting--;  // escritor sai da fila
  writer_active = 1;  // e aqui começa de fato
  pthread_mutex_unlock(&mutex);
}

// função para finalizar a escrita
void end_write() {
  pthread_mutex_lock(&mutex);
  writer_active = 0;  // escritor termina, pode setar 0 pq é garantido que só
                      // tem um ativo por vêz por mutex

  // preferencia de fila pra escritores
  if (writers_waiting > 0) {
    pthread_cond_signal(&writers_cond);
  } else {
    pthread_cond_broadcast(&readers_cond);  // se n, acorda todos os leitores
  }

  pthread_mutex_unlock(&mutex);
}

// tarefa das threads
void* tarefa(void* arg) {
  long int id = (long int)arg;
  int op;
  int in = 0, out = 0, read = 0;

  // realiza operacoes de consulta (98%), insercao (1%) e remocao (1%)
  for (long int i = id; i < QTDE_OPS; i += nthreads) {
    op = rand() % 100;
    if (op < 98) {
      start_read();
      Member(i % MAX_VALUE, head_p); /* Ignore return value */
      end_read();
      read++;
    } else if (op >= 98 && op < 99) {
      start_write();
      Insert(i % MAX_VALUE, &head_p); /* Ignore return value */
      end_write();
      in++;
    } else if (op >= 99) {
      start_write();
      Delete(i % MAX_VALUE, &head_p); /* Ignore return value */
      end_write();
      out++;
    }
  }

  // registra a qtde de operacoes realizadas por tipo
  printf("Thread %ld: in=%d out=%d read=%d\n", id, in, out, read);
  pthread_exit(NULL);
}

/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
  pthread_t* tid;
  double ini, fim, delta;

  // verifica se o numero de threads foi passado na linha de comando
  if (argc < 2) {
    printf("Digite: %s <numero de threads>\n", argv[0]);
    return 1;
  }
  nthreads = atoi(argv[1]);

  // insere os primeiros elementos na lista
  for (int i = 0; i < QTDE_INI; i++) {
    Insert(i % MAX_VALUE, &head_p); /* Ignore return value */
  }

  // aloca espaco de memoria para o vetor de identificadores de threads no
  // sistema
  tid = malloc(sizeof(pthread_t) * nthreads);
  if (tid == NULL) {
    printf("--ERRO: malloc()\n");
    return 2;
  }

  // Inicializa o mutex e as variáveis de condição
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&readers_cond, NULL);
  pthread_cond_init(&writers_cond, NULL);

  // tomada de tempo inicial
  GET_TIME(ini);

  // cria as threads
  for (long int i = 0; i < nthreads; i++) {
    if (pthread_create(tid + i, NULL, tarefa, (void*)i)) {
      printf("--ERRO: pthread_create()\n");
      return 3;
    }
  }

  // aguarda as threads terminarem
  for (int i = 0; i < nthreads; i++) {
    if (pthread_join(*(tid + i), NULL)) {
      printf("--ERRO: pthread_join()\n");
      return 4;
    }
  }

  // tomada de tempo final
  GET_TIME(fim);
  delta = fim - ini;
  printf("Tempo: %lf\n", delta);

  // libera os recursos
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&readers_cond);
  pthread_cond_destroy(&writers_cond);
  free(tid);
  Free_list(&head_p);

  return 0;
} /* main */
