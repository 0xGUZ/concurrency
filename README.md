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