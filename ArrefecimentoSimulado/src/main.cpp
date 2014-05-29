#include "SimulatedAnn.h"
#include <iostream>
#include <vector>
#include <exception>
#include <time.h>
int main(){
	srand(time(NULL));

	string filename;
	cout << "Insira o nome do ficheiro: " << endl;
	cin >> filename;

	Populacao pop(filename);
	
	for(int i = 0; i < 5; i++){
		SimulatedAnnealing s = SimulatedAnnealing(3000,pop,5,5,0.98,100,Solucao()); // maxIteracoes, populacoes, maxPerturbacoes, maxSucessos, fatorReducao, janelaTemporal
		
	}
	system("pause");
	return 0;
	
}