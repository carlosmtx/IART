#include <vector>
#include "aviao.h"
#include "BNB.h"
#include <fstream>
#include <iostream>

/*	Aviao(string nome, int horaPref, int horaJanIn, int horaJanFim, int custAnt, int custDep, int tempNutil){*/

int main()
{
	FILE *stream;
	std::ofstream out("out.txt");
	std::cout.rdbuf(out.rdbuf());
	vector<Aviao*> vec;
	/*vec.push_back(new Aviao("1", 2, 0, 3, 2, 4, 4));
	vec.push_back(new Aviao("2", 3, 3, 4, 3, 1, 2));
	vec.push_back(new Aviao("3", 25, 20, 40, 7, 2, 3));
	vec.push_back(new Aviao("4", 35, 30, 40, 2, 1, 5));
	vec.push_back(new Aviao("5", 42, 25, 50, 4, 8, 6));
	*/
	vec.push_back(new Aviao("Aviao1", 11, 0, 30, 2, 4, 5));
	vec.push_back(new Aviao("Aviao2", 22, 20, 25, 3, 5, 5));
	vec.push_back(new Aviao("Aviao3", 33, 30, 45, 4, 6, 5));
	vec.push_back(new Aviao("Aviao4", 44, 40, 65, 5, 7, 5));
	vec.push_back(new Aviao("Aviao5", 55, 40, 75, 6, 8, 5));
	vec.push_back(new Aviao("Aviao6", 66, 50, 95, 7, 9, 5));
	vec.push_back(new Aviao("Aviao7", 77, 20, 95, 7, 8, 5));
	vec.push_back(new Aviao("Aviao8", 88, 54, 95, 7, 7, 5));
	vec.push_back(new Aviao("Aviao9", 98, 80, 99, 7, 6, 5));
	vec.push_back(new Aviao("Aviao10", 13, 1, 95, 7, 5, 5));
	vec.push_back(new Aviao("Aviao11", 23, 13, 45, 7, 4, 5));
	vec.push_back(new Aviao("Aviao12", 33, 10, 40, 7, 3, 5));
	vec.push_back(new Aviao("Aviao13", 42, 24, 49, 7, 2, 5));

	BNB search;
	search.solve(vec);

	system("PAUSE");
	return 0;
}