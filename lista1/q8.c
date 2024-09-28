// a) O turno vai ser da que entrou primeiro visto que a thread cede o turno
// para a outra. Então a última que entrar cede pra primeira, e a última fica no
// while até a primeira sinalizar que não quer entrar mais.

// b) Ela não vai conseguir, pois somente a outra thread pode lhe conceder o
// turno. Então se uma thread tentar executar uma segunda vez ela já vai ficar
// presa até que a outra thread a libere.

// c) A thread que tentou acessar caiu no while. Isso significa que a intenção
// de executar já foi setada como 1. A thread quando sai da seção crítica seta
// sua intenção de entrar como 0, o que é suficiente para liberar a outra
// thread. Além disso, quando a thread que está saindo da seção crítica executar
// novamente ela vai dar o turno à essa thread que aguarda.

// d) Sim o código garante a exclusão mútua, isso pois a variável definidora é a
// booleana turno que caso for 0 executa a thread 0, caso for 1 executa a
// thread 1. É impossível que essa variável seja 0 e 1 ao mesmo tempo.

// e) Deadlock ocorre quando as threads podem se bloquear indefinidamente,
// impedindo qualquer progresso de ser feito. Isso é impossível no algoritmo de
// Peterson, visto que o bloqueio de uma depende da liberação da outra.
//
// Starvation acontece quando uma thread aguarda excessivamente e não existe um
// balanço entre a execução das threads, o que pode levar a tempos
// desequilibrados de bloqueio em uma ou outra. Tal evento também é impossível
// no algoritmo de Peterson dado que as threads alternam constantemente, diz-se
// então que a política de divisão é justa.
//
// Livelock é uma condição onde as threads repetidamente mudam de estado em
// resposta a outra, o que impede efetivamente o progresso de qualquer thread.
// O turno e a exclusão mútua garantem a não-ocorrência de tal evento.