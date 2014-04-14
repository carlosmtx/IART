
#ifndef ALG_GENETICO_H
#define ALG_GENETICO_H

#include <vector>
#include <iostream>
#include <exception>
#include <random>
#include "Cromossoma.h"
 


using namespace std;

class Alg_Genetico{
private:
	vector<Cromossoma*> cromossomas;
	vector<double> valores;
	int elitista;
	int selecionados;
	double mutationProb;
	void   cicloDeVida();
	vector<Cromossoma*> obterAmostraPRepr(int n);
	vector<Cromossoma*> removerMenosFit(int n);

public:
	Alg_Genetico(int seleccionadosPIteracao,int elitista);
	
	Alg_Genetico();
	Alg_Genetico(vector<Cromossoma*> popInicial,int selecionadosPRonda,double probMutacao);

	Cromossoma* fazerIteracoes(int n);	
	
};

#endif