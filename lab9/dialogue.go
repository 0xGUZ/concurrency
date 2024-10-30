//introduz canais na aplicação Go
package main

import (
	"fmt"
)

func tarefa(str chan string) {
	var msg = ""
	
	// 1. recebe a primeira mensagem da Main
	msg = <- str
	fmt.Println(msg," (impresso pela Goroutine)\n")

	// 2. envia resposta pra main
	str <- "Oi Main, bom dia, tudo bem?" 

	// 3. recebe a segunda mensagem da Main
	msg = <- str
	fmt.Println(msg," (impresso pela Goroutine)\n")

	// 4. envia a ultima mensagem pra Main
	str <- "Certo, entendido."

	// 5. finaliza
	fmt.Println("Goroutine imprime: finalizando\n")
}

func main() {
	var msg = "" //string vazia
	
	//cria um canal de comunicacao nao-bufferizado
	str := make(chan string)

	//cria um goroutine que executará a função 'tarefa'
	go tarefa(str)

	// 1. manda primeira mensagem
	str <- "Olá, Goroutine, bom dia!"

	// 2. recebe a resposta
	msg = <-str
	fmt.Println(msg," (impresso pela Main)\n")

	// 3. manda a segunda mensagem
	str <- "Tudo bem! Vou terminar tá?"

	// 4. recebe a ultima mensagem
	msg = <-str
	fmt.Println(msg," (impresso pela Main)\n")

	fmt.Println("Main imprime: finalizando\n")
}
