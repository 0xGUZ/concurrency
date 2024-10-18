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

# faz multiplicacao matricial com 5 threads de matrix1 e matrix2 e grava em matrix3
./mml matrix1 matrix2 matrix3 5
```

A política adotada para distribuição de tarefa é de dividir as linhas da primeira e colunas da segunda matriz entre as threads, alocando o resto para a última.

## Lab 4

-- No code

## Lab 5 

### soma-lock-atom.c

Este programa soma +1 cem mil vezes, por thread, à uma variável compartilhada "soma". Uma outra thread especial pode ser usada pra printar múltiplos de um certo número.

No programa está definido:

```c
#define MOD 10          // número base que define os múltiplos a serem printados
#define QNT_PRINT 20    // número de múltiplos a serem printados 
```

O programa recebe como argumentos:
1. Número de threads de soma

```shell
gcc lab5/soma-lock-atom.c -Wall -o soma

# 5 threads de soma
./soma 5
```

## Lista 1

Minhas respostas para a lista 1 estão inclusas nesse diretório.

## Lab 6

### my_rwlock.c

Este programa implementa - usando mutexes, condições e variáveis globais - o equivalente a um rwlock, onde a fila de espera garante prioridade à operações de escrita. 

As operações seguem uma distribuição aleatória de 98/1/1 para consulta, inserção e remoção. Essas operações executam sobre a linked list provida em `list_int.c`.

O programa recebe como argumentos:
1. Número de threads 

```shell
gcc my_rwlock.c list_int.c -o my_rwlock

# 5 threads seguindo a distribuicao aleatoria de operacoes
./my_rwlock 5
```

## Lab 6

### atividade3.c

O programa recebe como argumentos:
1. Arquivo de entrada 

```shell
gcc atividade3.c -o a

./a entrada.txt
```

Um exemplo de entrada.txt pode ser encontrado dentro do diretório /lab7