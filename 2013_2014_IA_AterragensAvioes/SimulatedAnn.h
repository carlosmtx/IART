#ifndef SIMUL_H
#define SIMUL_H

#include "SimAnnSolucao.h"
#include "Aviao.h"


class SimulatedAnnealing{
private:
	vector<Aviao> pop;
	int maxIteracoes;
	int maxPerturbacoes;
	int maxSucessos;
	int janelaTemporal;
	float fatorReducao;

	int tempInicial;
	int tempAtual;
	int energiaInicial;
	int variacaoEnergia;

	double temperatura;

	
	SimAnnSolucao solucaoInicial;
	SimAnnSolucao solucaoAtual;
	vector<SimAnnSolucao> caminho;


public:
	SimulatedAnnealing();
	SimulatedAnnealing(int maxIteracoes, vector<Aviao> pop, int maxPerturbacoes, int maxSucessos, float fatorReducao, int janelaTemporal);
	SimulatedAnnealing(int maxIteracoes, vector<Aviao> pop, int maxPerturbacoes, int maxSucessos, float fatorReducao, int janelaTemporal, SimAnnSolucao& solInicia);
	void executa(); // corre algoritmo

	void geraEstadoInicial(); // Gera estado inicial
	int calculaTemperatura(); // Calcula temperatura inicial
	SimAnnSolucao perturbacao(SimAnnSolucao sol); // Exerce uma perturbacao sobre a solucao passada
	bool condExponenteAleatoria(); // condicao para mesmo alterar a solucao atual

	SimAnnSolucao getSolucaoAtual(){ return this->solucaoAtual;};
};


#endif