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
	bool operator<(AviaoTempo& b){return tempo < b.tempo; }
};
class Solucao : public Cromossoma{
private:
	double penalizacao;
	int penalizacoes;
	double valor;
	bool sofreuMutacao;
	int indexMutacao;
	int valorAntesMutacao;
	int valorDepoisMutacao;

	int maskLower;
	int maskUpper;
	int domTemporal;
	
	vector<AviaoTempo> aterragens;
	Solucao (Solucao& sol);
	Solucao (vector<AviaoTempo> aterragens,double penalizacao,int domTemporal,int maskUpper,int maskLower);
	
	
public:
	Solucao(vector<Aviao>& avioes,double penalizacao,int domTemporal);
	Cromossoma* obterCopia();
	string toString();
	void printString();
	void gerarCromo(vector<Aviao>& avioes,int domTemporal);
	double obterValor();
	void reproduzir(Cromossoma* crom);
	void mutar();
};