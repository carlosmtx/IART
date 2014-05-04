#include "GenAlgoritmo.h"
#include "GenSolucao.h"
#include "GenCromossoma.h"
#include <fstream>
#include <iostream>
#include <time.h>
#include <Windows.h>
int main(){
	vector<Aviao> avioes;
	srand(time(NULL));
	
	try{
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
	vector<GenCromossoma*> solucoes;
	for ( int i = 0 ; i< 50 ; i++){
		solucoes.push_back(new GenSolucao(avioes,1000,100));
	}
	GenAlgoritmo a(solucoes,0.1,true);
	GenSolucao* best = (GenSolucao*)a.fazerIteracoes(5000);

	cout << "\n";
 }