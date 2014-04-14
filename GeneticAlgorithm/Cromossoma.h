#ifndef CROMOSSOMA_H
#define CROMOSSOMA_H
class Cromossoma {
public:
	
	virtual double obterValor()=0;
	virtual Cromossoma* obterDescendencia(Cromossoma*)=0;
	virtual void mutar()=0;

};

#endif