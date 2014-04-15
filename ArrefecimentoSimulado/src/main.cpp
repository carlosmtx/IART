#include "SimulatedAnn.h"
#include <iostream>
#include <vector>
#include <exception>
#include <time.h>
int main(){
	srand(time(NULL));

	Populacao pop;
	SimulatedAnnealing(3000,pop,3,3,0.98); // maxIteracoes, populacoes, maxPerturbacoes, maxSucessos, fatorReducao

	system("pause");
	return 0;
	
}