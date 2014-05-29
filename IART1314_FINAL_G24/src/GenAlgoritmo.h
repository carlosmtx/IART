
#ifndef ALG_GENETICO_H
#define ALG_GENETICO_H

#include <vector>
#include <iostream>
#include <fstream>
#include <exception>
#include <random>
#include "GenCromossoma.h"

using namespace std;

class GenAlgoritmo{
private:
	vector<GenCromossoma*> cromossomas;
	vector<double> valores;
	bool elitista;
	int selecionadosElitista;
	double mutationProb;
	void   cicloDeVida();
	vector<GenCromossoma*> obterAmostraPRepr(int n);
	vector<GenCromossoma*> removerMenosFit(int n);
	ofstream csvFile;
	ofstream* htmlFile;
public:
	GenAlgoritmo(int seleccionadosPIteracao,bool elitista);
	GenAlgoritmo();
	GenAlgoritmo(vector<GenCromossoma*> popInicial,double probMutacao,bool elitista, ofstream* htmlFile, string csvPath);
	double obterSoma();
	GenCromossoma* obterMaisBemAdaptado();
	GenCromossoma* fazerIteracoes(int n);
	void printAll(int& iteration);

	/*Print*/

	
};

#endif