#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ELEM_VALUE 1000
#define PRINT_STDOUT

int main(int argc, char* argv[]) {
  float *vec1, *vec2;
  long int n;
  float elem1, elem2;
  int negative_factor = 1;
  double inner_product = 0;
  FILE* file;
  size_t ret;

  if (argc < 3) {
    fprintf(stderr, "-- ERRO: digite: %s <dimensao> <arquivo saida>\n",
            argv[0]);
    return 1;
  }
  n = atoi(argv[1]);

  vec1 = (float*)malloc(sizeof(float) * n);
  if (!vec1) {
    fprintf(stderr, "-- ERRO: alocacao da memoria do primeiro vetor falhou\n");
    return 2;
  }

  vec2 = (float*)malloc(sizeof(float) * n);
  if (!vec2) {
    fprintf(stderr, "-- ERRO: alocacao da memoria do segundo vetor falhou\n");
    return 3;
  }

  srand(time(NULL));
  for (long int i = 0; i < n; i++) {
    elem1 = (rand() % MAX_ELEM_VALUE) / 3.0 * negative_factor;
    elem2 = (rand() % MAX_ELEM_VALUE) / 3.0 * negative_factor;
    vec1[i] = elem1;
    vec2[i] = elem2;
    inner_product += elem1 * elem2;
    negative_factor *= -1;
  }

#ifdef PRINT_STDOUT
  fprintf(stdout, "%ld\n", n);
  fprintf(stdout, "[");
  for (long int i = 0; i < n; i++) {
    fprintf(stdout, "%f", vec1[i]);
    if (i != n - 1) {
      fprintf(stdout, ", ");
    }
  }
  fprintf(stdout, "]\n");
  fprintf(stdout, "[");
  for (long int i = 0; i < n; i++) {
    fprintf(stdout, "%f", vec2[i]);
    if (i != n - 1) {
      fprintf(stdout, ", ");
    }
  }
  fprintf(stdout, "]\n");
  fprintf(stdout, "inner product = %lf\n", inner_product);
#endif

  file = fopen(argv[2], "wb");
  if (!file) {
    fprintf(stderr, "-- ERRO: abertura do arquivo falhou\n");
    return 4;
  }

  ret = fwrite(&n, sizeof(long int), 1, file);

  ret = fwrite(vec1, sizeof(float), n, file);
  if (ret < n) {
    fprintf(stderr, "-- ERRO: escrita do primeiro vetor no arquivo falhou\n");
    return 5;
  }

  ret = fwrite(vec2, sizeof(float), n, file);
  if (ret < n) {
    fprintf(stderr, "-- ERRO: escrita do segundo vetor no arquivo falhou\n");
    return 6;
  }

  ret = fwrite(&inner_product, sizeof(double), 1, file);

  fclose(file);
  free(vec1);
  free(vec2);
  return 0;
}