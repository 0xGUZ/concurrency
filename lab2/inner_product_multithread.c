#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

long int n;
float *vec1, *vec2;
int num_threads;

void* InnerProduct(void* tid) {
  long int id = (long int)tid;
  int first, last, block;
  double local_inner = 0, *ret;

  // division policy:
  // every thread: takes n / num_threads
  // last thread: takes remaining n % num_threads
  block = n / num_threads;
  first = id * block;
  last = first + block;
  if (id == (num_threads - 1)) {
    last = n;
  }

  for (int i = first; i < last; i++) {
    local_inner += vec1[i] * vec2[i];
  }

  ret = malloc(sizeof(double));
  if (ret != NULL)
    *ret = local_inner;
  else
    printf("--ERRO: malloc() thread\n");
  pthread_exit((void*)ret);
}

int main(int argc, char* argv[]) {
  FILE* file;
  size_t ret;

  double file_inner;
  double global_inner;
  double* local_inner;

  pthread_t* tid_system;

  if (argc < 3) {
    printf("-- ERRO: digite: %s <arquivo de entrada> <numero de threads> \n",
           argv[0]);
    return 1;
  }

  file = fopen(argv[1], "rb");
  if (file == NULL) {
    printf("--ERRO: leitura do arquivo falhou\n");
    return 2;
  }

  ret = fread(&n, sizeof(long int), 1, file);
  if (!ret) {
    fprintf(stderr, "--ERRO: leitura da dimensao dos vetores falhou\n");
    return 3;
  }

  vec1 = (float*)malloc(sizeof(float) * n);
  if (!vec1) {
    fprintf(stderr, "-- ERRO: alocacao da memoria do primeiro vetor falhou\n");
    return 4;
  }

  ret = fread(vec1, sizeof(float), n, file);
  if (ret < n) {
    fprintf(stderr, "-- ERRO: leitura do primeiro vetor do arquivo falhou");
    return 5;
  }

  vec2 = (float*)malloc(sizeof(float) * n);
  if (!vec2) {
    fprintf(stderr, "-- ERRO: alocacao da memoria do segundo vetor falhou\n");
    return 6;
  }

  ret = fread(vec2, sizeof(float), n, file);
  if (ret < n) {
    fprintf(stderr, "-- ERRO: leitura do segundo vetor do arquivo falhou");
    return 7;
  }

  num_threads = atoi(argv[2]);
  if (num_threads > n)
    num_threads = n;

  tid_system = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
  if (tid_system == NULL) {
    printf("--ERRO: alocacao da memoria dos ids das threads falhou\n");
    return 8;
  }

  for (long int i = 0; i < num_threads; i++) {
    if (pthread_create(&tid_system[i], NULL, InnerProduct, (void*)i)) {
      printf("--ERRO: pthread_create() falhou\n");
      return 9;
    }
  }

  global_inner = 0;
  for (int i = 0; i < num_threads; i++) {
    if (pthread_join(tid_system[i], (void*)&local_inner)) {
      printf("--ERRO: pthread_join() falhou\n");
      return 10;
    }
    global_inner += *local_inner;
    free(local_inner);
  }

  printf("inner_concorrente    = %.26lf\n", global_inner);

  ret = fread(&file_inner, sizeof(double), 1, file);
  printf("file_inner           = %.26lf\n", file_inner);

  printf("e = %lf\n", ((file_inner - global_inner) / file_inner));

#ifdef PRINT_SIZES
  printf("sizeof(void)     = %ld\n", sizeof(void));
  printf("sizeof(void*)    = %ld\n", sizeof(void*));
  printf("sizeof(float)    = %ld\n", sizeof(float));
  printf("sizeof(double)   = %ld\n", sizeof(double));
  printf("sizeof(int)      = %ld\n", sizeof(int));
  printf("sizeof(long int) = %ld\n", sizeof(long int));
#endif

  free(vec1);
  free(vec2);
  free(tid_system);
  fclose(file);
  return 0;
}
