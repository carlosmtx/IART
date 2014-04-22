#ifndef SOLUCAO_H
#define SOLUCAO_H
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <random>

using namespace std;
class AviaoAterragem{
public:
	AviaoAterragem(Aviao* a,int hAterragem):aviao(a),hAterragem(hAterragem)
	{}
	bool operator<(AviaoAterragem b){return hAterragem < b.hAterragem;}
	int getCusto(){
		return aviao->getCusto(hAterragem);
	}
	Aviao* aviao;
	int    hAterragem;
};


class Solucao{
public:
	vector<AviaoAterragem> aterragens;
	int janelaTemp;
	int custoSolucao;
	
	Solucao(){};

	Solucao(vector<Aviao>& avioes, int janelaTemp){
		aterragens.reserve(avioes.size());
		this->janelaTemp = janelaTemp;

		vector<int> randomNumbers;																	
		while(randomNumbers.size() < avioes.size()){										/*Povoacao de um vetor de numeros aleatorios n-repetidos no intervalo[0,timeDom]*/
			while(randomNumbers.size() < avioes.size()){									/*Povoar o vetor com avioes.size() numeros*/
				randomNumbers.push_back(rand()%janelaTemp);
			}
			sort(randomNumbers.begin(),randomNumbers.end());								/*Preparar para remover repetidos*/
			unique(randomNumbers.begin(),randomNumbers.end());								/*Remover repetidos*/
		}																					/*Voltar a tentar ate ter N numeros aleatorios n-repetidos*/
		shuffle(randomNumbers.begin(),randomNumbers.end(),default_random_engine(rand()));   /*Baralhar os inteiros*/

		for (int i= 0 ; i <  avioes.size() ; i++){											/*Atribuir a cada aviao uma hora de aterragem*/
			aterragens.push_back(AviaoAterragem(&avioes[i],randomNumbers[i]));
		}
		
		calculaCustoSolucao(); // atualiza "custoSolucao"
		//imprimeSolucao();
	}



	void imprimeSolucao(){
		for(int i = 0 ; i < aterragens.size() ; i++ ){
			cout << setw(10) << aterragens[i].aviao->nome;
			cout << ": Hora->" << setw(5) << aterragens[i].hAterragem;
			cout << " Custo->" << setw(5) << aterragens[i].getCusto() <<endl;
		}
		cout << endl;
	}

	int getCustoSolucao(){
		return custoSolucao;
	}


	int calculaCustoSolucao(){
		int res = 0;
		for(int i = 0 ; i < aterragens.size() ; i++ ){
			res += aterragens[i].getCusto();

			// Verificar tempo de Nao Utilizacao da Pista
			int incioAterragem = aterragens[i].hAterragem;
			int fimAterragem = incioAterragem + aterragens[i].aviao->tempoNaoUtilizacao;
			
			int j = i;
			for(j = j+1 ; j < aterragens.size(); j++){
				int inicioJ = aterragens[j].hAterragem;
				int fimJ = inicioJ + aterragens[j].aviao->tempoNaoUtilizacao;

				if(intervaloIntersecta(incioAterragem, fimAterragem, inicioJ, fimJ)){
					res+= 1500;
				}
			}

		}
		this->custoSolucao = res;
		return res;
	}


	bool intervaloIntersecta(int i1, int f1, int i2, int f2){
		if(i2 > i1 && i2 < f1){
			return true;
		}
		else if(f2 > i1 && f2 < f1){
			return true;
		}
		return false;
	}


};
#endif