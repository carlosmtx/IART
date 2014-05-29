#ifndef LOADER_H
#define LOADER_H


#include <string>
#include <fstream>
#include <iostream>
#include "Aviao.h"
#include <vector>
#include <sstream>

using namespace std;

class Loader{
private:
public:
	Loader(string genFile, string simAnnealingFile,string unit);
	int gen_numIter;
	int gen_populacao;
	double gen_probMutacao;
	bool gen_elitista;

	int simA_maxIter;
	int simA_maxPerturb;
	int simA_maxSucessos;
	double simA_fatorReducao;

	int unit_MaxTime;
	string filename;
	vector<Aviao> avioes;
	
};

#endif