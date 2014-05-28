#include "GenAlgoritmo.h"
#include "GenSolucao.h"
#include "GenCromossoma.h"
#include "UniCost.h"
#include "SimulatedAnn.h"
#include <fstream>
#include <iostream>
#include <time.h>
#include <Windows.h>
#include <sstream>
#include "Loader.h"

int main(){

	Loader loader("Config/GenConfig.txt","Config/SimAConfig.txt","Config/UnitCost.txt");


	srand(time(NULL));
	
	

	string resultsPath = "Solucao/"+loader.filename+"_results.html";
	string resultsTimesPath="Solucao/"+loader.filename+"_time.csv";
	time_t timeBeg;

	ofstream htmlFile;
	ofstream timeFile;
	htmlFile.open(resultsPath,ios::out);
	htmlFile <<(string)" <head>"+
			"<script src=\"results.js\"></script>"+
			"</head>"+
			"<canvas id=\"genCanvas\" width=\"10000\" height=\"200\">"+
			"</canvas>" +
			"<canvas id=\"saCanvas\" width=\"10000\" height=\"200\">"+
			"</canvas>" +
			"<canvas id=\"bnbCanvas\" width=\"10000\" height=\"200\">"+
			"</canvas>";
	
	
	cout << "\n- A executar Algoritmo Genetico " << endl;
	vector<GenCromossoma*> solucoes;
	for ( int i = 0 ; i< loader.gen_populacao ; i++){
		solucoes.push_back(new GenSolucao(loader.avioes,1000));
	}
	timeFile.open(resultsTimesPath);
	GenAlgoritmo a(solucoes,loader.gen_probMutacao,loader.gen_elitista, &htmlFile, "Solucao/"+loader.filename+"_results.csv");
	timeBeg = time(NULL);
	GenSolucao* best = (GenSolucao*)a.fazerIteracoes(loader.gen_numIter);
	timeFile << "Genetico;" << time(NULL)-timeBeg<<endl;
	cout << "\n\n";
	htmlFile << "<script>drawAll(\"genCanvas\",\"jsonDataGen\");</script>";
	htmlFile.close();
	
	htmlFile.open(resultsPath,ios::app);
	cout << "- A executar Arrefecimento Simulado " << endl;
	timeBeg = time(NULL);
	SimulatedAnnealing s = SimulatedAnnealing(loader.simA_maxIter,loader.avioes,loader.simA_maxSucessos,loader.simA_maxPerturb,loader.simA_fatorReducao,100, &htmlFile);
	timeFile << "SimAn;" << time(NULL)-timeBeg<<endl;
	cout << "\n";

	htmlFile << "<script>drawAll(\"saCanvas\",\"jsonDataSA\");</script>";
	htmlFile.close();
	
	htmlFile.open(resultsPath,ios::app);
	cout << "- A executar Unit Cost \n" << endl;
	BNB search;
	timeBeg = time(NULL);
	search.solve(loader.unit_MaxTime,loader.avioes,&htmlFile);
	timeFile << "UnitCost;" << time(NULL)-timeBeg<<endl;
	cout << "\n";

	htmlFile << "<script>drawAll(\"bnbCanvas\",\"jsonDataBNB\");</script>";
	htmlFile.close();
	
	timeFile.close();

	system("PAUSE");
 }