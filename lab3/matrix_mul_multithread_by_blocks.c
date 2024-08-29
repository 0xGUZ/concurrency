#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

float *matriz1, *matriz2, *matriz3;
int linhas1, colunas1, linhas2, colunas2;
long long int tam1, tam2;
int num_threads, block_size;

typedef struct {
  int row_start;
  int col_start;
  int block_size;
} t_Args;

void* MatrixMul(void* args) {
  t_Args* arg = (t_Args*)args;
  int row_start = arg->row_start;
  int col_start = arg->col_start;
  int block_size = arg->block_size;

  for (int i = row_start; i < row_start + block_size && i < linhas1; i++) {
    for (int j = col_start; j < col_start + block_size && j < colunas2; j++) {
      matriz3[i * colunas2 + j] = 0;
      for (int k = 0; k < colunas1; k++) {
        matriz3[i * colunas2 + j] +=
            matriz1[i * colunas1 + k] * matriz2[k * colunas2 + j];
      }
    }
  }

  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  FILE *descritorArquivo1, *descritorArquivo2, *descritorArquivoSaida;
  size_t ret;
  pthread_t* tid_system;
  t_Args* args;
  double inicio, fim, delta;

  if (argc < 5) {
    fprintf(stderr,
            "Digite: %s <arquivo entrada 1> <arquivo entrada 2> <arquivo "
            "saida> <num threads>\n",
            argv[0]);
    return 1;
  }

  num_threads = atoi(argv[4]);

  // Abre Matriz 1
  descritorArquivo1 = fopen(argv[1], "rb");
  if (!descritorArquivo1) {
    fprintf(stderr, "Erro de abertura do arquivo\n");
    return 2;
  }

  // MATRIZ 1
  ret = fread(&linhas1, sizeof(int), 1, descritorArquivo1);
  if (!ret) {
    fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo\n");
    return 3;
  }
  ret = fread(&colunas1, sizeof(int), 1, descritorArquivo1);
  if (!ret) {
    fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo\n");
    return 4;
  }
  tam1 = linhas1 * colunas1;

  matriz1 = (float*)malloc(sizeof(float) * tam1);
  if (!matriz1) {
    fprintf(stderr, "Erro de alocacao da memoria da matriz\n");
    return 5;
  }

  ret = fread(matriz1, sizeof(float), tam1, descritorArquivo1);
  if (ret < tam1) {
    fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
    return 6;
  }

  fclose(descritorArquivo1);

  // Abre Matriz 2
  descritorArquivo2 = fopen(argv[2], "rb");
  if (!descritorArquivo2) {
    fprintf(stderr, "Erro de abertura do arquivo\n");
    return 7;
  }

  // MATRIZ 2
  ret = fread(&linhas2, sizeof(int), 1, descritorArquivo2);
  if (!ret) {
    fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo\n");
    return 8;
  }
  ret = fread(&colunas2, sizeof(int), 1, descritorArquivo2);
  if (!ret) {
    fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo\n");
    return 9;
  }
  tam2 = linhas2 * colunas2;

  if (colunas1 != linhas2) {
    fprintf(stderr,
            "Erro: O número de colunas da Matriz 1 deve ser igual ao número de "
            "linhas da Matriz 2\n");
    return 10;
  }

  matriz2 = (float*)malloc(sizeof(float) * tam2);
  if (!matriz2) {
    fprintf(stderr, "Erro de alocacao da memoria da matriz\n");
    return 11;
  }

  ret = fread(matriz2, sizeof(float), tam2, descritorArquivo2);
  if (ret < tam2) {
    fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
    return 12;
  }

  fclose(descritorArquivo2);

  // Matriz 3 de retorno
  matriz3 = (float*)malloc(sizeof(float) * linhas1 * colunas2);
  if (!matriz3) {
    fprintf(stderr, "Erro de alocacao da memoria da matriz\n");
    return 13;
  }

  // Calcular automaticamente o tamanho do bloco
  int num_blocks_per_dim = (int)sqrt(
      (double)num_threads);  // Divisão em blocos em ambas as dimensões
  block_size =
      linhas1 / num_blocks_per_dim;  // Tamanho do bloco em uma dimensão

  if (block_size < 1)
    block_size = 1;  // Garante que o tamanho do bloco seja pelo menos 1

  if (num_threads > (linhas1 * colunas2) / (block_size * block_size)) {
    num_threads = (linhas1 * colunas2) / (block_size * block_size);
  }

  GET_TIME(inicio);
  tid_system = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
  args = (t_Args*)malloc(sizeof(t_Args) * num_threads);

  if (tid_system == NULL || args == NULL) {
    printf("--ERRO: alocacao da memoria falhou\n");
    return 8;
  }

  int thread_id = 0;
  for (int i = 0; i < linhas1; i += block_size) {
    for (int j = 0; j < colunas2; j += block_size) {
      args[thread_id].row_start = i;
      args[thread_id].col_start = j;
      args[thread_id].block_size = block_size;

      if (pthread_create(&tid_system[thread_id], NULL, MatrixMul,
                         (void*)&args[thread_id])) {
        printf("--ERRO: pthread_create() falhou\n");
        return 9;
      }
      thread_id++;
    }
  }

  for (int i = 0; i < thread_id; i++) {
    if (pthread_join(tid_system[i], NULL)) {
      printf("--ERRO: pthread_join() falhou\n");
      return 10;
    }
  }
  GET_TIME(fim);
  delta = fim - inicio;
  printf(
      "Tempo calculo (%d threads) multiplicacao (%dx%d)*(%dx%d): %lf "
      "segundos\n",
      num_threads, linhas1, colunas1, linhas2, colunas2, delta);

  // Salva a Matriz 3 no arquivo de saída
  descritorArquivoSaida = fopen(argv[3], "wb");
  if (!descritorArquivoSaida) {
    fprintf(stderr, "Erro de abertura do arquivo de saida\n");
    return 14;
  }

  ret = fwrite(&linhas1, sizeof(int), 1, descritorArquivoSaida);
  ret = fwrite(&colunas2, sizeof(int), 1, descritorArquivoSaida);
  ret =
      fwrite(matriz3, sizeof(float), linhas1 * colunas2, descritorArquivoSaida);
  if (ret < linhas1 * colunas2) {
    fprintf(stderr, "Erro de escrita dos elementos da matriz de saida\n");
    return 15;
  }

  fclose(descritorArquivoSaida);

#ifdef PRINT_STDOUT
  // imprime a matriz na saida padrao
  for (int i = 0; i < linhas1; i++) {
    for (int j = 0; j < colunas2; j++)
      fprintf(stdout, "%.6f ", matriz3[i * colunas2 + j]);
    fprintf(stdout, "\n");
  }
#endif

  // finaliza o uso das variaveis
  free(matriz1);
  free(matriz2);
  free(matriz3);
  free(tid_system);
  free(args);

  return 0;
}
