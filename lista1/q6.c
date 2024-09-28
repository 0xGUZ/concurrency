// a) Os requisitos lógicos atendidos são:
// 1. Exclusão mútua apropriada.
// Nínguem pode escrever ao mesmo tempo, não se pode ler enquanto se escreve.
// 2. Permitindo múltiplos leitores.
// A única condição que impede a entrada de um leitor é a existência de um
// escritor ativo.
// 3. Sincronização entre as threads
// O fim da leitura só precisa sinalizar para a entrada de um escritor se
// tiverem 0 leitores ativos. E ele não precisa sinalizar a entrada de leitores
// por que esses não estão esperando pra entrar.
// O fim de uma escritura precisa sinalizar tanto para leitores quanto para
// escritores.

// b) Não se pode trocar o while. A parte boa do while é que a sua condição de
// entrada é novamente verificada quando a thread desperta. Por exemplo, se
// existe um escritor ativo então a thread iniciando a leitura aguarda. Quando
// a escritura termina ela sinaliza tanto o inicio da leitura quanto da
// escritura. Vamos supor que a escritura começe primeiro, então quando a
// leitura despertar ela vai olhar novamente se existe alguma escritura ativa:
// se tiver volta a dormir.