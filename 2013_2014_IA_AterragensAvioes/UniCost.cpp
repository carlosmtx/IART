#include "UniCost.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <limits>
#include <thread>
#include <windows.h>

using namespace std;

#define JSON_AVIAO(nome_,hAterragem_,tempoOc_,intervInf_,intervSup_) "{\"nome\":"+'"'+nome_+'"'+",\"hAterragem\":"+to_string(hAterragem_)+ ",\"tempoOcup\":"+to_string(tempoOc_)+",\"intervSup\":"+to_string(intervSup_)+",\"intervInf\":"+to_string(intervInf_)+"}"

/*Basic functions*/
#define boolean(a) ((a)==1?"TRUE":"FALSE")
//[x1,x2] vs [y1,y2]
bool is_overlapping(int x1, int x2, int y1, int y2) {
	return (bool)((((unsigned int)((x2 - y1) | (y2 - x1))) >> (sizeof(int)* 8 - 1)) ^ 1);
}

ostream& operator<<(std::ostream& os, priority_queue<Node*> container)
{
	priority_queue<Node*> x = container;
	os << "{";
	while (!x.empty())
	{
		os << x.top();
		if (x.size() > 1)
			os << ",";
		x.pop();
	}
	os << "}";
	return os;
}

template <class T>
ostream& operator<<(std::ostream& os, vector<T*> container)
{
	int size = container.size();
	os << "[";
	for (int i = 0; i < size; i++)
	{
		os << (container[i]);
		if (i < size - 1)
			os << ",";
	}
	os << "]";
	return os;
}

bool operator<(const Node& lhs, const Node& rhs)
{
	return lhs.getTotalCost() > rhs.getTotalCost();
}

/*Branch and Bound class related functions*/
BNB::BNB()
{
	/*
	for (int i = 0; i < 5; i++)
	{
	cout << "Testing for " << i << ": 1->" << boolean(is_overlapping(1, 3, i, i + 5)) << ", 2->" << boolean(is_overlapping(6, 10, i, i + 5)) << endl;
	}*/
}

Aviao* BNB::nextPlane(Node* node)
{
	return planes[node->level];
}

int BNB::getSolutionCost()
{
	return solution[0]->getTotalCost();
}

int BNB::getLatestTime()
{
	int size = planes.size();
	int cost = 0;
	for (int i = 0; i < size; i++)
	{
		if (planes[i]->horaJanelaFim > cost)
			cost = planes[i]->horaJanelaFim;
	}
	return cost;
}

BNB::~BNB()
{}


void stopSolving(int t, bool *done, bool *solved)
{
	Sleep(t*1000);
	if (*solved)
		return;
	*done = true;
}


bool BNB::solve(int timeToSolve, vector<Aviao> planes, ofstream* htmlFile)
{
	this->time_to_solve = timeToSolve;
	
	bool timedOut = false;
	bool solved = false;
	
	std::thread timer(&stopSolving, time_to_solve, &timedOut, &solved);
	timer.detach();

	html = htmlFile;
	for (int i = 0; i < planes.size(); i++)
	{
		this->planes.push_back(&planes[i]);
	}
	
	currentTime = 0;
	planeIndex = 0;
	Node* root = new Node();
	int nodeLvl, i, nBranch;
	queue.push(root);
	Node * top = NULL;
	nodeLvl = 0;
	int nPlanes = planes.size();
	while (!queue.empty() && !timedOut) //condition must change possibly queue.top().level != planes.size()
	{
		top = queue.top();
		queue.pop();
		nodeLvl = top->level;
		if (nodeLvl == nPlanes)
			break;
		
		Node* parent = top;
	
		nBranch = generateBranches(top);
		
		for (i = 0; i < nBranch; i++)
		{
			queue.push((top->branches[i]));
		}
		
	}
	
	if (timedOut)
	{
		cout << "Algoritmo demorou demasiado tempo, vamos ver o que temos:\n\n";
	}
	else
		solved = true; // searched the tree completly or found solution

	buildSolution(top);
	cout << "Custo Uniforme: ";
	printSolution();

	if (solution.size() == nPlanes)
	{
		cout << "Solu��o encontrada";
		//printSolution();
		return true;
	}
	else
	{
		cout << "Nenhuma solucao encontrada" << endl;
		return false;
	}
}

