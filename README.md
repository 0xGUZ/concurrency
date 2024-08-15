## increment_array_multithread.c

O programa recebe como argumentos:
1. O número de threads (N)
2. O tamanho do vetor  (M)

```shell
gcc increment_array_multithread.c -Wall

# 5 threads e 8 posições no vetor
./a.out 5 8
```

A política adotada para distribuição de tarefas em que `N mod M != 0` é de 1 tarefa para as `N % M` primeiras threads.