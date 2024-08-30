#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"  // implementei meu proprio header por conta de OS

float *matriz1, *matriz2, *matriz3;
int linhas1, colunas1, linhas2, colunas2;
int num_threads;

typedef struct {
  int first;
  int last;
} t_Args;

void* MatrixMul(void* args) {
  t_Args* arg = (t_Args*)args;

  for (int i = arg->first; i < arg->last; i++) {
    for (int j = 0; j < colunas2; j++) {
      float soma = 0;
      for (int k = 0; k < colunas1; k++) {
        soma += matriz1[i * colunas1 + k] * matriz2[k * colunas2 + j];
      }
      matriz3[i * colunas2 + j] = soma;
    }
  }

  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  if (argc < 5) {
    fprintf(stderr,
            "Uso: %s <matriz1> <matriz2> <matriz_saida> <num_threads>\n",
            argv[0]);
    return EXIT_FAILURE;
  }

  double inicio, fim, delta;

  GET_TIME(inicio);

  num_threads = atoi(argv[4]);

  FILE* descritorArquivo1 = fopen(argv[1], "rb");
  if (!descritorArquivo1) {
    perror("Erro ao abrir arquivo matriz1");
    return EXIT_FAILURE;
  }

  FILE* descritorArquivo2 = fopen(argv[2], "rb");
  if (!descritorArquivo2) {
    perror("Erro ao abrir arquivo matriz2");
    fclose(descritorArquivo1);
    return EXIT_FAILURE;
  }

  fread(&linhas1, sizeof(int), 1, descritorArquivo1);
  fread(&colunas1, sizeof(int), 1, descritorArquivo1);
  matriz1 = malloc(sizeof(float) * linhas1 * colunas1);
  fread(matriz1, sizeof(float), linhas1 * colunas1, descritorArquivo1);
  fclose(descritorArquivo1);

  fread(&linhas2, sizeof(int), 1, descritorArquivo2);
  fread(&colunas2, sizeof(int), 1, descritorArquivo2);
  matriz2 = malloc(sizeof(float) * linhas2 * colunas2);
  fread(matriz2, sizeof(float), linhas2 * colunas2, descritorArquivo2);
  fclose(descritorArquivo2);

  if (colunas1 != linhas2) {
    fprintf(stderr,
            "Erro: As dimensões das matrizes não são compatíveis para "
            "multiplicação.\n");
    free(matriz1);
    free(matriz2);
    return EXIT_FAILURE;
  }

  matriz3 = malloc(sizeof(float) * linhas1 * colunas2);
  if (!matriz3) {
    perror("Erro de alocação de memória para matriz3");
    free(matriz1);
    free(matriz2);
    return EXIT_FAILURE;
  }

  if (num_threads > linhas1)
    num_threads = linhas1;

  pthread_t* tid_system = malloc(sizeof(pthread_t) * num_threads);
  t_Args* args = malloc(sizeof(t_Args) * num_threads);

  if (!tid_system || !args) {
    perror("Erro de alocação de memória para threads ou argumentos");
    free(matriz1);
    free(matriz2);
    free(matriz3);
    free(tid_system);
    free(args);
    return EXIT_FAILURE;
  }

  GET_TIME(fim);
  delta = fim - inicio;
  printf("Tempo de inicialização: %lf segundos\n", delta);

  GET_TIME(inicio);

  for (int i = 0; i < num_threads; i++) {
    args[i].first = i * (linhas1 / num_threads);
    args[i].last =
        (i == num_threads - 1) ? linhas1 : (i + 1) * (linhas1 / num_threads);
    if (pthread_create(&tid_system[i], NULL, MatrixMul, (void*)&args[i])) {
      perror("Erro ao criar thread");
      free(tid_system);
      free(args);
      free(matriz1);
      free(matriz2);
      free(matriz3);
      return EXIT_FAILURE;
    }
  }

  for (int i = 0; i < num_threads; i++) {
    pthread_join(tid_system[i], NULL);
  }

  GET_TIME(fim);
  delta = fim - inicio;
  printf("Tempo de cálculo (%d threads): %lf segundos\n", num_threads, delta);

  GET_TIME(inicio);

  FILE* descritorArquivoSaida = fopen(argv[3], "wb");
  if (!descritorArquivoSaida) {
    perror("Erro ao abrir arquivo de saída");
    free(tid_system);
    free(args);
    free(matriz1);
    free(matriz2);
    free(matriz3);
    return EXIT_FAILURE;
  }

  fwrite(&linhas1, sizeof(int), 1, descritorArquivoSaida);
  fwrite(&colunas2, sizeof(int), 1, descritorArquivoSaida);
  fwrite(matriz3, sizeof(float), linhas1 * colunas2, descritorArquivoSaida);
  fclose(descritorArquivoSaida);

  GET_TIME(fim);
  delta = fim - inicio;
  printf("Tempo de finalização: %lf segundos\n", delta);

#ifdef PRINT_STDOUT
  for (int i = 0; i < linhas1; i++) {
    for (int j = 0; j < colunas2; j++) {
      printf("%.6f ", matriz3[i * colunas2 + j]);
    }
    printf("\n");
  }
#endif

  free(matriz1);
  free(matriz2);
  free(matriz3);
  free(tid_system);
  free(args);

  return EXIT_SUCCESS;
}