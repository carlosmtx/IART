#include "SimulatedAnn.h"

SimulatedAnnealing::SimulatedAnnealing(){}

SimulatedAnnealing::SimulatedAnnealing(int maxIteracoes, Populacao pop, int maxPerturbacoes, int maxSucessos, float fatorReducao){
	this->maxIteracoes = maxIteracoes;
	this->pop = pop;
	
	this->janelaTemporal = 100;
	this->energiaAtual = 0;

	// Ainda nao implementado
	this->maxPerturbacoes = maxPerturbacoes;
	this->maxSucessos = maxSucessos;
	this->fatorReducao = fatorReducao;


	executa();
}

void SimulatedAnnealing::executa(){
	geraEstadoInicial(); // coloca em solucaoAtual e solucaoInicial a solucao inicial
	temperatura = calculaTemperatura(); // inicia temperatura com valor dependendo do tamanho da populacao
	tempInicial = temperatura;

	int j = 0;
	int i = 0;
	int nSucessos;
	int energiaAposPerturbacao = 0;
	do{
		//cout << "Ciclo Exterior: Iteracao " << j << endl;
		nSucessos = 0;

		do{
			cout << "[" << j << "]" << " - " << i << endl;
			
			solucaoAtual = perturbacao(solucaoInicial); // Solucao da iteracao corrente é o resultado de uma perturbacao sobre a solucaoInicial

			energiaAposPerturbacao = solucaoAtual.custoSolucao;
			variacaoEnergia = energiaAposPerturbacao - energiaAtual;

			if(variacaoEnergia <= 0 || condExponenteAleatoria()){ // Solucao desta iteracao é escolhida
				solucaoInicial = solucaoAtual; // SolucaoInicial da proxima iter vai ser a obtida nesta iteracao
				nSucessos++;
			}
			
			i++;
		}while(nSucessos < maxSucessos && i < maxPerturbacoes ); // Para quando chegar ao numero max de perturbcaoes ou ao numero max de sucessos

		temperatura = this->fatorReducao * temperatura;
		j++;
	}while(temperatura > 0 && j < maxIteracoes); // Para o algoritmo quando temperatura chegar a 0 (nunca lel) ou atingir o max de iters


	cout << "Fim" << endl;
	solucaoAtual.imprimeSolucao();
}

void SimulatedAnnealing::geraEstadoInicial(){
	solucaoInicial = Solucao(this->pop.avioes, this->janelaTemporal);
	energiaAtual = solucaoInicial.custoSolucao;
	solucaoAtual = solucaoInicial;

}

int SimulatedAnnealing::calculaTemperatura(){
	return pop.avioes.size() * 2000;
}

bool SimulatedAnnealing::condExponenteAleatoria(){
	double e = exp(-(variacaoEnergia / temperatura));
	double r = ((double) rand() / (RAND_MAX));
	
	return e > r;
}

Solucao SimulatedAnnealing::perturbacao(Solucao s){
	// fwieongw
	s.calculaCustoSolucao();
	return s;
}