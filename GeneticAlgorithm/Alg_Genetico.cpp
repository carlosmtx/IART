#include "Alg_Genetico.h"
Alg_Genetico::Alg_Genetico(vector<Cromossoma*> popInicial,int selecionadosPRonda,double probMutacao){
	this->cromossomas = popInicial;
	this->selecionados = selecionadosPRonda;
	this->mutationProb = probMutacao;
	
}
vector<Cromossoma*> Alg_Genetico::obterAmostraPRepr(int n){
	if (n > cromossomas.size()){ throw new invalid_argument("Alg_Genetico : obterAmostraPRep : valor invalido para n : n > cromossomas.size\n");}
	if (n < 1 )				   { throw new invalid_argument("Alg_Genetico : obterAmostraPRep : valor invalido para n : n < 1\n");}
	valores.resize(cromossomas.size());
	double sum =0;
	
	for(int i=0 ; i < cromossomas.size() ; i++){
		valores[i] = cromossomas[i]->obterValor();
		sum+=valores[i];
	}

	if (sum == 0 ){throw new range_error("Alg_Genetico : obterAmostraPRep : valor invalido em sum : sum == 0");}
	valores[0] = valores[0] / sum;

	for (int i = 1 ; i < cromossomas.size();i++){
		valores[i] = valores[i]/sum;
		valores[i] = valores[i-1]+valores[i];
	}
	valores.back() = 1;


	vector<Cromossoma*> selecionados;
	double aleatorio;

	for (int i = 0 ; i < n ; i++){
		aleatorio = (rand()%10000)/double(10000);
		for ( int j = 0 ; j < cromossomas.size() ; j++){
			if ( aleatorio < valores[j] ){selecionados.push_back( cromossomas[j] );break;}		
		}
	}
	return selecionados;

}
vector<Cromossoma*> Alg_Genetico::removerMenosFit(int n){
	if (n > cromossomas.size()){ throw new invalid_argument("Alg_Genetico : obterAmostraPRep : valor invalido para n : n > cromossomas.size\n");}
	if (n < 1 )				   { throw new invalid_argument("Alg_Genetico : obterAmostraPRep : valor invalido para n : n < 1\n");}
	valores.resize(cromossomas.size());
	double sum =0;
	
	for(int i=0 ; i < cromossomas.size() ; i++){
		valores[i] = 1/cromossomas[i]->obterValor();
		sum+=valores[i];
	}

	if (sum == 0 ){throw new range_error("Alg_Genetico : removerMenosFit : valor invalido em sum : sum == 0");}
	valores[0] = valores[0] / sum;

	for (int i = 1 ; i < cromossomas.size();i++){
		valores[i] = valores[i]/sum;
		valores[i] = valores[i-1]+valores[i];
	}
	valores.back() = 1;


	
	double aleatorio;
	vector<Cromossoma*> baixas;	
	for (int i = 0 ; i < n ; i++){
		aleatorio = (rand()%10000)/(double)10000;
		for ( int j = 0 ; j < cromossomas.size() ; j++){
			if ( aleatorio < valores[j] ){
				baixas.push_back( cromossomas[j] );
				cromossomas.erase(cromossomas.begin()+i);
				valores.erase(valores.begin()+i);
				double sum = 0;
				for ( int i = 0 ; i < valores.size(); i++){sum+=valores[i];}
				valores[0] = valores[0] / sum;
				for ( int i = 1 ; i < valores.size(); i++){valores[i]=valores[i]/sum;valores[i] = valores[i-1]+valores[i];}
				valores.back() = 1;
				break;
			}
		}
	}

	return baixas;
}
void Alg_Genetico::cicloDeVida(){
	vector<Cromossoma*> selecionados = obterAmostraPRepr(this->selecionados);
	vector<Cromossoma*> baixas       = removerMenosFit(this->selecionados/2);

	for ( int i =0 ; i < selecionados.size() ; i+=2){
		cromossomas.push_back(selecionados[i]->obterDescendencia(selecionados[i+1]));
		if ( (rand()%10000)/(double)10000  < mutationProb)
		{cromossomas.back()->mutar();}
	}


}
Cromossoma* Alg_Genetico::fazerIteracoes(int n){
	for ( int i = 0 ; i < n ; i++){
		cicloDeVida();
	}
	double sum=0;
	valores.resize(cromossomas.size());
	for(int i=0 ; i < cromossomas.size() ; i++){
		valores[i] = cromossomas[i]->obterValor();
		sum+=valores[i];
	}

	
	Cromossoma* best = cromossomas[0];
	for ( int i = 1 ; i < cromossomas.size() ; i++){
		if (best->obterValor() < cromossomas[i]->obterValor())
		{best = cromossomas[i];} 
	}
	return best;
}