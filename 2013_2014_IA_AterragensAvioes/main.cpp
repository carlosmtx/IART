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

int main(){
	vector<Aviao> avioes;
	srand(time(NULL));
	ifstream infile;

	string filename;
	cout << "Insira o nome do ficheiro: " << endl;
	cin >> filename;
	infile.open(filename);

	string line;
	string nomeAviao;
	int horaPref, janI, janF, custAnt, custRet, repouso;

	getline(infile, line);
	try{
		while (getline(infile, line))
		{
			 
			string buf;
			stringstream ss(line);
			vector<string> tokens;
			while (ss >> buf)
				tokens.push_back(buf);

			nomeAviao = tokens[0];
			horaPref = atoi(tokens[1].c_str());
			janI = atoi(tokens[2].c_str());
			janF = atoi(tokens[3].c_str());
			custAnt = atoi(tokens[4].c_str());
			custRet = atoi(tokens[5].c_str());
			repouso = atoi(tokens[6].c_str());
			
			avioes.push_back(Aviao(nomeAviao, horaPref, janI, janF, custAnt, custRet, repouso));
		}
		cout << "Avioes carregados com sucesso\n";

	}catch(ErroInput& e){
		cout << e.erro << endl;
	}
	

	/*
	try{
		/*
		// Melhor teste para BNB
		avioes.push_back(Aviao("1", 2, 0, 3, 2, 4, 4));
		avioes.push_back(Aviao("2", 3, 3, 10, 3, 1, 2));
		avioes.push_back(Aviao("3", 25, 20, 40, 7, 2, 3));
		avioes.push_back(Aviao("4", 35, 30, 40, 2, 1, 5));
		avioes.push_back(Aviao("5", 42, 25, 50, 4, 8, 6));
		*/
		
		/*
		// Teste sem ficheiro
		avioes.push_back(Aviao("TAP-V0001"		,11,0 ,30,2 ,4 ,5 ));
		avioes.push_back(Aviao("TAP-V0002"		,22,20,25,3 ,5 ,5 ));
		avioes.push_back(Aviao("RyanAir-V003"	,33,30,45,4 ,6 ,5 ));
		avioes.push_back(Aviao("RyanAir-V004"	,44,40,65,5 ,7 ,5 ));
		avioes.push_back(Aviao("Lufthansa-V005"	,55,40,75,6 ,8 ,5 ));
		avioes.push_back(Aviao("Lufthansa-V006"	,66,50,95,7 ,9 ,5 ));
		avioes.push_back(Aviao("TAP-V0007"		,77,20,95,7 ,8 ,5 ));
		avioes.push_back(Aviao("JavardAIRV-0008",88,54,95,7 ,7 ,5 ));	
		avioes.push_back(Aviao("AeroSol-V0009"	,98,80,99,7 ,6 ,5 ));	
		avioes.push_back(Aviao("CansAir-V0010"	,13,1 ,95,7 ,5 ,5 ));	
		avioes.push_back(Aviao("NadAir-V0011"	,23,13,45,7 ,4 ,5 ));	
		avioes.push_back(Aviao("SaltAir-V0012"	,33,10,40,7 ,3 ,5 ));	
		avioes.push_back(Aviao("CantAir-V0013"	,42,24,49,7 ,2 ,5 ));
		avioes.push_back(Aviao("CantAir-V0014"	,52,24,60,7 ,2 ,5 ));	
		avioes.push_back(Aviao("CantAir-V0015"	,22,20,49,7 ,2 ,5 ));	
		avioes.push_back(Aviao("CantAir-V0016"	,32,18,49,7 ,2 ,5 ));	
		avioes.push_back(Aviao("CantAir-V0017"	,42,24,49,7 ,2 ,5 ));	
		avioes.push_back(Aviao("CantAir-V0018"	,82,24,83,7 ,2 ,5 ));	
		
	}
	catch(ErroInput& e){
		cout << e.erro << endl;
	}
	*/

	string resultsPath = "Solucao/results.html";
	ofstream htmlFile;
	htmlFile.open(resultsPath,ios::out);
	htmlFile <<(string)" <head>"+
			"<script src=\"results.js\"></script>"+
			"</head>"+
			"<canvas id=\"genCanvas\" width=\"10000\" height=\"100\">"+
			"</canvas>" +
			"<canvas id=\"saCanvas\" width=\"10000\" height=\"100\">"+
			"</canvas>" +
			"<canvas id=\"bnbCanvas\" width=\"10000\" height=\"100\">"+
			"</canvas>";
	

	cout << "- A executar Algoritmo Genetico " << endl;
	vector<GenCromossoma*> solucoes;
	for ( int i = 0 ; i< 50 ; i++){
		solucoes.push_back(new GenSolucao(avioes,1000,100));
	}
	GenAlgoritmo a(solucoes,0.1,true, &htmlFile, "Solucao/results.csv");
	GenSolucao* best = (GenSolucao*)a.fazerIteracoes(1000);

	cout << "\n";
	
	cout << "- A executar Arrefecimento Simulado " << endl;
	SimulatedAnnealing s = SimulatedAnnealing(2000,avioes,5,5,0.98,100, &htmlFile);
	cout << "\n";

	htmlFile << "<script>drawAll(\"genCanvas\",\"jsonDataGen\");</script>";
	htmlFile << "<script>drawAll(\"saCanvas\",\"jsonDataSA\");</script>";
	htmlFile << "<script>drawAll(\"bnbCanvas\",\"jsonDataBNB\");</script>";


	htmlFile.close();
	
	/*
	cout << "- A executar Branch And Bound " << endl;
	BNB search;
	search.solve(avioes);
	
	*/
	

	system("PAUSE");
 }