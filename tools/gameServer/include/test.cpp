#include "RuleCollection.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
	

	GenRule a("idk");
	GenRule b("lmao");
	GenRule c("rule");

	vector<int> list = {1,2,3,4,5};
	int element = 3;
	std::vector<GenRule> rules_list = {a,b,c};

	condition x = true;
	condition y = false;

	ForEach d(list, element, rules_list);
	Loop e(x,y,rules_list);
	Inparallel f(rules_list);
	Parallelfor g(list, element, rules_list);

	vector<GenRule> v = {d,e,f,g};


	std::cout << "SUCCESS!" << endl;
	return 0;

	
}