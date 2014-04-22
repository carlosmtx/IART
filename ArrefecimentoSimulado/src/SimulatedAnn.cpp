#include "SimulatedAnn.h"

SimulatedAnnealing::SimulatedAnnealing(){}

SimulatedAnnealing::SimulatedAnnealing(int maxIteracoes, Populacao pop, int maxPerturbacoes, int maxSucessos, float fatorReducao){
	this->maxIteracoes = maxIteracoes;
	this->pop = pop;
	
	this->janelaTemporal = 100;
	this->energiaInicial = 0;

	// Ainda nao implementado
	this->maxPerturbacoes = maxPerturbacoes;
	this->maxSucessos = maxSucessos;
	this->fatorReducao = fatorReducao;


	executa();
}

void SimulatedAnnealing::executa(){
	geraEstadoInicial(); // coloca em solucaoAtual e solucaoInicial a solucao inicial
	temperatura = calculaTemperatura(); // inicia temperatura com valor dependendo do tamanho da populacao
	energiaInicial = solucaoInicial.custoSolucao;
	
	Solucao melhorSolucaoAtual(solucaoInicial);
	int melhorEnergiaAtual = melhorSolucaoAtual.getCustoSolucao();

	int i,j;
	int nSucessos;
	int contadorAux;
	int energiaAposPerturbacao;

	j = 0;
	do{
		i = 0;
		nSucessos = 0;
		contadorAux = 0;

		cout << "Iteracao " << j << endl;
		cout << "Temperatura Atual " << temperatura << endl;
		
		do{		
			contadorAux++;

			energiaInicial = solucaoInicial.custoSolucao;

			solucaoAtual = perturbacao(solucaoInicial);
			solucaoAtual.calculaCustoSolucao();
			energiaAposPerturbacao = solucaoAtual.custoSolucao;

			variacaoEnergia = energiaAposPerturbacao - energiaInicial;

			if(variacaoEnergia <= 0 || condExponenteAleatoria()){ // Solucao desta iteracao é escolhida
				solucaoInicial = solucaoAtual; // SolucaoInicial da proxima iter vai ser a obtida nesta iteracao

				if(energiaAposPerturbacao < melhorEnergiaAtual){
					melhorEnergiaAtual = energiaAposPerturbacao;
					melhorSolucaoAtual = solucaoAtual;
				}

				nSucessos++;
			}		
			i++;	
			
		}while(nSucessos < maxSucessos && i < maxPerturbacoes ); // Para quando chegar ao numero max de perturbcaoes ou ao numero max de sucessos
		
		
		temperatura = this->fatorReducao * temperatura;
		j++;

		cout << " --------------------------------------" << endl;
	}while(temperatura > 0 && j < maxIteracoes); // Para o algoritmo quando temperatura chegar a 0 (nunca lel) ou atingir o max de iters


	cout << "Fim" << endl;
	solucaoAtual.imprimeSolucao();
	cout << "Tem custo " << solucaoAtual.getCustoSolucao() << endl;
}

void SimulatedAnnealing::geraEstadoInicial(){
	solucaoInicial = Solucao(this->pop.avioes, this->janelaTemporal);
	energiaInicial = solucaoInicial.custoSolucao;
	solucaoAtual = solucaoInicial;

}

int SimulatedAnnealing::calculaTemperatura(){
	return pop.avioes.size() * 1000000;
}

bool SimulatedAnnealing::condExponenteAleatoria(){
	double e = exp(-(variacaoEnergia / temperatura));
	double r = ((double) rand() / (RAND_MAX));
	return e > r;
}

Solucao SimulatedAnnealing::perturbacao(Solucao s){
	Solucao proximaSol = s;
	int energiaSolAtual = s.getCustoSolucao();

	vector<Solucao> vizinhos;

	int noAvioes = pop.avioes.size();


	int	noIteradores = 2;
	// Numero de Solucoes geradas a cada iteracao é igual ao numero de avioes
	for(int i = 0; i < pop.avioes.size(); i++){
		Solucao cur(s);
		
		vector<int> aleat;
			
		// Aleatorios têm que ser diferentes
		while(aleat.size() < noIteradores){									
			while(aleat.size() < noIteradores){
				aleat.push_back(rand()%noAvioes);
			}
			sort(aleat.begin(),aleat.end());								
			unique(aleat.begin(),aleat.end());	
		}

		// Alterar os iteradores escolhidos
		Aviao* aux;
		for(int j = 0; j < aleat.size(); j++){
			aux = cur.aterragens.at(aleat[j]).aviao;
			int intervalo = aux->horaJanelaFim - aux->horaJanelaInicio;
			cur.aterragens.at(aleat[j]).hAterragem = rand()%intervalo + aux->horaJanelaInicio;
		}		
		
		cur.calculaCustoSolucao();
		vizinhos.push_back(cur);
		//cout << "Custo: " << cur.getCustoSolucao() << endl;
		
	}

	
	// Escolher aleatoriamente das soluções geradas
	int itNovaSol = rand()%vizinhos.size();
	proximaSol = vizinhos.at(itNovaSol);
	//cout << "Solucao Escolhida tem custo:" << proximaSol.getCustoSolucao() << endl;
	return proximaSol;
	
}