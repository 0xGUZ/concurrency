/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Comunicação entre threads usando variável compartilhada e exclusao
 * mutua com bloqueio */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MOD 10
#define QNT_PRINT 20

long int soma = 0;
int print_count = 0;
int turno = 0;

pthread_mutex_t mutex, mutex2;
pthread_cond_t cond;

void* ExecutaTarefa(void* arg) {
  long int id = (long int)arg;
  printf("Thread : %ld esta executando...\n", id);

  for (int i = 0; i < 100000; i++) {
    pthread_mutex_lock(&mutex);

    // Se for múltiplo de mod e ainda não imprimimos qnt_print vezes, aguarda
    // impressão
    if (soma % MOD == 0 && print_count < QNT_PRINT) {
      pthread_mutex_lock(&mutex2);
      turno = 1;  // Define que é a vez da thread de impressão
      pthread_cond_signal(&cond);         // Sinaliza a thread de impressão
      pthread_cond_wait(&cond, &mutex2);  // Aguarda a impressão
      pthread_mutex_unlock(&mutex2);
    }

    // Se for turno de soma, continua somando
    if (turno == 0) {
      soma++;
    }

    pthread_mutex_unlock(&mutex);
  }
  printf("Thread : %ld terminou!\n", id);
  pthread_exit(NULL);
}

void* extra(void* args) {
  printf("Extra : esta executando...\n");

  while (print_count < QNT_PRINT) {
    pthread_mutex_lock(&mutex2);

    // Se for turno de impressão, imprime o múltiplo
    if (turno == 0) {
      pthread_cond_wait(&cond, &mutex2);  // Aguarda a thread de soma sinalizar
    }

    // Imprime o valor de soma (múltiplo)
    printf("soma = %ld \n", soma);
    print_count++;

    // Libera o turno para a thread de soma continuar
    turno = 0;
    pthread_cond_signal(&cond);  // Sinaliza para as threads de soma continuar

    pthread_mutex_unlock(&mutex2);
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
  pthread_mutex_init(&mutex2, NULL);
  pthread_cond_init(&cond, NULL);

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

  //--finaliza
  pthread_mutex_destroy(&mutex);
  pthread_mutex_destroy(&mutex2);
  pthread_cond_destroy(&cond);

  printf("Valor de 'soma' = %ld\n", soma);

  return 0;
}
