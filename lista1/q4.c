// Queremos 1, −1, 0, 2, −2, 3, −3, 4, −4

// Dado as threads
/*
int x=0; //variavel global

(0) T1:         T2:         T3:
(1) x = x-1;    x = x+1;    x = x+1;
(2) x = x+1;    x = x-1;    if(x == 1)
(3) x = x-1;                    printf("%d",x);
(4) if (x == -1)
(5)     printf("%d",x);
*/

// T3 executa até linha 3, x = 1              stdout: 1
// T1 executa duas vezes até linha 5, x = -1  stdout: 1, -1
// T3 executa ate linha 3, x = 0,
// T1 executa até linha 4, x = -1
// T3 executa até linha linha 1, x = 0
// T1 executa linha 5, x = 0                  stdout: 1, -1, 0
// T3 executa até linha 2, x = 1
// T2 executa linha 1, x = 2
// T3 executa linha 3, x = 2                  stdout: 1, -1, 0, 2
// T2 executa linha 2, x = 1
// T1 executa linha 1, x = 0
// T3 executa até linha 2, x = 1
// T1 executa linha 2, x = 2
// T2 executa linha 1, x = 3
// T3 executa linha 3, x = 3                  stdout: 1, -1, 0, 2, 3

// Para alcançar x = -3 a gente primeiro vai até x = 1 e entra no if da T3
// Agora loopamos T1 até chegar em -3 e printamos.

// Para alcançar x = 4, vamos até -1 e entramos no if da T1. Agora loopamos a T3
// até que x == 4. Então printamos. Analogamente fazemos o processo de x = -3
// para x = -4.

// É possível de resolver sempre pois temos um loop que subtrai -1 ao fim e um
// que aumenta 1 ao fim.