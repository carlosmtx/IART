#ifndef UNICOST_H
#define UNICOST_H
#pragma once

#include "aviao.h"
#include <vector>
#include <queue>
#include <fstream>

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
	Aviao* aviao;
	vector<timeInterval> restrictions;

	Node();
	Node(Aviao* plane, int t);

	int getTotalCost() const;

	int getTimeInterval();

	friend ostream& operator<<(std::ostream& os, const Node* obj);


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
	bool hasSolution;
	priority_queue<Node*, vector<Node*>, DereferenceCompareNode> queue;
	ofstream* html;
	//priority_queue<Node*> queue;
	int currentTime,planeIndex;
	int getLatestTime();
	Aviao* nextPlane(Node* node);
	int generateBranches(Node* origin);
	void buildSolution(Node* node);
public:
	BNB();
	int getSolutionCost();
	vector<Node*> getSolution(){ return solution; };
	void printSolution();
	bool solve(vector<Aviao> planes, ofstream* htmlFile);
	~BNB();
};

bool operator<(const Node& lhs, const Node& rhs);


#endif
