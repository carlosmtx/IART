#include "GenSolucao.h"
#include <math.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
#define JSON_AVIAO(nome_,hAterragem_,tempoOc_,intervSup_,intervInf_) "{\"nome\":"+'"'+nome_+'"'+",\"hAterragem\":"+to_string(hAterragem_)+ ",\"tempoOcup\":"+to_string(tempoOc_)+",\"intervSup\":"+to_string(intervSup_)+",\"intervInf\":"+to_string(intervInf_)+"}"

GenSolucao::GenSolucao(vector<AviaoTempo> aterragens,double penalizacao,int domTemporal,int maskUpper,int maskLower){
	this->aterragens =  aterragens;
	this->penalizacao = penalizacao;
	this->penalizacoes=0;
	this->domTemporal = domTemporal;
		
	sofreuMutacao=false;
	valorAntesMutacao=0;
	valorDepoisMutacao=0;
}
GenSolucao::GenSolucao(vector<Aviao>& avioes,double penalizacao,int domTemporal){
		this->penalizacao = penalizacao;
		this->domTemporal = domTemporal;
		
		int maskSize = (int)(log(domTemporal)/log(2)+1);
		int fullrange= (0x01 << (maskSize+1))-1;
		maskUpper = ((fullrange)>> (maskSize+1)/2)<<(maskSize+1)/2;
		maskLower = fullrange ^ maskUpper;

		maskUpper = maskUpper & fullrange;
		maskLower = maskLower & fullrange;

		gerarCromo(avioes,domTemporal);
	}
GenSolucao::GenSolucao(GenSolucao& sol){
	this->aterragens = sol.aterragens;
	this->domTemporal= sol.domTemporal;
	this->maskLower = sol.maskLower;
	this->maskUpper = sol.maskUpper;
	this->penalizacao = sol.penalizacao;
}
GenCromossoma* GenSolucao::obterCopia(){
	return new GenSolucao(this->aterragens,penalizacao,domTemporal,maskUpper,maskLower);
}
void GenSolucao::gerarCromo(vector<Aviao>& avioes,int domTemporal){
	aterragens.reserve(avioes.size());
	int limitSup;
	int limitInf;
	int rnd;
	for (int i= 0 ; i <  avioes.size() ; i++){
		
		limitSup = 	avioes[i].horaJanelaFim;
		limitInf =  avioes[i].horaJanelaInicio;
		rnd     =  limitInf + rand() %(limitSup - limitInf);

		aterragens.push_back(AviaoTempo(&avioes[i],rnd));
	}
}
double GenSolucao::obterValor(){
		double acc=0;																		/*Acumulador... guarda o valor final */
		double aux=0;																		/**/
		penalizacoes =0;
		for(int i = 0 ; i < aterragens.size() ; i++){										/*Obter somatorio de todos os custos*/
			aux = aterragens[i].getCusto();													/*Obter custo*/
			if (aux >= 0){acc+=aux;}														/*Se custo valido somar*/
			else{acc+=penalizacao;penalizacoes++;}											/*Se invalido adicionar penalizacao*/
		}
		vector<AviaoTempo> auxiliar = aterragens;
		sort(auxiliar.begin(),auxiliar.end());

		int hAterragem;
		int hPistaLivre;

		for ( int i = 0 ; i < auxiliar.size()-1 ; i++){
			hAterragem = auxiliar[i].tempo;
			hPistaLivre= hAterragem + auxiliar[i].aviao->tempoNaoUtilizacao;
			if ( hPistaLivre > auxiliar[i+1].tempo){acc+= penalizacao;penalizacoes++;}
		}
		valor = acc; 
		return 1/acc;
	}
void GenSolucao::reproduzir(GenCromossoma* crom){
	double temp;
	int i = aterragens.size()/2 * (rand()%2);
	int h = i == 0 ? aterragens.size()/2 : aterragens.size() ;

	for ( i ; i < h ; i++){
		temp = aterragens[i].tempo;
		aterragens[i].tempo = ((GenSolucao*)crom)->aterragens[i].tempo;
		((GenSolucao*)crom)->aterragens[i].tempo = temp;
	}
	}
void GenSolucao::mutar(){
	
	sofreuMutacao = true;

	int randPos = rand()% aterragens.size();
	int limitSup = 	aterragens[randPos].aviao->horaJanelaFim;
	int limitInf =  aterragens[randPos].aviao->horaJanelaInicio;
	int rnd      =  limitInf + rand() %(limitSup - limitInf);

	indexMutacao = randPos;
	valorAntesMutacao=aterragens[randPos].tempo;
	valorDepoisMutacao=rnd;

	aterragens[randPos].tempo = rnd; 
	
}
string GenSolucao::toString(){
	string str;
	str += "[";
	for ( int i = 0 ; i < aterragens.size() ; i++){
	 str+=((string)JSON_AVIAO(aterragens[i].aviao->nome,aterragens[i].tempo,aterragens[i].aviao->tempoNaoUtilizacao,aterragens[i].aviao->horaJanelaFim,aterragens[i].aviao->horaJanelaFim));
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
void GenSolucao::printString(){
	cout << "\n";
	cout << left<<setw(20) <<"Aviao Nome";
	cout << left<<setw(8) <<"Tempo";
	cout << left<<setw(8)  <<"Custo";
	cout << "\n";

	for ( int i = 0 ; i < aterragens.size() ; i++){
			cout << left<< setw(20) <<aterragens[i].aviao->nome;
			cout << left<< setw(8) <<std::to_string(aterragens[i].tempo);
			cout << left<< setw(8)  <<to_string((int)aterragens[i].getCusto());
			cout << endl;
	}
	
	//cout << (JSON_AVIAO("",1,2,3,4));
}