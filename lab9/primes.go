package main

import (
    "fmt";
	"time"
)

// funcao que checka primalidade, fornecida
func is_prime(n int) bool {
	if (n<=1) {return false;}
	if (n==2) {return true;}
	if (n%2==0) {return false;} //very cheap single bit check :)
	for i:=3; i*i<=n; i+=2 {
		if (n%i==0){return false;}
	}
	return true;
}

// funcao que a rotina executará, loopa pelos numeros do range designado para ela checkando a primalidade
func primeWorker(start, end int, results chan<- int) {
    prime_count := 0
    for n := start; n <= end; n++ {
        if is_prime(n) {
            prime_count++
        }
    }
    results <- prime_count // envia a contagem de primos local para o canal de resultados
}


func main() {
	max_num := 10000000 // numero de cima do range de numeros a serem checados
	num_goroutines := 1000	// numero de goroutines a serem usadas

	// pra medir tempo da solucao concorrente
    start_time := time.Now()

	// cria um canal de resultados
	results := make(chan int, num_goroutines)

	// calcula o range de numeros que cada rotina checará
	range_size := max_num / num_goroutines

	// comeca as rotinas
	for i := 0; i < num_goroutines; i++ {
        start := i*range_size + 1
        end := (i + 1) * range_size
        if i == num_goroutines-1 {
            end = max_num // a ultima rotina cobra o que sobra de n % m
        }
        go primeWorker(start, end, results)
    }

	// recebe os resultados via canal
	totalPrimes := 0
    for i := 0; i < num_goroutines; i++ {
        totalPrimes += <-results
    }

    fmt.Printf("total prime numbers found (concurrent): %d\n", totalPrimes)
    fmt.Printf("concurrent time: %v\n", time.Since(start_time))

	// solucao sequencial pra teste de corretude
	start_time = time.Now()

	sequentialPrimes := 0
    for n := 1; n <= max_num; n++ {
        if is_prime(n) {
            sequentialPrimes++
        }
    }

    fmt.Printf("Total prime numbers found (sequential): %d\n", sequentialPrimes)
    fmt.Printf("Sequential time: %v\n", time.Since(start_time))

	// checa se os resultados batem
	if totalPrimes == sequentialPrimes {
        fmt.Println("Both solutions are correct!")
    } else {
        fmt.Println("Mismatch in prime counts!")
    }
}