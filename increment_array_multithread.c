#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int* vec;
  int startPos, endPos;
} t_Args;

void* incrementByOne(void* arg) {
  t_Args args = *(t_Args*)arg;
  for (int i = args.startPos; i <= args.endPos; i++) {
    args.vec[i]++;
  }
  free(arg);
  pthread_exit(NULL);
}

int* initVec(int vec_size) {
  return (int*)malloc(vec_size * sizeof(int));
}

int checkVec(int* vec, int vec_size, int expected_value) {
  for (int i = 0; i < vec_size; i++) {
    // printf("vec[%d]: %d\n",i,vec[i]);
    if (vec[i] != expected_value) {
      return 1;
    }
  }
  return 0;
}

int main(int argc, char* argv[]) {
  int num_threads;
  int vec_size;

  if (argc < 2) {
    printf("--ERRO: informe a quantidade de threads <%s> <num_threads>\n",
           argv[0]);
    return 1;
  } else if (argc < 3) {
    printf("--ERRO: informe o tamanho do vetor <%s> <vec_size>\n", argv[0]);
    return 2;
  }

  num_threads = atoi(argv[1]);
  pthread_t tid_sistema[num_threads];

  vec_size = atoi(argv[2]);
  int* vec = initVec(vec_size);

  for (int i = 0; i < vec_size; i++) {
    vec[i] = 2;
  }

  t_Args* args;
  int range = vec_size / num_threads;            // floors the range
  int range_remainder = vec_size % num_threads;  // remainder
  int pos = 0;

  for (int i = 0; i < num_threads; i++) {
    args = malloc(sizeof(t_Args));

    if (args == NULL) {
      printf("--ERRO: malloc()\n");
      return 3;
    }

    args->vec = vec;
    args->startPos = pos;

    // if there is a remainder, add one to the range, with this policy we
    // prioritize more tasks to the first threads
    if (range_remainder > 0) {
      pos++;
      range_remainder--;
    }

    // the -1 is because range is 1 indexed
    pos += range - 1;
    args->endPos = pos++;

    printf("--Cria a thread %d com indices {%d,%d} - %d posicoes\n", i,
           args->startPos, args->endPos, args->endPos - args->startPos + 1);
    if (pthread_create(&tid_sistema[i], NULL, incrementByOne, (void*)args)) {
      printf("--ERRO: pthread_create()\n");
      return 4;
    }
  }

  // wait for each thread conclusion
  for (int i = 0; i < num_threads; i++) {
    if (pthread_join(tid_sistema[i], NULL)) {
      printf("--ERRO: pthread_join() da thread %d\n", i);
      return 5;
    }
  }

  // check vec values
  if (checkVec(vec, vec_size, 3)) {
    printf("--ERRO: valores finais incorretos no vetor <vec>\n");
    return 6;
  }

  free(vec);
  return 0;
}