#include <vector>
#include "aviao.h"
#include "BNB.h"

/*	Aviao(string nome, int horaPref, int horaJanIn, int horaJanFim, int custAnt, int custDep, int tempNutil){*/

int main()
{
	vector<Aviao*> vec;
	vec.push_back(new Aviao("1", 2, 0, 3, 2, 4, 5));
	vec.push_back(new Aviao("2", 10, 0, 15, 3, 1, 8));
	vec.push_back(new Aviao("3", 25, 20, 40, 7, 2, 3));
	vec.push_back(new Aviao("4", 35, 30, 40, 2, 1, 5));
	vec.push_back(new Aviao("5", 42, 25, 50, 4, 8, 6));
	BNB search;
	search.solve(vec);
	system("PAUSE");
	return 0;
}