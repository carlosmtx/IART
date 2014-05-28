#include "Loader.h"

Loader::Loader(string gen, string sim,string unit){



	ifstream infile;

	
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
	infile.close();




	/*CONFIGURACOES*/
	infile.open(gen);
	getline(infile, line);
	sscanf (line.c_str(),"iteracoes=%d", &this->gen_numIter);
	getline(infile, line);
	sscanf (line.c_str(),"elitista=%d", &this->gen_elitista);
	getline(infile, line);
	sscanf (line.c_str(),"populacao=%d", &this->gen_populacao);
	getline(infile, line);
	sscanf (line.c_str(),"probMutacao=%lf", &this->gen_probMutacao);
	infile.close();

	infile.open(sim);
	getline(infile, line);
	sscanf (line.c_str(),"maxIter=%d", &this->simA_maxIter);
	getline(infile, line);
	sscanf (line.c_str(),"maxPerturb=%d", &this->simA_maxPerturb);
	getline(infile, line);
	sscanf (line.c_str(),"maxSucessos=%d", &this->simA_maxSucessos);
	getline(infile, line);
	sscanf (line.c_str(),"fatorReducao=%lf", &this->simA_fatorReducao);
	infile.close();
	
	
	infile.open(unit);
	if (infile.is_open()){cout << "OPEN";}
	getline(infile,line);
	sscanf(line.c_str(),"maxTime=%d",&this->unit_MaxTime);
	infile.close();

}