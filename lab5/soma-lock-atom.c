/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Comunicação entre threads usando variável compartilhada e exclusao
 * mutua com bloqueio */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define QNT_PRINT 20

long int soma = 0;
pthread_mutex_t mutex;

void* ExecutaTarefa(void* arg) {
  long int id = (long int)arg;
  printf("Thread : %ld esta executando...\n", id);

  for (int i = 0; i < 100000; i++) {
    pthread_mutex_lock(&mutex);
    soma++;
    pthread_mutex_unlock(&mutex);
  }
  printf("Thread : %ld terminou!\n", id);
  pthread_exit(NULL);
}

void* extra(void* args) {
  printf("Extra : esta executando...\n");
  for (int i = 0; i < 10000; i++) {
    if (!(soma % 10))
      printf("soma = %ld \n", soma);
  }
  printf("Extra : terminou!\n");
  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  pthread_t* tid;
  int nthreads;

  if (argc < 2) {
    printf("Digite: %s <numero de threads>\n", argv[0]);
    return 1;
  }
  nthreads = atoi(argv[1]);

  tid = (pthread_t*)malloc(sizeof(pthread_t) * (nthreads + 1));
  if (tid == NULL) {
    puts("ERRO--malloc");
    return 2;
  }

  pthread_mutex_init(&mutex, NULL);

  //--cria thread de log
  if (pthread_create(&tid[nthreads], NULL, extra, NULL)) {
    printf("--ERRO: pthread_create()\n");
    exit(-1);
  }

  //--cria threads de soma
  for (long int t = 0; t < nthreads; t++) {
    if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void*)t)) {
      printf("--ERRO: pthread_create()\n");
      exit(-1);
    }
  }

  //--espera todas as threads terminarem
  for (int t = 0; t < nthreads + 1; t++) {
    if (pthread_join(tid[t], NULL)) {
      printf("--ERRO: pthread_join() \n");
      exit(-1);
    }
  }

  //--finaliza o mutex
  pthread_mutex_destroy(&mutex);

  printf("Valor de 'soma' = %ld\n", soma);

  return 0;
}
