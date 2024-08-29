#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ELEM_VALUE 1000
#define PRINT_STDOUT

int main(int argc, char* argv[]) {
  float* matriz;
  int linhas, colunas;
  long long int tam;
  FILE* descritorArquivo;
  size_t ret;

  if (argc < 4) {
    fprintf(stderr,
            "-- ERRO: digite: %s <numero de fileiras> <numero de colunas>"
            "<arquivo saida>\n",
            argv[0]);
    return 1;
  }
  linhas = atoi(argv[1]);
  colunas = atoi(argv[2]);
  tam = linhas * colunas;

  matriz = (float*)malloc(sizeof(float) * tam);
  if (!matriz) {
    fprintf(stderr, "-- ERRO: alocacao da memoria da matriz falhou\n");
    return 2;
  }

  srand(time(NULL));

  for (long long int i = 0; i < tam; i++) {
    *(matriz + i) = (rand() % MAX_ELEM_VALUE) * 0.3;
  }

#ifdef PRINT_STDOUT
  fprintf(stdout, "linhas: %d, colunas: %d\n", linhas, colunas);
  for (int i = 0; i < linhas; i++) {
    for (int j = 0; j < colunas; j++) {
      fprintf(stdout, "%f ", matriz[i * colunas + j]);
    }
    fprintf(stdout, "\n");
  }
#endif

  descritorArquivo = fopen(argv[3], "wb");
  if (!descritorArquivo) {
    fprintf(stderr, "-- ERRO: abertura do arquivo falhou\n");
    return 3;
  }

  ret = fwrite(&linhas, sizeof(int), 1, descritorArquivo);
  ret = fwrite(&colunas, sizeof(int), 1, descritorArquivo);

  ret = fwrite(matriz, sizeof(float), tam, descritorArquivo);
  if (ret < tam) {
    fprintf(stderr, "-- ERRO: escrita da matriz no arquivo falhou\n");
    return 4;
  }

  fclose(descritorArquivo);
  free(matriz);
  return 0;
}
