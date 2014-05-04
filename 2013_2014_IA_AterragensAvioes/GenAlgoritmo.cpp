#include "GenAlgoritmo.h"
#include <limits>
#include <time.h>

inline void printHTMLHeader(ofstream& html){
	/*html << (string)
		"<head><meta http-equiv='Content-Type' content='text/html; charset=UTF-8'>"
		"<link rel='stylesheet' href='bootstrap/css/bootstrap.min.css' media='all' />"+
		"<link rel='stylesheet' href='bootstrap/css/bootstrap-theme.min.css' media='all' />"+
		"<link rel='stylesheet' href='bootstrap/genetic.css'/>"+
		"<script src='bootstrap/js/jquery.js'></script>"+
		"<script src='bootstrap/js/bootstrap.min.js'></script>"+
		"</head>";*/
	html <<(string)" <head>"+
			"<script src=\"results.js\"></script>"+
			"</head>"+
			"<canvas id=\"myCanvas\" width=\"10000\" height=\"100\">"+
			"</canvas>";
}
inline void printHTML(ofstream& html,int& iteration,int& media , int& bestValue , string& sol){
	
	if ( iteration % 30 == 0){
		html << "<div class='row rowheader'> "<<
						"<div class='col-md-2'>"<< "Iteracao"<<"</div>" <<
						"<div class='col-md-2'>"<< "Media"   <<"</div>" <<
						"<div class='col-md-2'>"<< "Melhor Valor" <<"</div>" <<
						"<div class='col-md-6'>"<< "Solucao" <<"</div>" <<
					"</div>";
		}
		
		html << "<div class='row'> "<<
					"<div class='col-md-2'>"<< iteration <<"</div>" <<
					"<div class='col-md-2'>"<< media<<"</div>" <<
					"<div class='col-md-2'>"<< bestValue<<"</div>" <<
					"<div class='col-md-6'>"<< sol		<<"</div>";
		html<<"</div>";
	
}
inline void printCSV(ofstream& csv , int& iteration, int& media,int& bestValue){
	csv << iteration <<";"<< bestValue << ";" << media << "\n";
}
inline void printProgressBar(int& i,int& n,time_t& tm){
	if ( i % 10 == 0 && i > 0){
			cout << "\r"; 
			cout << "[";
			for ( int j = 0 ; j < 100*i/n	 ;j+=5,cout << "-" ); 
			for ( int j = 0 ; j < 100-100*i/n;j+=5,cout << "." );
			cout << "]";
			cout << 100*i/n << "% completo";
			cout <<"->Tempo Estimado: " <<double(time(NULL)-tm)*(n-i)/double(i) << " s";
	}
}
inline void printFinalSolution(GenCromossoma* crom){

}


inline void GenAlgoritmo::printAll(int& iteration){
	
	double sum=0;
	valores.resize(cromossomas.size());
	for(int i=0 ; i < cromossomas.size() ; i++){
		valores[i] = cromossomas[i]->obterValor();
		sum+=1/valores[i];
	}
	GenCromossoma* best = obterMaisBemAdaptado();
	int bestValue = 1/best->obterValor();
	int media   = sum/valores.size();

	//printHTML(htmlFile,iteration,media,bestValue,best->toString());
	printCSV (csvFile ,iteration,media,bestValue);
	delete(best);
}

GenAlgoritmo::GenAlgoritmo(vector<GenCromossoma*> popInicial,double probMutacao,bool elitista){
	this->cromossomas = popInicial;
	this->mutationProb = probMutacao;
	this->elitista = elitista;
	this->selecionadosElitista = selecionadosElitista;
	if ( elitista && popInicial.size()%2 == 0){
		this->cromossomas.pop_back();
	}
	csvFile.open("Solucao/Genresults.csv ",ios::out);
	htmlFile.open("Solucao/results.html",ios::out);	
	printHTMLHeader(htmlFile);
}

