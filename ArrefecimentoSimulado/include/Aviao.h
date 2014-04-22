#ifndef AVIAO_H
#define AVIAO_H
#include <string>
#define ERRO_OUT(nome,hpref,hjin,hjfim,caa,car,tnu,desc) "("+nome+","+to_string(hpref)+","+to_string(hjin)+","+to_string(hjfim)+","+to_string(caa)+","+to_string(tnu)+")"+"Erro:"+desc
using namespace std;
class ErroInput{
public:
	string erro;
	ErroInput(string a){erro = a;}
};

class Aviao
{
public:
	string nome;					/*Nome Do Aviao*/

	int horaPreferencial;			/*Hora optima de aterragem*/
	
	int horaJanelaInicio;			/*Inicio da janela de aterragem do aviao*/
	int horaJanelaFim;				/*Fim da janela de aterragem do aviao*/

	int custoAterrAntecipada;		/*Factor de custo utilizado no calculo de uma aterragem antes  da hora optima*/
	int custoAterrRetardada;		/*Factor de custo utilizada no calculo de uma aterragem depois da hora optima*/

	int tempoNaoUtilizacao;			/*Tempo em que a pista fica inutilizada apos a aterragem de um aviao*/
public:
	Aviao(string nome,int horaPref,int horaJanIn,int horaJanFim ,int custAnt,int custDep,int tempNutil){
		this->nome					= nome;
		this->horaPreferencial		= horaPref;
		this->horaJanelaInicio		= horaJanIn;
		this->horaJanelaFim			= horaJanFim;
		this->custoAterrAntecipada  = custAnt;
		this->custoAterrRetardada   = custDep;
		this->tempoNaoUtilizacao	= tempNutil;
		if (horaPreferencial	 < 0){throw ErroInput(ERRO_OUT(nome,horaPreferencial,horaJanelaInicio,horaJanelaFim,custoAterrAntecipada,custoAterrRetardada,tempoNaoUtilizacao,"A Hora Preferencial de aterragem e negativa "));}
		if (horaJanelaInicio	 < 0){throw ErroInput(ERRO_OUT(nome,horaPreferencial,horaJanelaInicio,horaJanelaFim,custoAterrAntecipada,custoAterrRetardada,tempoNaoUtilizacao,"A Hora de Inicio da janela temporal de aterragem e negativa"));}
		if (horaJanelaFim		 < 0){throw ErroInput(ERRO_OUT(nome,horaPreferencial,horaJanelaInicio,horaJanelaFim,custoAterrAntecipada,custoAterrRetardada,tempoNaoUtilizacao,"A Hora de Fim da janela temporal de aterragem e negativa"));}
		if (custoAterrAntecipada < 0){throw ErroInput(ERRO_OUT(nome,horaPreferencial,horaJanelaInicio,horaJanelaFim,custoAterrAntecipada,custoAterrRetardada,tempoNaoUtilizacao,"O Custo da aterragem antecipada e negativo"));}
		if (custoAterrRetardada	 < 0){throw ErroInput(ERRO_OUT(nome,horaPreferencial,horaJanelaInicio,horaJanelaFim,custoAterrAntecipada,custoAterrRetardada,tempoNaoUtilizacao,"O Custo da aterragem retardada e negativo"));}
		if (tempoNaoUtilizacao	 < 0){throw ErroInput(ERRO_OUT(nome,horaPreferencial,horaJanelaInicio,horaJanelaFim,custoAterrAntecipada,custoAterrRetardada,tempoNaoUtilizacao,"O Tempo de nao utilizacao da pista e negativo "));}
		
		if (horaJanelaFim	 < horaJanelaInicio){throw ErroInput(ERRO_OUT(nome,horaPreferencial,horaJanelaInicio,horaJanelaFim,custoAterrAntecipada,custoAterrRetardada,tempoNaoUtilizacao,"A janela temporal tem valor final menor que o inicial "));}
		if (horaPreferencial < horaJanelaInicio){throw ErroInput(ERRO_OUT(nome,horaPreferencial,horaJanelaInicio,horaJanelaFim,custoAterrAntecipada,custoAterrRetardada,tempoNaoUtilizacao,"A hora preferencial nao esta entre os valores da janela temporal"));}
		if (horaPreferencial > horaJanelaFim)	{throw ErroInput(ERRO_OUT(nome,horaPreferencial,horaJanelaInicio,horaJanelaFim,custoAterrAntecipada,custoAterrRetardada,tempoNaoUtilizacao,"A hora preferencial nao esta entre os valores da janela temporal"));}
	}
	int getCusto(int tempo){
		if (tempo < horaJanelaInicio || tempo > horaJanelaFim){
			if(custoAterrAntecipada > custoAterrRetardada){ return 100 * custoAterrAntecipada;}
			else { return 100 * custoAterrRetardada;}
		} 
		else if (tempo == horaPreferencial){return custoAterrAntecipada*(horaPreferencial-tempo);}
		else if (tempo < horaPreferencial) {return custoAterrAntecipada*(horaPreferencial-tempo);}
		else							   {return custoAterrRetardada*(tempo-horaPreferencial);}
	}
};

#endif