#ifndef POPULACAO_H
#define POPULACAO_H
#include "Individuo.h"
class Populacao{
public:
	vector<Aviao>     avioes;
	vector<Individuo> individuos;
	int				  nIteracoes;
	Populacao(int tamanho,int retardPenalty){
		try{
			avioes.push_back(Aviao("Aviao1" ,11,0 ,30,2 ,4 ,5 ));
			avioes.push_back(Aviao("Aviao2" ,22,20,25,3 ,5 ,5 ));
			avioes.push_back(Aviao("Aviao3" ,33,30,45,4 ,6 ,5 ));
			avioes.push_back(Aviao("Aviao4" ,44,40,65,5 ,7 ,5 ));
			avioes.push_back(Aviao("Aviao5" ,55,40,75,6 ,8 ,5 ));
			avioes.push_back(Aviao("Aviao6" ,66,50,95,7 ,9 ,5 ));
			avioes.push_back(Aviao("Aviao7" ,77,20,95,7 ,8 ,5 ));
			avioes.push_back(Aviao("Aviao8" ,88,54,95,7 ,7 ,5 ));	
			avioes.push_back(Aviao("Aviao9" ,98,80,99,7 ,6 ,5 ));	
			avioes.push_back(Aviao("Aviao10",13,1 ,95,7 ,5 ,5 ));	
			avioes.push_back(Aviao("Aviao11",23,13,45,7 ,4 ,5 ));	
			avioes.push_back(Aviao("Aviao12",33,10,40,7 ,3 ,5 ));	
			avioes.push_back(Aviao("Aviao13",42,24,49,7 ,2 ,5 ));	
		}
		catch(ErroInput& e){
			cout << e.erro << endl;
		}
		for ( unsigned int i = 0 ; i < tamanho ; i++)
		{
			individuos.push_back(Individuo(100,avioes));
		}
	}

	void reproduzir(){

	}
	void mutar(){

	}
	Individuo melhorAdaptado(){

	}
};
#endif