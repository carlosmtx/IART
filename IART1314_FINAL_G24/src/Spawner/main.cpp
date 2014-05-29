#include <iostream>
#include <string>
#include <vector>
#include <random>   
#include <time.h>
#include <algorithm>
#include <fstream>
using namespace std;
int main(){
	srand(time(NULL));
	string fileName;
	int numAvioes;
	int maxOffset;
	int maxTempOc;
	int dominio;
	int maxCusto;
	int minTempOc;
	cout << "Nome do Ficheiro de Output: ";cin >> fileName;
	cout << "Numero Avioes:";cin >> numAvioes;
	cout << "Tamanho Maximo da Janela:";cin >> maxOffset;
	cout << "Tempo Maximo de Ocupacao:";cin >> maxTempOc;
	cout << "Tempo Minimo de Ocupacao:";cin >> minTempOc;
	cout << "Custo Maximo:";cin >> maxCusto;
	cout << "Dominio:";cin >> dominio;
	
	vector<string> avioesNome;
	vector<int> hPref;
	vector<int> hInicio;
	vector<int> hFim;
	vector<int> tOcup;
	for ( int i = 0 ; i < numAvioes ; i++){
		avioesNome.push_back( i < 10 ? "Aviao0"+to_string(i):"Aviao"+to_string(i));
	}

	{
		vector<int> all;
		for ( int i= 0 ; i < dominio;i++){all.push_back(i);}
		shuffle(all.begin(),all.end(),std::default_random_engine(time(NULL)));
		hPref.insert(hPref.begin(),all.begin(),all.begin()+numAvioes);
	}

	sort(hPref.begin(),hPref.end());
	
	for ( int i = 0 ; i < hPref.size() ; i++){
		int hBegin=0;
		int hEnd  =dominio;
		
		do{
			hBegin = hPref[i]-rand()%(maxOffset);
			hEnd   = hPref[i]+rand()%(maxOffset);
		}while( hEnd - hBegin > maxOffset || hEnd < 0 || hBegin < 0 || hBegin == hEnd || hEnd < hBegin );
		
		hInicio.push_back(hBegin);
		hFim.push_back(hEnd);
	}

	for (int i = 0 ; i < numAvioes; i++){
		tOcup.push_back(rand()%(maxTempOc-minTempOc)+minTempOc);
	}

	ofstream file(fileName);

	file <<" Nome HoraPreferencial HoraJanInicial HoraJanFim CustoAterragemAntecipada CustoAterragemRetardada TempoRepousoPista" <<endl;
	for(int i = 0 ; i < numAvioes ; i++){
		file << avioesNome[i] << ' ' << hPref[i] << ' '<< hInicio[i] <<' ' << hFim[i] << ' ' <<  rand()%maxCusto+1 << ' ' << rand()%maxCusto+1 << ' ' << tOcup[i] << endl;  
	}
	file.close();
	

}