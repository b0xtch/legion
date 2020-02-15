#include <string>
#include <vector>
#include <iostream>


#include "RuleCollection.h"

using namespace std; 


struct testPlayer {
	testPlayer(string name, int id) : name{name}, id{id} {};
	testPlayer() : name{"element_place_holder"}, id{0} {};
	
	string name;
	int id;
};

int main(int argc, char const *argv[])
{
	//list operation lists
	vector<string> from = {"A", "B", "C", "D", "E", "F", "G"};
	vector<string> to;

	//constructing rules
	Deal deal1(from, to, 2);
	Discard discard1(from, 1);

	vector<GenRule> rules_to_run = {deal1, discard1};

	//players
	testPlayer p1("Alan", 1);
	testPlayer p2("Bob", 2);
	testPlayer p3("Charlie", 3);
	testPlayer p4("Dennis", 4);

	testPlayer element_place_holder("idk", 123);

	vector<testPlayer> players = {p1, p2, p3, p4};

	ForEach<testPlayer> foreach1(players, element_place_holder, rules_to_run);

	//yet to write ForEach function
	for(GenRule rule : foreach1.rules_to_run){
		std::cout << "process rule: " << rule.rule_name << std::endl;
	}

	for(testPlayer p : foreach1.list){
		std::cout << "players in foreach1: " << p.name << "ID: " << p.id << std::endl;
	}

	return 0;
}