// Dado:
/*
 1. Ler dados da base
 2. Processar dado
 3. Escrever dado na base
*/
// O programa já se beneficiaria da concorrência pois (2.) é independente do
// acesso a memória, isso signifa que não há seção crítica em sua execução, logo
// várias threads podem processar dados diferentes ao mesmo tempo. A situação de
// (1.) e (3.) dependem do programa da base de dados. Se ele permitir acesso
// atômicos a memória então não há interferência alguma! Isso pois se leio uma
// linha no banco de dados, em seguida a processo, então quando for escrever eu
// já não a leio mais. Porém mesmo no caso em que essa atomicidade não existe,
// soluções concorrentes possibilitam o balanceio de carga. N threads poderiam
// ler os dados, enquanto N e N threads processam e escrevem. A concorrência
// garante um uso mais eficiente da CPU com mais tempo de trabalho e portanto um
// maior throughput.