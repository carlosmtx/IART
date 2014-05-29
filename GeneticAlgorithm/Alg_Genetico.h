
#ifndef ALG_GENETICO_H
#define ALG_GENETICO_H

#include <vector>
#include <iostream>
#include <fstream>
#include <exception>
#include <random>
#include "Cromossoma.h"

using namespace std;

class Alg_Genetico{
private:
	vector<Cromossoma*> cromossomas;
	vector<double> valores;
	bool elitista;
	int selecionadosElitista;
	double mutationProb;
	void   cicloDeVida();
	vector<Cromossoma*> obterAmostraPRepr(int n);
	vector<Cromossoma*> removerMenosFit(int n);
	ofstream csvFile;
	ofstream htmlFile;
public:
	Alg_Genetico(int seleccionadosPIteracao,bool elitista);
	Alg_Genetico();
	Alg_Genetico(vector<Cromossoma*> popInicial,double probMutacao,bool elitista);
	double obterSoma();
	Cromossoma* obterMaisBemAdaptado();
	Cromossoma* fazerIteracoes(int n);
	void printAll(int& iteration);

	/*Print*/

	
};

#endif