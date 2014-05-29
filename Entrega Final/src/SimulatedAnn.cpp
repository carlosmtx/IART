#include "SimulatedAnn.h"

SimulatedAnnealing::SimulatedAnnealing(){}

SimulatedAnnealing::SimulatedAnnealing(int maxIteracoes, vector<Aviao> pop, int maxPerturbacoes, int maxSucessos, float fatorReducao, int janelaTemporal, ofstream* htmlFile){

	this->maxIteracoes = maxIteracoes;
	this->pop = pop;
	
	this->janelaTemporal = janelaTemporal;
	this->energiaInicial = 0;

	this->maxPerturbacoes = maxPerturbacoes;
	this->maxSucessos = maxSucessos;
	this->fatorReducao = fatorReducao;

	this->htmlFile = htmlFile;

	geraEstadoInicial();
	executa();
}

SimulatedAnnealing::SimulatedAnnealing(int maxIteracoes, vector<Aviao> pop, int maxPerturbacoes, int maxSucessos, float fatorReducao, int janelaTemporal, SimAnnSolucao& solInicial, string resultsPath){
		
	this->maxIteracoes = maxIteracoes;
	this->pop = pop;
	
	this->janelaTemporal = janelaTemporal;
	this->energiaInicial = 0;

	this->maxPerturbacoes = maxPerturbacoes;
	this->maxSucessos = maxSucessos;
	this->fatorReducao = fatorReducao;

	this->solucaoInicial = solInicial;

	executa();
}

inline void printProgressBar(int& i,int& n,time_t& tm){
	if ( i % 10 == 0 && i > 0){
			cout << "\r"; 
			cout << "[";
			for ( int j = 0 ; j < 100*i/n	 ;j+=5,cout << "-" ); 
			for ( int j = 0 ; j < 100-100*i/n;j+=5,cout << "." );
			cout << "]";
			cout << 100*i/n << "% completo";
			cout <<"->Tempo Estimado: " <<double(time(NULL)-tm)*(n-i)/double(i) << " s";
	}
}

void SimulatedAnnealing::executa(){
	
	temperatura = calculaTemperatura(); // inicia temperatura com valor dependendo do tamanho da populacao
	energiaInicial = solucaoInicial.custoSolucao;
		
	SimAnnSolucao melhorSolucaoAtual(solucaoInicial);
	int melhorEnergiaAtual = melhorSolucaoAtual.getCustoSolucao();

	int i,j;
	int nSucessos;
	int contadorAux;
	int energiaAposPerturbacao;

	int itMelhor=0;
	cout << "A executar ";
	j = 0;
	time_t tm= time(NULL);
	do{

		i = 0;
		nSucessos = 0;
		contadorAux = 0;

		if(j%20 == 0){ 
			printProgressBar(j,maxIteracoes,tm);
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


	}while(temperatura > 0 && j < maxIteracoes); // Para o algoritmo quando temperatura chegar a 0 (nunca lel) ou atingir o max de iters
	printProgressBar(maxIteracoes,maxIteracoes,tm);

	cout << endl << "A melhor solucao encontrada foi: " << itMelhor << "/" << j << endl;
	solucaoAtual.printSolucaoFinal();
	cout <<endl<<"CUSTO:"<< solucaoAtual.getCustoSolucao() << endl;

	*htmlFile <<"<div id=\"jsonDataSA\" style=\"display:none;\">" <<solucaoAtual.toString() <<"</div>";
}

void SimulatedAnnealing::geraEstadoInicial(){
	solucaoInicial = SimAnnSolucao(this->pop, this->janelaTemporal);
	energiaInicial = solucaoInicial.custoSolucao;
	solucaoAtual = solucaoInicial;

}

int SimulatedAnnealing::calculaTemperatura(){
	return pop.size() * this->janelaTemporal * 90000;
}

bool SimulatedAnnealing::condExponenteAleatoria(){
	double e = exp(-(variacaoEnergia / temperatura));
	double r = ((double) rand() / (RAND_MAX));
	return e > r;
}

SimAnnSolucao SimulatedAnnealing::perturbacao(SimAnnSolucao s){
	SimAnnSolucao proximaSol = s;
	int energiaSolAtual = s.getCustoSolucao();

	vector<SimAnnSolucao> vizinhos;

	int noAvioes = pop.size();

	int	noIteradores = 1;
	if(energiaSolAtual > 1000) { noIteradores = 3;}
	
	// Numero de Solucoes geradas a cada iteracao é igual ao numero de avioes
	for(int i = 0; i < pop.size(); i++){
		SimAnnSolucao cur(s);
		
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
			
			// A inserir aleatoriamente na hora preferencial
			int r = rand();
			if(r < 0.1){
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