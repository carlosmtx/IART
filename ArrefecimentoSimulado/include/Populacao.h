#ifndef POPULACAO_H
#define POPULACAO_H
#include "Aviao.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
class Populacao{
public:
	vector<Aviao> avioes;

	Populacao(string filename){

		ifstream infile;
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
		}catch(ErroInput& e){
			cout << e.erro << endl;
		}

	}

	Populacao(){
		try{
			avioes.push_back(Aviao("Aviao 1" ,11,0 ,30,2 ,4 ,5 ));
			avioes.push_back(Aviao("Aviao 2" ,22,20,25,3 ,5 ,5 ));
			avioes.push_back(Aviao("Aviao 3" ,33,30,45,4 ,6 ,5 ));
			avioes.push_back(Aviao("Aviao 4" ,44,40,65,5 ,7 ,5 ));
			avioes.push_back(Aviao("Aviao 5" ,55,40,75,6 ,8 ,5 ));
			avioes.push_back(Aviao("Aviao 6" ,66,50,95,7 ,9 ,5 ));
			avioes.push_back(Aviao("Aviao 7" ,77,20,95,7 ,8 ,5 ));
			avioes.push_back(Aviao("Aviao 8" ,88,54,95,7 ,7 ,5 ));	
			avioes.push_back(Aviao("Aviao 9" ,98,80,99,7 ,6 ,5 ));	
			avioes.push_back(Aviao("Aviao10",13,1 ,95,7 ,5 ,5 ));	
			avioes.push_back(Aviao("Aviao11",23,13,45,7 ,4 ,5 ));	
			avioes.push_back(Aviao("Aviao12",33,10,40,7 ,3 ,5 ));	
			avioes.push_back(Aviao("Aviao13",42,24,49,7 ,2 ,5 ));	
		}
		catch(ErroInput& e){
			cout << e.erro << endl;
		}
	}
};
#endif