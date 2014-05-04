#include "GenCromossoma.h"
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
class GenSolucao : public GenCromossoma{
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
	GenSolucao (GenSolucao& sol);
	GenSolucao (vector<AviaoTempo> aterragens,double penalizacao,int domTemporal,int maskUpper,int maskLower);
	
	
public:
	GenSolucao(vector<Aviao>& avioes,double penalizacao,int domTemporal);
	GenCromossoma* obterCopia();
	string toString();
	void printString();
	void gerarCromo(vector<Aviao>& avioes,int domTemporal);
	double obterValor();
	void reproduzir(GenCromossoma* crom);
	void mutar();
};