vector<GenCromossoma*> GenAlgoritmo::obterAmostraPRepr(int n){
	if (n > cromossomas.size()){ throw new invalid_argument("Alg_Genetico : obterAmostraPRep : valor invalido para n : n > cromossomas.size\n");}
	if (n < 1 )				   { throw new invalid_argument("Alg_Genetico : obterAmostraPRep : valor invalido para n : n < 1\n");}
	
	valores.resize(cromossomas.size());
	double sum =0;
	
	for(int i=0 ; i < cromossomas.size() ; i++){
		valores[i] = cromossomas[i]->obterValor();
		sum+=valores[i];
	}

	if (sum == 0 ){throw new range_error("Alg_Genetico : obterAmostraPRep : valor invalido em sum : sum == 0");}
	valores[0] = valores[0] / sum;

	for (int i = 1 ; i < cromossomas.size();i++){
		valores[i] = valores[i]/sum;
		valores[i] = valores[i-1]+valores[i];
	}
	valores.back() = 1;


	vector<GenCromossoma*> selecionados;
	selecionados.reserve(cromossomas.size());
	
	double aleatorio;
	for (int i = 0 ; i < n ; i++){
		aleatorio = (rand()%10000)/double(10000);
		for ( int j = 0 ; j < cromossomas.size() ; j++){
			if ( aleatorio < valores[j] ){selecionados.push_back( cromossomas[j] );break;}		
		}
	}
	return selecionados;

}
vector<GenCromossoma*> GenAlgoritmo::removerMenosFit(int n){
	if (n > cromossomas.size()){ throw new invalid_argument("Alg_Genetico : obterAmostraPRep : valor invalido para n : n > cromossomas.size\n");}
	if (n < 1 )				   { throw new invalid_argument("Alg_Genetico : obterAmostraPRep : valor invalido para n : n < 1\n");}
	valores.resize(cromossomas.size());
	double sum =0;
	
	for(int i=0 ; i < cromossomas.size() ; i++){
		valores[i] = 1/cromossomas[i]->obterValor();
		sum+=valores[i];
	}

	if (sum == 0 ){throw new range_error("Alg_Genetico : removerMenosFit : valor invalido em sum : sum == 0");}
	valores[0] = valores[0] / sum;

	for (int i = 1 ; i < cromossomas.size();i++){
		valores[i] = valores[i]/sum;
		valores[i] = valores[i-1]+valores[i];
	}
	valores.back() = 1;


	
	double aleatorio;
	vector<GenCromossoma*> baixas;	
	for (int i = 0 ; i < n ; i++){
		aleatorio = (rand()%10000)/(double)10000;
		for ( int j = 0 ; j < cromossomas.size() ; j++){
			if ( aleatorio < valores[j] ){
				baixas.push_back( cromossomas[j] );
				cromossomas.erase(cromossomas.begin()+i);
				valores.erase(valores.begin()+i);
				double sum = 0;
				for ( int i = 0 ; i < valores.size(); i++){sum+=valores[i];}
				valores[0] = valores[0] / sum;
				for ( int i = 1 ; i < valores.size(); i++){valores[i]=valores[i]/sum;valores[i] = valores[i-1]+valores[i];}
				valores.back() = 1;
				break;
			}
		}
	}

	return baixas;
}
GenCromossoma* GenAlgoritmo::obterMaisBemAdaptado(){
/*	double sum=0;
	valores.resize(cromossomas.size());
	for(int i=0 ; i < cromossomas.size() ; i++){
		valores[i] = cromossomas[i]->obterValor();
		sum+=valores[i];
	}
*/
	GenCromossoma* best = cromossomas[0];
	for ( int i = 1 ; i < cromossomas.size() ; i++){
		if (best->obterValor() < cromossomas[i]->obterValor())
		{best = cromossomas[i];} 
	}
	return best->obterCopia();
}
double GenAlgoritmo::obterSoma(){
	double sum=0;
	valores.resize(cromossomas.size());
	for(int i=0 ; i < cromossomas.size() ; i++){
		valores[i] = cromossomas[i]->obterValor();
		sum+=valores[i];
	}
	return sum;	
}
void GenAlgoritmo::cicloDeVida(){
	
	vector<GenCromossoma*> newPop ;
	newPop.reserve(cromossomas.size());

	vector<GenCromossoma*> pReproduzir = obterAmostraPRepr(cromossomas.size());

	if(elitista){
		newPop.push_back(obterMaisBemAdaptado());
	}

	for ( int i = newPop.size() ; i < pReproduzir.size() ; i+=2){
		newPop.push_back(pReproduzir[i]  ->obterCopia());
		newPop.push_back(pReproduzir[i+1]->obterCopia());
		newPop[newPop.size()-1]->reproduzir(newPop[newPop.size()-2]);
		if ( (rand()%10000)/10000 < mutationProb ){newPop[newPop.size()-1]->mutar();}
		if ( (rand()%10000)/10000 < mutationProb ){newPop[newPop.size()-2]->mutar();}

	}

	for ( int i = 0 ; i< cromossomas.size() ; i++){
		delete(cromossomas[i]);
	}

	cromossomas = newPop;
}
GenCromossoma* GenAlgoritmo::fazerIteracoes(int n){
	time_t tm= time(NULL);
	for ( int i = 0 ; i < n ; i++){
		cicloDeVida();
		printAll(i);
		printProgressBar(i,n,tm);
	}
	printProgressBar(n,n,tm);
	GenCromossoma* best = cromossomas[0];
	for ( int i = 1 ; i < cromossomas.size() ; i++){
		if (best->obterValor() < cromossomas[i]->obterValor())
		{best = cromossomas[i];} 
	}
	best->printString();
	cout <<endl<<"CUSTO:"<< 1/best->obterValor();
	htmlFile <<"<div id=\"jsonData\" style=\"display:none;\">" <<best->toString() <<"</div>";
	htmlFile <<"<script>drawAll();</script>";
	return best;
}





