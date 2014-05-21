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

	Loader loader("Config/GenConfig.txt","Config/SimAConfig.txt");


	srand(time(NULL));
	
	

	string resultsPath = "Solucao/results.html";
	ofstream htmlFile;
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
	GenAlgoritmo a(solucoes,loader.gen_probMutacao,loader.gen_elitista, &htmlFile, "Solucao/results.csv");
	GenSolucao* best = (GenSolucao*)a.fazerIteracoes(loader.gen_numIter);

	cout << "\n\n";
	htmlFile << "<script>drawAll(\"genCanvas\",\"jsonDataGen\");</script>";
	htmlFile.close();
	
	htmlFile.open(resultsPath,ios::app);
	cout << "- A executar Arrefecimento Simulado " << endl;
	SimulatedAnnealing s = SimulatedAnnealing(loader.simA_maxIter,loader.avioes,loader.simA_maxSucessos,loader.simA_maxPerturb,loader.simA_fatorReducao,100, &htmlFile);
	cout << "\n";

	htmlFile << "<script>drawAll(\"saCanvas\",\"jsonDataSA\");</script>";
	htmlFile.close();
	
	htmlFile.open(resultsPath,ios::app);
	cout << "- A executar Branch And Bound \n" << endl;
	BNB search;
	search.solve(loader.avioes,&htmlFile);
	cout << "\n";

	htmlFile << "<script>drawAll(\"bnbCanvas\",\"jsonDataBNB\");</script>";
	htmlFile.close();
	
	

	system("PAUSE");
 }