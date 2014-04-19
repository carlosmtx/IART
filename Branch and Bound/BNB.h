#ifndef BNB_H
#define BNB_H
#pragma once

#include "aviao.h"
#include <vector>
#include <queue>

using namespace std;

struct timeInterval
{
	int start, finish;
	timeInterval(int s, int f)
	{
		start = s;
		finish = f;
	}
};

class Node
{
public:
	int level;
	int departTime;
	Node* parent;
	vector<Node*> branches;
	Aviao* plane;
	vector<timeInterval> restrictions;

	Node();
	Node(Aviao* plane, int t);

	int getTotalCost() const;

	int getTimeInterval();
	bool noConflict(Node* node, int t);

	friend ostream& operator<<(std::ostream& os, const Node obj);


};

struct DereferenceCompareNode : public std::binary_function<Node*, Node*, bool>
{
	bool operator()(const Node* lhs, const Node* rhs) const
	{
		return lhs->getTotalCost() > rhs->getTotalCost();
	}
};

class BNB
{
private:
	vector<Aviao*> planes;
	vector<Node*> solution;
	priority_queue<Node*, vector<Node*>, DereferenceCompareNode> queue;

	//priority_queue<Node*> queue;
	int currentTime,planeIndex;
	int getLatestTime();
	Aviao* nextPlane(Node* node);
	int generateBranches(Node* origin);
	vector<Node*> buildSolution(Node* node);
public:
	BNB();
	int getSolutionCost();
	vector<Node*> solve(vector<Aviao*> planes);
	~BNB();
};

bool operator<(const Node& lhs, const Node& rhs);


#endif
