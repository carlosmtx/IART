#ifndef CROMOSSOMA_H
#define CROMOSSOMA_H
#include <String>
using namespace std;
class Cromossoma {
public:
	
	virtual double obterValor()=0;
	virtual void reproduzir(Cromossoma*)=0;
	virtual Cromossoma* obterCopia()=0;
	virtual void mutar()=0;
	virtual string toString()=0;
	virtual void printString()=0;

};

#endif