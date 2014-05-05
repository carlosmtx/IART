#include "UniCost.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

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


bool BNB::solve(vector<Aviao> planes)
{
	for (int i = 0; i < planes.size(); i++)
	{
		this->planes.push_back(&planes[i]);
	}
	currentTime = 0;
	planeIndex = 0;
	Node* root = new Node();
	//root.parent.time = planes[0]->horaJanelaInicio; //set earliest time for the root node
	int nodeLvl, i, nBranch;
	queue.push(root);
	Node * top = NULL;
	nodeLvl = 0;
	int nPlanes = planes.size();
	while (!queue.empty()) //condition must change possibly queue.top().level != planes.size()
	{
		top = queue.top();
		queue.pop();
		nodeLvl = top->level;
		if (nodeLvl == nPlanes)
			break;
		//cout << "nodeLvl: " << nodeLvl << ", testing: " << top << endl;
		
		Node* parent = top;
		/* cout << endl << "Path: " << endl << "\t";
		while (parent->plane != NULL)
		{
			cout << parent->plane->nome << "[" << parent->departTime << "]" << "<-";
			parent = parent->parent;
		}
		cout << "root" << endl << "\tCost:" << top->getTotalCost() << endl << endl;
		*/
		nBranch = generateBranches(top);
		//cout << endl << "number of branches generated: " << nBranch << endl;
		for (i = 0; i < nBranch; i++)
		{
			queue.push((top->branches[i]));
		}
		//cout << "Queue size: " << queue.size() << endl << endl;
		//system("PAUSE");
	}

	buildSolution(top);
	cout << "Uniform cost: ";
	if (solution.size() == nPlanes)
	{
		cout << "Found solution";
		printSolution();
		return true;
	}
	else
	{
		cout << "No solution could be found" << endl;
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

	if (origin->plane == NULL) //root node
	{
		for (int i = nextPlane->horaJanelaInicio; i <= end; i++)
		{
			Node* next = new Node(nextPlane, level);
			next->parent = origin;
			next->departTime = i;
			next->restrictions.push_back(timeInterval(i, i + next->plane->tempoNaoUtilizacao));
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
	while (node->plane != NULL)
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
		cout << left << setw(20) << solution[i]->plane->nome;
		cout << left << setw(8) << std::to_string(solution[i]->departTime);
		cout << left << setw(8) << to_string((int)solution[i]->plane->getCusto(solution[i]->departTime));
		cout << endl;
	}

	cout << endl << "CUSTO:" << getSolutionCost()<<endl;
}


/*Node class related functions*/

Node::Node()
{
	plane = NULL;
	level = 0;
}

Node::Node(Aviao* plane, int lvl)
{
	this->plane = plane;
	this->level = lvl;
}

ostream& operator<<(std::ostream& os, const Node* obj)
{
	if (obj->plane == NULL)
		os << "root";
	else
	{
		os << "{" << obj->plane->nome << " -> " << obj->departTime << "}";
	}
	return os;
}

int Node::getTotalCost() const
{
	int cost = 0;
	int auxValue;
	const Node* aux = this;
	while (aux->plane != NULL)
	{

		auxValue = aux->plane->getCusto(aux->departTime);
		if (auxValue != -1)
			cost += auxValue;
		else
			cost += std::numeric_limits<int>::max(); //cost penalty
		aux = aux->parent;
	}
	return cost;
}