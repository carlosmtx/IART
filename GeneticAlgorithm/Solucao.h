#include "Cromossoma.h"
#include "Aviao.h"
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

class AviaoTempo{
public:
	int tempo;
	Aviao* aviao;


	AviaoTempo(Aviao* aviao , int tempo){
		this->aviao = aviao;
		this->tempo = tempo;
	}
	double getCusto(){
		return aviao->getCusto(tempo);
	}


};
class Solucao : public Cromossoma{
	vector<AviaoTempo> aterragens;
	double penalizacao;
	int domTemporal;
	double valor;
public:
	Solucao (vector<AviaoTempo> aterragens,double penalizacao,int domTemporal){
		this->aterragens =  aterragens;
		this->penalizacao = penalizacao;
		this->domTemporal = domTemporal;
	}
	Solucao(vector<Aviao>& avioes,double penalizacao,int domTemporal){
		this->penalizacao = penalizacao;
		this->domTemporal = domTemporal;
		gerarCromo(avioes,domTemporal);
	}
	void gerarCromo(vector<Aviao>& avioes,int domTemporal){
		aterragens.reserve(avioes.size());
		
		for (int i= 0 ; i <  avioes.size() ; i++){											/*Atribuir a cada aviao uma hora de aterragem*/
			aterragens.push_back(AviaoTempo(&avioes[i],avioes[i].horaJanelaInicio+rand()%(avioes[i].horaJanelaFim-avioes[i].horaJanelaInicio)));
		}

	}
	double obterValor(){
		double acc=0;
		double aux=0;
		
		vector<int> repetidos;
		
		for(int i = 0 ; i < aterragens.size() ; i++){										/*Obter somatorio de todos os custos*/
			repetidos.push_back(aterragens[i].tempo);
			aux = aterragens[i].getCusto();													/*Obter custo*/
			if (aux >= 0){acc+=aux;}														/*Se custo valido somar*/
			else{acc+=penalizacao;}															/*Se invalido adicionar penalizacao*/
		}
		sort(repetidos.begin(),repetidos.end());
		for ( int i = 0 ; i < repetidos.size()-1 ; i++){
			if ( repetidos[i] == repetidos[i+1]){acc+= penalizacao;}
		}
		this->valor =  1/acc;
		return 1/acc;
	}
	Cromossoma* obterDescendencia(Cromossoma* crom){
		vector<AviaoTempo> temp = this->aterragens;
		for ( int i = 0 ; i < temp.size() ; i++){
			int ini = temp[i].tempo;
			int sec = ((Solucao*)crom)->aterragens[i].tempo;
			int first = temp[i].tempo & 0x07;
			int last  = ((Solucao*)crom)->aterragens[i].tempo & 0x18;
			int result = first | last;
			temp[i].tempo = result;
		}
		return new Solucao(temp,penalizacao,domTemporal);
	}
	void mutar(){
		int randPos = rand()% aterragens.size();
		aterragens[randPos].tempo = (aterragens[randPos].tempo & rand())%domTemporal; 
	}
};