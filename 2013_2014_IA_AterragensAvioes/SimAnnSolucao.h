#ifndef SOLUCAO_H
#define SOLUCAO_H
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <random>

#include "Aviao.h"

#define JSON_AVIAO(nome_,hAterragem_,tempoOc_,intervSup_,intervInf_) "{\"nome\":"+'"'+nome_+'"'+",\"hAterragem\":"+to_string(hAterragem_)+ ",\"tempoOcup\":"+to_string(tempoOc_)+",\"intervSup\":"+to_string(intervSup_)+",\"intervInf\":"+to_string(intervInf_)+"}"


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


class SimAnnSolucao{
public:
	vector<AviaoAterragem> aterragens;
	int janelaTemp;
	int custoSolucao;

	SimAnnSolucao(){};

	SimAnnSolucao(vector<Aviao>& avioes, int janelaTemp){
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

	void printSolucaoFinal(){
		cout << "\n";
		cout << left<<setw(20) <<"Aviao Nome";
		cout << left<<setw(8) <<"Tempo";
		cout << left<<setw(8)  <<"Custo";
		cout << "\n";

		for ( int i = 0 ; i < aterragens.size() ; i++){
			cout << left<< setw(20) <<aterragens[i].aviao->nome;
			cout << left<< setw(8) <<std::to_string(aterragens[i].hAterragem);
			cout << left<< setw(8)  <<to_string((int)aterragens[i].getCusto());
			cout << endl;
		}

	}

	string toString(){
		string str;
		str += "[";
		for ( int i = 0 ; i < aterragens.size() ; i++){

			aterragens[i].aviao->nome;
			aterragens[i].aviao->tempoNaoUtilizacao;
			aterragens[i].aviao->custoAterrAntecipada;

			str+=((string)JSON_AVIAO(
				aterragens[i].aviao->nome,
				aterragens[i].hAterragem,
				aterragens[i].aviao->tempoNaoUtilizacao,
				aterragens[i].aviao->horaJanelaInicio,
				aterragens[i].aviao->horaJanelaFim));
			if ( i != aterragens.size()-1 ){str+=",";}
		}
		str += "]";
		return str;
		/*
		string str;
		str +="<table><tr>";
		for ( int i = 0 ; i < aterragens.size() ; i++){
		str+=(string)+"<td class='nomeAviao'>"+aterragens[i].aviao->nome+"</td>" +"<td class='tempoAterragem'>"+ std::to_string(aterragens[i].tempo)+"</td>"+"<td class='custoAterragem'>"+to_string((int)aterragens[i].getCusto())+"</td>";
		}
		str+="</tr></table>";
		return str;*/
	}


};
#endif