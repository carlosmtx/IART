#include "Populacao.h"
class GeneticAlgorithm{
public:
	Populacao pop;
	int nIteracoes;
	GeneticAlgorithm(int nIteracoes):pop(20,200){
		this->nIteracoes = nIteracoes;
	}
	void setOutputFile(){
	}
	void start(){
		for ( int i = 0 ; i < nIteracoes ; i++){
			pop.reproduzir();
			cout << "Done";
		}
	}
};