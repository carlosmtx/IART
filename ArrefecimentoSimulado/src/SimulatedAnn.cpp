#include "SimulatedAnn.h"

SimulatedAnnealing::SimulatedAnnealing(){}

SimulatedAnnealing::SimulatedAnnealing(int maxIteracoes, Populacao pop, int maxPerturbacoes, int maxSucessos, float fatorReducao, int janelaTemporal, Solucao& solInicial){
	this->maxIteracoes = maxIteracoes;
	this->pop = pop;
	
	this->janelaTemporal = janelaTemporal;
	this->energiaInicial = 0;

	// Ainda nao implementado
	this->maxPerturbacoes = maxPerturbacoes;
	this->maxSucessos = maxSucessos;
	this->fatorReducao = fatorReducao;

	//geraEstadoInicial();
	//if(solInicial.aterragens.size() == 0){
		geraEstadoInicial();
	//}
	/*
	else{
		solucaoAtual = solInicial;
		solucaoAtual.calculaCustoSolucao();
		energiaInicial = solucaoInicial.custoSolucao;
		solucaoInicial = solucaoAtual;
	}
	*/
	executa();
}

void SimulatedAnnealing::executa(){
	
	temperatura = calculaTemperatura(); // inicia temperatura com valor dependendo do tamanho da populacao
	energiaInicial = solucaoInicial.custoSolucao;
	cout << "---------------------------" << endl;
	//cout << "Solucao Inicial Gerada. " << endl;
	cout << "Custo da Solucao Inicial: " << energiaInicial << endl;
	
	Solucao melhorSolucaoAtual(solucaoInicial);
	int melhorEnergiaAtual = melhorSolucaoAtual.getCustoSolucao();

	int i,j;
	int nSucessos;
	int contadorAux;
	int energiaAposPerturbacao;

	int itMelhor=0;
	cout << "A executar ";
	j = 0;
	do{
		i = 0;
		nSucessos = 0;
		contadorAux = 0;

		if(j%80 == 0){ 
			cout << ".";
		}
		//cout << "Iteracao " << j << endl;
		//cout << "Temperatura Atual " << temperatura << endl;
		
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
					itMelhor = j;
				}

				nSucessos++;
			}		
			i++;	
			
		}while(nSucessos < maxSucessos && i < maxPerturbacoes ); // Para quando chegar ao numero max de perturbcaoes ou ao numero max de sucessos
		
		
		temperatura = this->fatorReducao * temperatura;
		j++;

		//cout << " --------------------------------------" << endl;
	}while(temperatura > 0 && j < maxIteracoes); // Para o algoritmo quando temperatura chegar a 0 (nunca lel) ou atingir o max de iters

	//cout << endl << "Algoritmo terminou apos " << j << " iteracoes." << endl;
	cout << endl << "A melhor solucao encontrada foi: " << itMelhor << "/" << j << endl;
	//cout << "Fim" << endl;
	//solucaoAtual.imprimeSolucao();
	cout << "Tem custo [" << solucaoAtual.getCustoSolucao() << "]" << endl;
}

void SimulatedAnnealing::geraEstadoInicial(){
	solucaoInicial = Solucao(this->pop.avioes, this->janelaTemporal);
	energiaInicial = solucaoInicial.custoSolucao;
	solucaoAtual = solucaoInicial;

}

int SimulatedAnnealing::calculaTemperatura(){
	return pop.avioes.size() * this->janelaTemporal * 90000;
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


	int	noIteradores = 1;
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
		
		//vector<int> aleat;
		//aleat.push_back(rand()%noAvioes);

		// Alterar os iteradores escolhidos
		Aviao* aux;
		for(int j = 0; j < aleat.size(); j++){
			aux = cur.aterragens.at(aleat[j]).aviao;
			
			// A inserir aleatoriamente na hora preferencial
			int r = rand();
			if(r < 0.2){
				cur.aterragens.at(aleat[j]).hAterragem = aux->horaPreferencial;
			}
			else{
				int intervalo = aux->horaJanelaFim - aux->horaJanelaInicio;
				cur.aterragens.at(aleat[j]).hAterragem = rand()%intervalo + aux->horaJanelaInicio;
			}
			cur.calculaCustoSolucao();
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