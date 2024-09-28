// Com certeza, vamos supor que N = 2. Se duas threads A e B, chegarem ao if
// antes de uma de fato fazer a subtração então para ambas as threads meuSaldo
// == val, então ambas vão retirar. Apesar de não ocorrer violação de
// atomicidade, pois os locks impedem ambas de retirar ao mesmo tempo, o lock
// deveria cobrir ler e retirar como uma só coisa. Isto é, o lock poderia ser
// implementado na tarefa em si.