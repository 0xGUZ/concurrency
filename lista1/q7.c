#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 10
#define PRODUCERS 2
#define CONSUMERS 5

typedef struct Buffer {
  int* arr;
  int length;
} Buffer;

Buffer* buffer;
int encheu_counter = 0;
pthread_mutex_t lock;
pthread_cond_t prod, cons;

void printBuffer() {
  printf("buffer: [");
  for (int i = 0; i < buffer->length; i++) {
    if (i == buffer->length - 1) {
      printf("%d", buffer->arr[i]);
      break;
    }
    printf("%d,", buffer->arr[i]);
  }
  printf("]\n");
}

void* insere(void* arg) {
  pthread_mutex_lock(&lock);
  // espera o buffer esvaziar
  while (buffer->length > 0) {
    pthread_cond_wait(&prod, &lock);
  }
  // preenche o buffer todo
  for (int i = 0; i < BUFFER_SIZE; i++) {
    buffer->arr[i] = 1;
    buffer->length++;
  }
  encheu_counter++;
  printf("ENCHEU   ");
  // printa o buffer
  printBuffer();
  // sinaliza os consumidores
  pthread_cond_broadcast(&cons);
  pthread_mutex_unlock(&lock);
  pthread_exit(NULL);
}

void* retira(void* arg) {
  while (encheu_counter < PRODUCERS || buffer->length > 0) {
    pthread_mutex_lock(&lock);
    // espera o buffer estar cheio
    while (buffer->length == 0 && encheu_counter < PRODUCERS) {
      pthread_cond_wait(&cons, &lock);
    }

    // consome um elemento
    buffer->arr[buffer->length - 1] = 0;
    buffer->length--;

    printf("CONSUMIU ");
    // printa o buffer
    printBuffer();

    if (buffer->length == 0) {
      pthread_cond_broadcast(&prod);
    }

    pthread_mutex_unlock(&lock);
  }
  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  buffer = malloc(sizeof(Buffer));
  buffer->arr = (int*)malloc(sizeof(int) * BUFFER_SIZE);
  buffer->length = 0;  // init empty

  pthread_t* tid_system;

  tid_system = (pthread_t*)malloc(sizeof(pthread_t) * (PRODUCERS + CONSUMERS));

  pthread_mutex_init(&lock, NULL);
  pthread_cond_init(&prod, NULL);
  pthread_cond_init(&cons, NULL);

  for (long int i = 0; i < PRODUCERS; i++) {
    pthread_create(&tid_system[i], NULL, insere, (void*)i);
  }

  for (long int i = PRODUCERS; i < PRODUCERS + CONSUMERS; i++) {
    pthread_create(&tid_system[i], NULL, retira, (void*)i);
  }

  for (long int i = 0; i < PRODUCERS + CONSUMERS; i++) {
    pthread_join(tid_system[i], NULL);
  }

  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&prod);
  pthread_cond_destroy(&cons);

  free(buffer->arr);
  free(buffer);
  free(tid_system);
  return 0;
}