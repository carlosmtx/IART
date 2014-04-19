#include "BNB.h"
#include <algorithm>
#include <iostream>
#include <limits>

using namespace std;

/*Basic functions*/
#define boolean(a) ((a)==1?"TRUE":"FALSE")
//[x1,x2] vs [y1,y2]
bool is_overlapping(int x1, int x2, int y1, int y2) {
	return (bool)((((unsigned int)((x2 - y1) | (y2 - x1))) >> (sizeof(int)* 8 - 1)) ^ 1);
}

ostream& operator<<(std::ostream& os, priority_queue<Node> container)
{
	priority_queue<Node> x = container;
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
		os << (*container[i]);
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
	int totalCost = 0;
	for (vector<Aviao*>::iterator it = planes.begin(); it != planes.end(); it++)
	{
		totalCost = (*it)->getCusto(currentTime);
	}
	return totalCost;
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


vector<Node*> BNB::solve(vector<Aviao*> planes)
{
	this->planes = planes;
	currentTime = 0;
	planeIndex = 0;
	Node* root = new Node();
	//root.parent.time = planes[0]->horaJanelaInicio; //set earliest time for the root node
	int nodeLvl, i, nBranch;
	queue.push(root);
	Node * top=NULL;
	nodeLvl = 0;
	int nPlanes = planes.size();
	while (!queue.empty()) //condition must change possibly queue.top().level != planes.size()
	{
		top = queue.top();
		queue.pop();
		nodeLvl = top->level;
		if (nodeLvl == nPlanes)
			break;
		cout <<"nodeLvl: "<<nodeLvl <<", testing: " << (*top) << endl;
		//cout <<endl << "doing lvl:" << nodeLvl << endl;
		Node* parent = top;
		cout << endl << "Path: " << endl << "\t";
		while (parent->plane != NULL)
		{
			cout << parent->plane->nome << "[" << parent->departTime << "]" << "<-";
			parent = parent->parent;
		}
		cout << "root" << endl<<"\tCost:" <<top->getTotalCost() << endl<<endl;
		nBranch = generateBranches(top);
		cout << endl << "number of branches generated: " << nBranch << endl;
		for (i = 0; i < nBranch; i++)
		{
			queue.push((top->branches[i]));
		}
		cout << "Queue size: " << queue.size() <<  ",top " << *(queue.top()) << endl<<endl;
		//system("PAUSE");
	}

	solution = buildSolution(top);
	return solution;
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
			origin->branches.push_back(next);
			nBranches++;
			cout << "Added branch from root to " << next->plane->nome << " at time -> " << next->departTime << endl;
		}
	}
	else
	{
		cout << "Testing interval: [" << origin->departTime <<","<<origin->departTime+origin->plane->tempoNaoUtilizacao<<"]"<< endl;
		//origin->restrictions.push_back(timeInterval(origin->departTime, origin->departTime + origin->plane->tempoNaoUtilizacao)); //adds temporary restriction
		for (int i = nextPlane->horaJanelaInicio; i <= end;i++)
		{
			cout << "  with: [" << i <<","<<i+nextPlane->tempoNaoUtilizacao<<"]";
			if (is_overlapping(i, i + nextPlane->tempoNaoUtilizacao, origin->departTime, origin->departTime+origin->plane->tempoNaoUtilizacao))
			{
				cout << " ... skipped "<< endl;
			}
			else
			{
				Node* next = new Node(nextPlane, level);
				if (origin->noConflict(next, i))
				{
					//add parent's restrictions to new son
					int nRestrictions = origin->restrictions.size();

					for (int j = 0; j < nRestrictions; j++) //add parent's restrictions
					{
						next->restrictions.push_back(origin->restrictions[j]);
					}

					//add new branch
					next->departTime = i;
					next->parent = origin;
					origin->branches.push_back(next);
					nBranches++;
					cout << " ... Added branch " << origin->plane->nome << " to " << next->plane->nome << " with interval: [" << next->departTime <<"," <<next->departTime+next->plane->tempoNaoUtilizacao<<"], and cost: "<<next->getTotalCost() << endl;
				}
				else
				{
					delete next; //avoid memory leak
					cout << ".... Deleted 1 of "<<origin->restrictions.size()<<" restrictions" << endl;
				}
			}
		}
		//origin->restrictions.pop_back(); //removes temporary restriction
	}
	return nBranches;
}

vector<Node*> BNB::buildSolution(Node* node)
{
	vector<Node*> sol;
	while (node->plane != NULL)
	{
		sol.push_back(node);
		node = node->parent;
	}
	std::reverse(sol.begin(), sol.end());

	cout << "solution: " << sol<<endl;
	return sol;
}

/*Node class related functions*/

bool Node::noConflict(Node* node, int t)
{
	int size = restrictions.size();
	int start = node->plane->horaJanelaInicio;
	int end = node->plane->horaJanelaFim;
	int timeOcupied = node->plane->tempoNaoUtilizacao;

	//cout << "restriction conflit: " << this->plane->nome << " has " << restrictions.size() << " restrictions, " << node->plane->nome << " has " << node->restrictions.size() << " restrictions" << endl;
	for (start; start <= end; start++)
	{
		for (int i = 0; i < size; i++)
		{
			//cout << "testing " << "[" << start << "," << start + timeOcupied << "]" << " to " << "[" << restrictions[i].start << "," << restrictions[i].finish << "]" << endl;
			if (is_overlapping(start, start + timeOcupied, restrictions[i].start, restrictions[i].finish))
				return false;
		}
	}

	return true;
}

Node::Node()
{
	plane = NULL;
	level =0;
}

Node::Node(Aviao* plane, int lvl)
{
	this->plane = plane;
	this->level = lvl;
}

ostream& operator<<(std::ostream& os, const Node obj)
{
	if (obj.plane == NULL)
		os << "root";
	else
	{
		os << "{" << obj.plane->nome << " -> " << obj.departTime << "}";
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