int BNB::generateBranches(Node* origin)
{
	int nBranches;
	nBranches = 0;
	int level = origin->level + 1;
	Aviao* nextPlane = this->nextPlane(origin);
	int end = nextPlane->horaJanelaFim;

	if (origin->aviao == NULL) //root node
	{
		for (int i = nextPlane->horaJanelaInicio; i <= end; i++)
		{
			Node* next = new Node(nextPlane, level);
			next->parent = origin;
			next->departTime = i;
			next->restrictions.push_back(timeInterval(i, i + next->aviao->tempoNaoUtilizacao));
			origin->branches.push_back(next);
			nBranches++;
			//cout << "Added branch from root to " << next->plane->nome << " at time -> " << next->departTime << endl;
		}
	}
	else
	{
		bool valid;
		int size = origin->restrictions.size();
		int startNext, endNext, startOri, endOri;
		/*cout << "Restrictions: ";
		for (int i = 0; i < size; i++)
		{
			cout << "[" << origin->restrictions[i].start << "," << origin->restrictions[i].finish << "]";
			if (i != origin->restrictions.size() - 1)
				cout << ", ";
		}
		cout << endl;*/
		for (int i = nextPlane->horaJanelaInicio; i <= end; i++)
		{
			valid = true;
			startNext = i;
			endNext = i + nextPlane->tempoNaoUtilizacao;
			for (int j = 0; j < size; j++)
			{
				startOri = origin->restrictions[j].start;
				endOri = origin->restrictions[j].finish;
				//cout << "Testing [" << startNext << "," << endNext << "] with: [" << startOri << "," << endOri << "]" << endl;
				if (is_overlapping(startNext, endNext, startOri, endOri))
				{
					//cout << " ... skipped " << endl;
					valid = false;
					break;
				}
			}
			if (valid)
			{
				Node* next = new Node(nextPlane, level);
				//add parent's restrictions to new son
				//cout << "\tadding " << size << " parent restrictions\n\n";
				for (int z = 0; z < size; z++) //add parent's restrictions
				{
					next->restrictions.push_back(origin->restrictions[z]);
				}
				//add new branch
				next->departTime = startNext;
				next->parent = origin;
				next->restrictions.push_back(timeInterval(startNext, endNext));
				origin->branches.push_back(next);
				nBranches++;
				//cout << " ... Added branch " << origin->plane->nome << " to " << next->plane->nome << " with interval: [" << startNext << "," << endNext << "], and cost: " << next->getTotalCost() << endl << endl;
			}
		}
		//origin->restrictions.pop_back(); //removes temporary restriction
	}
	return nBranches;
}

void BNB::buildSolution(Node* node)
{
	vector<Node*> sol;
	while (node->aviao != NULL)
	{
		sol.push_back(node);
		node = node->parent;
	}
	std::reverse(sol.begin(), sol.end());
	solution = sol;
	return;
}

void BNB::printSolution(){

	cout << "\n";
	cout << left << setw(20) << "Aviao Nome";
	cout << left << setw(8) << "Tempo";
	cout << left << setw(8) << "Custo";
	cout << "\n";

	for (int i = 0; i < solution.size(); i++){
		cout << left << setw(20) << solution[i]->aviao->nome;
		cout << left << setw(8) << std::to_string(solution[i]->departTime);
		cout << left << setw(8) << to_string((int)solution[i]->aviao->getCusto(solution[i]->departTime));
		cout << endl;
	}

	cout << endl << "CUSTO:" << getSolutionCost()<<endl;

	string str;
	str += "[";
	for (int i = 0; i < solution.size(); i++){
		str += ((string)JSON_AVIAO(solution[i]->aviao->nome, solution[i]->departTime, solution[i]->aviao->tempoNaoUtilizacao, solution[i]->aviao->horaJanelaInicio, solution[i]->aviao->horaJanelaFim));
		if (i != solution.size() - 1){ str += ","; }
	}
	str += "]";

	*html << "<div id=\"jsonDataBNB\" style=\"display:none;\">" << str << "</div>";
}


/*Node class related functions*/

Node::Node()
{
	aviao = NULL;
	level = 0;
}

Node::Node(Aviao* aviao, int lvl)
{
	this->aviao = aviao;
	this->level = lvl;
}

ostream& operator<<(std::ostream& os, const Node* obj)
{
	if (obj->aviao == NULL)
		os << "root";
	else
	{
		os << "{" << obj->aviao->nome << " -> " << obj->departTime << "}";
	}
	return os;
}

int Node::getTotalCost() const
{
	int cost = 0;
	int auxValue;
	const Node* aux = this;
	while (aux->aviao != NULL)
	{

		auxValue = aux->aviao->getCusto(aux->departTime);
		if (auxValue != -1)
			cost += auxValue;
		else
			cost += 50000; //cost penalty
		aux = aux->parent;
	}
	return cost;
}