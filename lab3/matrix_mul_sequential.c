#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

int main(int argc, char* argv[]) {
  if (argc < 4) {
    fprintf(stderr,
            "Uso: %s <arquivo entrada 1> <arquivo entrada 2> <arquivo saida>\n",
            argv[0]);
    return 1;
  }

  double inicio, fim, delta;
  float *matriz1, *matriz2, *matriz3;
  int linhas1, colunas1, linhas2, colunas2;
  long long int tam1, tam2;
  FILE *descritorArquivo1, *descritorArquivo2, *descritorArquivoSaida;
  size_t ret;

  GET_TIME(inicio);

  descritorArquivo1 = fopen(argv[1], "rb");
  if (!descritorArquivo1) {
    perror("Erro ao abrir arquivo matriz1");
    return 2;
  }

  fread(&linhas1, sizeof(int), 1, descritorArquivo1);
  fread(&colunas1, sizeof(int), 1, descritorArquivo1);
  tam1 = linhas1 * colunas1;
  matriz1 = (float*)malloc(sizeof(float) * tam1);
  fread(matriz1, sizeof(float), tam1, descritorArquivo1);
  fclose(descritorArquivo1);

  descritorArquivo2 = fopen(argv[2], "rb");
  if (!descritorArquivo2) {
    perror("Erro ao abrir arquivo matriz2");
    free(matriz1);
    return 3;
  }

  fread(&linhas2, sizeof(int), 1, descritorArquivo2);
  fread(&colunas2, sizeof(int), 1, descritorArquivo2);
  tam2 = linhas2 * colunas2;
  matriz2 = (float*)malloc(sizeof(float) * tam2);
  fread(matriz2, sizeof(float), tam2, descritorArquivo2);
  fclose(descritorArquivo2);

  if (colunas1 != linhas2) {
    fprintf(stderr,
            "Erro: Dimensões incompatíveis para multiplicação (%dx%d) * "
            "(%dx%d)\n",
            linhas1, colunas1, linhas2, colunas2);
    free(matriz1);
    free(matriz2);
    return 4;
  }

  matriz3 = (float*)malloc(sizeof(float) * linhas1 * colunas2);

  GET_TIME(fim);
  delta = fim - inicio;
  printf("Tempo inicializacao: %lf segundos\n", delta);

  GET_TIME(inicio);

  for (int i = 0; i < linhas1; i++) {
    for (int j = 0; j < colunas2; j++) {
      matriz3[i * colunas2 + j] = 0;
      for (int k = 0; k < colunas1; k++) {
        matriz3[i * colunas2 + j] +=
            matriz1[i * colunas1 + k] * matriz2[k * colunas2 + j];
      }
    }
    }

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de cálculo: %lf segundos\n", delta);

    GET_TIME(inicio);

    descritorArquivoSaida = fopen(argv[3], "wb");
    if (!descritorArquivoSaida) {
      perror("Erro ao abrir arquivo de saída");
      free(matriz1);
      free(matriz2);
      free(matriz3);
      return 5;
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

    return 0;
}