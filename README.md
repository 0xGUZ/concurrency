## Lab 1

### increment_array_multithread.c

O programa recebe como argumentos:
1. O número de threads (N)
2. O tamanho do vetor  (M)

```shell
gcc lab1/increment_array_multithread.c -Wall

# 5 threads e 8 posições no vetor
./a.out 5 8
```

A política adotada para distribuição de tarefas em que `N mod M != 0` é de 1 tarefa para as `N % M` primeiras threads.

## Lab 2

### gen_rand_vecs.c

O programa recebe como argumentos:
1. Dimensão do vetor (N)
2. Arquivo de saída

```shell
gcc lab2/gen_rand_vecs.c -Wall

# 10 dimensoes e arquivo de saida vecs
./a.out 10 vecs
```

### inner_product_multithread.c

O programa recebe como argumentos:
1. Arquivo de entrada
2. Número de threads (M)

```shell
gcc lab2/inner_product_multithread.c -Wall

# arquivo de entrada vecs e 5 threads
./a.out vecs 5
```

A política adotada para distribuição de tarefas em que `N mod M != 0` é de `N / M + N % M` para a última thread.

## Lab 3

### gen_rand_matrix.c

O programa recebe como argumentos:
1. Número de fileiras
2. Número de colunas 
3. Arquivo de saída

```shell
gcc lab3/gen_rand_matrix.c -Wall -o gen

# gera matriz 100x100 e grava no binario matrix1
./gen 100 100 matrix1
```

### matrix_mul_sequential.c

O programa recebe como argumentos:
1. Arquivo de entrada 1
2. Arquivo de entrada 2
3. Arquivo de saída

```shell
gcc lab3/matrix_mul_sequential.c -Wall -o seq

# faz multiplicacao matricial de matrix1 e matrix2 e grava em matrix3
./seq matrix1 matrix2 matrix3
```

### matrix_mul_multithread_by_lines.c

O programa recebe como argumentos:
1. Arquivo de entrada 1
2. Arquivo de entrada 2
3. Arquivo de saída
4. Número de threads

```shell
gcc lab3/matrix_mul_multithread_by_lines.c -Wall -o mml

# faz multiplicacao matricial multithread de matrix1 e matrix2 e grava em matrix3 usando 5 threads
./mml matrix1 matrix2 matrix3 5
```

A política adotada para distribuição de tarefa é de dividir as linhas da primeira e colunas da segunda matriz entre as threads, alocando o resto para a última.