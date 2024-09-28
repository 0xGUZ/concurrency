#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int num_termos;
int num_threads;

void* ConcurrentPi(void* arg) {
  long int id = (long int)arg;
  double* ret = malloc(sizeof(double));
  for (int i = id; i < num_termos; i += num_threads) {
    *ret += (((4.0 / (8.0 * i + 1.0)) - (2.0 / (8.0 * i + 4.0)) -
              (1.0 / (8.0 * i + 5.0)) - (1.0 / (8.0 * i + 6.0))) *
             (1.0 / pow(16, i)));
  }
  pthread_exit((void*)ret);
}

int main(int argc, char* argv[]) {
  double pi = 0.0;
  double* local;
  pthread_t* tid_system;

  if (argc < 3) {
    printf("missing args: %s num_termos num_threads\n", argv[0]);
    return 1;
  }

  num_termos = atoi(argv[1]);
  num_threads = atoi(argv[2]);

  tid_system = (pthread_t*)malloc(sizeof(pthread_t*) * num_threads);

  for (long int i = 0; i < num_threads; i++) {
    pthread_create(&tid_system[i], NULL, ConcurrentPi, (void*)i);
  }

  for (int i = 0; i < num_threads; i++) {
    pthread_join(tid_system[i], (void*)&local);
    pi += *local;
  }

  printf("pi com %d termos e %d threads:local_pi %f\n", num_termos, num_threads,
         pi);
  free(tid_system);
  return 0;
}