// a) Seção crítica é um trecho de código onde existe o acesso concorrente à
// recursos compartilhados. Ela deve ser protegida por exclusão mútua pra
// impedir que a seção crítica crie um problema de corrida de dados.

// b) A corrida de dados acontece quando threads diferentes leem e escrevem em
// um mesmo segmento de memória paralelamente. A escrita pode acabar modificando
// errôneamente o valor que deveria ser lido, um exemplo é: temos um contador
// global, temos uma thread que vai incrementar em um infinitamente, temos uma
// thread que printa múltiplos de 2, ela lê o contador, se ele for múltiplo de
// dois o imprime. No entanto, após a checagem da modularidade pode ser que a
// primeira thread incremente o contador, então printariamos incorretamente um
// ímpar.

// c) A violação de atomicidade ocorre quando mais de uma thread executa um
// trecho de código, que deveria ser executado por só uma, paralelamente.

// d) A violação de ordem acontece em um programa que deveria executar as
// threads em uma certa ordem específica, mas não o faz. Um exemplo seriam
// threads que jogam jogo da velha. Queremos alternar entre elas para que cada
// uma jogue exatamente uma vez, se não o fizermos uma violação de ordem
// aconteceu.

// e) Imagine uma reunião de trabalho onde para falar você deve estar segurando
// uma "pelúcia" que lhe dá esse direito. Se você deseja falar então levanta a
// mão e sinaliza a sua intenção de portar a "pelúcia", no entanto só poderas
// falar quando de fato a tiver. Essa "pelúcia" é o mutex, e os locks são as
// sinalizações de mão. Se nínguem estiver segurando o mutex e uma thread chama
// o método lock sobre este mutex, então essa thread "receberá" o mutex. Caso
// alguém o tenha então ao executar o lock você aguarda até a liberação para o
// ter. Essa técnica é fundamental para resolver os problemas de b) e c), pois
// garante que só uma thread possa interagir com um recurso compartilhado
// durante a execução da seção crítica.

// f) A sincronização condicional com bloqueio permite a coordenação entre as
// threads. Se precisamos impor uma ordem de execução, então ela é essencial. Um
// exemplo é: temos threads que leem entradas do usuário e armazenam num buffer,
// existem outras threads que consomem e esvaziam esse buffer. Quando o buffer
// encher sinalizamos a parada das leituras, para impedir o overflow
// do buffer. Quando o buffer estiver vazio exigimos que o consumo seja
// paralizado, para evitar consumo de elementos nulos. Dessa forma estabelecemos
// a coordenação entre as threads dentro delas mesmas, resolvendo problemas como
// em d) da questão da ordem de execução.

// g) Esses mecanismos, novamente, são essenciais para impedir a corrida de
// dados, a violação de atomicidade e a violação de ordem. Além disso, são
// essenciais para garantir a coordenação entre as threads, permitindo que uma
// grande variedade de tarefas seja implementada em concorrência. Podemos por
// conta disso inclusive implementar programas sequenciais usando o paralelismo,
// o que apesar de não ser muito útil indica o alcance dessas soluções.
