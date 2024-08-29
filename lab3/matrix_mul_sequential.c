#include <stdio.h>
#include <stdlib.h>

#define PRINT_STDOUT

int main(int argc, char* argv[]) {
  float *matriz1, *matriz2, *matriz3;
  int linhas1, colunas1, linhas2, colunas2;
  long long int tam1, tam2;
  FILE *descritorArquivo1, *descritorArquivo2, *descritorArquivoSaida;
  size_t ret;

  if (argc < 4) {
    fprintf(
        stderr,
        "Digite: %s <arquivo entrada 1> <arquivo entrada 2> <arquivo saida>\n",
        argv[0]);
    return 1;
  }

  // Abre Matriz 1
  descritorArquivo1 = fopen(argv[1], "rb");
  if (!descritorArquivo1) {
    fprintf(stderr, "Erro de abertura do arquivo\n");
    return 2;
  }

  // MATRIZ 1
  ret = fread(&linhas1, sizeof(int), 1, descritorArquivo1);
  if (!ret) {
    fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
    return 3;
  }
  ret = fread(&colunas1, sizeof(int), 1, descritorArquivo1);
  if (!ret) {
    fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
    return 4;
  }
  tam1 = linhas1 * colunas1;

  matriz1 = (float*)malloc(sizeof(float) * tam1);
  if (!matriz1) {
    fprintf(stderr, "Erro de alocao da memoria da matriz\n");
    return 5;
  }

  ret = fread(matriz1, sizeof(float), tam1, descritorArquivo1);
  if (ret < tam1) {
    fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
    return 6;
  }

  // Fecha Matriz 1
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
    fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
    return 8;
  }
  ret = fread(&colunas2, sizeof(int), 1, descritorArquivo2);
  if (!ret) {
    fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
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
    fprintf(stderr, "Erro de alocao da memoria da matriz\n");
    return 11;
  }

  ret = fread(matriz2, sizeof(float), tam2, descritorArquivo2);
  if (ret < tam2) {
    fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
    return 12;
  }

  // Fecha Matriz 2
  fclose(descritorArquivo2);

  // Matriz 3 de retorno
  matriz3 = (float*)malloc(sizeof(float) * linhas1 * colunas2);
  if (!matriz3) {
    fprintf(stderr, "Erro de alocao da memoria da matriz\n");
    return 13;
  }

  for (int i = 0; i < linhas1; i++) {
    for (int j = 0; j < colunas2; j++) {
      matriz3[i * colunas2 + j] = 0;
      for (int k = 0; k < colunas1; k++) {
        matriz3[i * colunas2 + j] +=
            matriz1[i * colunas1 + k] * matriz2[k * colunas2 + j];
      }
    }
  }

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

#ifndef PRINT_STDOUT
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
  return 0;
}