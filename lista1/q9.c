#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t lock;
pthread_cond_t cond1, cond2;
int phrase_count = 0;

void* thread1() {
  while (1) {
    pthread_mutex_lock(&lock);
    switch (phrase_count) {
      case 0:
        printf("ola, voce esta acessando a variavel 'aux' agora?\n");
        phrase_count++;
        pthread_cond_signal(&cond2);
        pthread_mutex_unlock(&lock);
        break;

      case 2:
        printf("certo, entao vou altera-la, ta?\n");
        phrase_count++;
        pthread_cond_signal(&cond2);
        pthread_mutex_unlock(&lock);
        break;

      case 4:
        printf("terminei a alteracao da variavel 'aux'\n");
        phrase_count++;
        pthread_cond_signal(&cond2);
        pthread_mutex_unlock(&lock);
        pthread_exit(NULL);
        break;

      default:
        pthread_cond_wait(&cond1, &lock);
        pthread_mutex_unlock(&lock);
    }
  }
}

void* thread2() {
  while (1) {
    pthread_mutex_lock(&lock);
    switch (phrase_count) {
      case 1:
        printf("oi, não, não estou\n");
        phrase_count++;
        pthread_cond_signal(&cond1);
        pthread_mutex_unlock(&lock);
        break;

      case 3:
        printf("tudo bem\n");
        phrase_count++;
        pthread_cond_signal(&cond1);
        pthread_mutex_unlock(&lock);
        break;

      case 5:
        printf("perfeito, recebido\n");
        phrase_count++;
        pthread_cond_signal(&cond1);
        pthread_mutex_unlock(&lock);
        pthread_exit(NULL);
        break;

      default:
        pthread_cond_wait(&cond2, &lock);
        pthread_mutex_unlock(&lock);
    }
  }
}

int main(int argc, char* argv[]) {
  pthread_t* tid_system = (pthread_t*)malloc(sizeof(pthread_t) * 2);

  pthread_mutex_init(&lock, NULL);
  pthread_cond_init(&cond1, NULL);
  pthread_cond_init(&cond2, NULL);

  pthread_create(&tid_system[0], NULL, thread1, (void*)1);
  pthread_create(&tid_system[1], NULL, thread2, (void*)2);

  pthread_join(tid_system[0], NULL);
  pthread_join(tid_system[1], NULL);

  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&cond1);
  pthread_cond_destroy(&cond2);
  free(tid_system);

  return 0;
}