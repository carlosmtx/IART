#ifndef SIMUL_H
#define SIMUL_H

#include "Populacao.h"
#include "Solucao.h"

class SimulatedAnnealing{
private:
	Populacao pop;
	int maxIteracoes;
	int maxPerturbacoes;
	int maxSucessos;
	int janelaTemporal;
	float fatorReducao;

	int tempInicial;
	int tempAtual;
	int energiaAtual;
	int variacaoEnergia;

	int temperatura;

	
	Solucao solucaoInicial;
	Solucao solucaoAtual;
	vector<Solucao> caminho;


public:
	SimulatedAnnealing();
	SimulatedAnnealing(int maxIteracoes, Populacao pop, int maxPerturbacoes, int maxSucessos, float fatorReducao);
	void executa(); // corre algoritmo

	void geraEstadoInicial(); // Gera estado inicial
	int calculaTemperatura(); // Calcula temperatura inicial
	Solucao perturbacao(Solucao sol); // Exerce uma perturbacao sobre a solucao passada
	bool condExponenteAleatoria(); // condicao para mesmo alterar a solucao atual

};


#endif