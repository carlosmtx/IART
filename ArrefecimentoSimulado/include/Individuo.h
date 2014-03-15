#ifndef INDIVIDUO_H
#define INDIVIDUO_H
#include "Aviao.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
/*Classes : AviaoAterragem -> Par que guarda um apontador para o aviao e a hora da sua aterragem
 *			Individuo      -> Individuo da populacao: pode reproduzir-se e "mutar-se" 
 */
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


class Individuo{
public:
	vector<AviaoAterragem> aterragens;
	int domTemporal;
	int penalizacao;
	
	
	Individuo(int domTemporal,vector<Aviao>& avioes){
		aterragens.reserve(avioes.size());
		vector<int> randomNumbers;																	
		while(randomNumbers.size() < avioes.size()){										/*Povoacao de um vetor de numeros aleatorios n-repetidos no intervalo[0,timeDom]*/
			while(randomNumbers.size() < avioes.size()){									/*Povoar o vetor com avioes.size() numeros*/
				randomNumbers.push_back(rand()%domTemporal);
			}
			sort(randomNumbers.begin(),randomNumbers.end());								/*Preparar para remover repetidos*/
			unique(randomNumbers.begin(),randomNumbers.end());								/*Remover repetidos*/
		}																					/*Voltar a tentar ate ter N numeros aleatorios n-repetidos*/
		shuffle(randomNumbers.begin(),randomNumbers.end(),default_random_engine(rand()));   /*Baralhar os inteiros*/

		for (int i= 0 ; i <  avioes.size() ; i++){											/*Atribuir a cada aviao uma hora de aterragem*/
			aterragens.push_back(AviaoAterragem(&avioes[i],randomNumbers[i]));
		}
		for(int i = 0 ; i < aterragens.size() ; i++ ){
			cout << "Aviao:" << aterragens[i].aviao->nome << " Hora:" << aterragens[i].hAterragem << " Custo:" << aterragens[i].getCusto() <<endl;
		}
		cout << endl;
	}
	
	int obterValor(){
		int acc=0;
		int aux=0;
		for(int i = 0 ; i < aterragens.size() ; i++){										/*Obter somatorio de todos os custos*/
			aux = aterragens[i].getCusto();													/*Obter custo*/
			if (aux >= 0){acc+=aux;}														/*Se custo valido somar*/
			else{acc+=penalizacao;}															/*Se invalido adicionar penalizacao*/
		}
	}
	void mutar(){																			/*Gera uma mutacao no individuo*/
		bool jaMutou=false;																	/*Flag que indica se a mutacao ja ocurreu*/
		int  mutacao= 0;																	/*Valor da mutacao*/
		int  mutacaoPos=0;																	/*Aviao ao qual vai ser aplicado a mutacao*/
		if (rand()%100 == 1){																/*Se for suposto existir uma mutacao*/
			for(int j = 0 ; j < 10 && !jaMutou;j++){										/*Fazer até 10 tentativas de mutacao...abandonar se nao der*/
				mutacao = rand()%domTemporal;												/*Geracao de um valor valido p/ a mutacao*/
				mutacaoPos = rand()%aterragens.size();										/*Geracao do aviao que vai sofrer a mutacao*/
				jaMutou=true;																/*A flag e posta a true... se nao for efetuada a mutacao reverte p/ false*/
				for(int i = 0 ; i < aterragens.size() ; i++){								/*Verificacao da nao existencia de 2 avioes com a mesma hora*/
					if (aterragens[i].hAterragem == mutacao){								/*Se ja existir um aviao com a mesma hora nao ocorre mutacao*/
						jaMutou = false;
						break;
					}
					if(jaMutou)																/*Se nao existir um aviao com a mesma hora(mutacao), efetuar a mutacao*/
					{aterragens[mutacaoPos].hAterragem = mutacao;}
				}
			}
		}
	}
	
	Individuo* trocarCromos(Individuo* B){
		int a = aterragens.size()/2;

	}

};
#endif