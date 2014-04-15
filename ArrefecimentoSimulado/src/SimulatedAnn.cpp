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
		cout << "Iteracao " << j << endl;
		cout << "Temperatura Atual " << temperatura << endl;
		nSucessos = 0;

		do{		
			solucaoAtual = perturbacao(solucaoAtual); // Solucao da iteracao corrente é o resultado de uma perturbacao sobre a solucaoInicial
			//solucaoAtual.calculaCustoSolucao();
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

		cout << " --------------------------------------" << endl;
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
	return pop.avioes.size() * 10000;
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

	int maxTemp = calculaTemperatura();
	int noAvioes = pop.avioes.size();
	float m = maxTemp / noAvioes;


	// Grau de Perturbacao
	int grauPerturbacao = 0;
	if     (temperatura > maxTemp -   maxTemp/3){ grauPerturbacao = 3;}
	else if(temperatura > maxTemp - 2*maxTemp/3){ grauPerturbacao = 2;}
	else { grauPerturbacao = 1;}
	//cout << "Grau : " << grauPerturbacao << endl;


	// Numero de posicoes a perturbar
	int noIteradores = floor(temperatura/m);
	if(noIteradores == 0){ noIteradores = 1;}
	cout << "Numero de pos mudadas: " << noIteradores << endl;
	// Numero de Solucoes geradas a cada iteracao é igual ao numero de avioes
	for(int i = 0; i < pop.avioes.size(); i++){
		Solucao cur(s);

		// Escolher aleatoria n avioes da Solucao a alterar a hora
		// n = temperatura / declive ( calculado antes do ciclo)
		vector<int> aleat;
			
		while(aleat.size() < noIteradores){									
			while(aleat.size() <noIteradores){
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

	/*
	// Escolher melhor das soluções geradas
	for(int i = 0; i < vizinhos.size(); i++){
		if(vizinhos.at(i).getCustoSolucao() < proximaSol.getCustoSolucao()){
			proximaSol = vizinhos.at(i);
		}
	}
	
	if(proximaSol.getCustoSolucao() < energiaSolAtual){
		cout << "Solucao Escolhida tem custo:" << proximaSol.getCustoSolucao() << endl;
		return proximaSol;
	}
	else{
		cout << "Anterior era melhor:" << energiaSolAtual << endl;
		return s;
	}
	*/
	// Escolher aleatoriamente das soluções geradas
	int itNovaSol = rand()%vizinhos.size();
	proximaSol = vizinhos.at(itNovaSol);
	cout << "Solucao Escolhida tem custo:" << proximaSol.getCustoSolucao() << endl;
	return proximaSol;
	
}