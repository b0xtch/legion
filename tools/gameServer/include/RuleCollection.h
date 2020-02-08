#ifndef RULE_COLLECTION_H
#define RULE_COLLECTION_H

#include <string>
#include <vector>

using namespace std; 

typedef bool condition;


// enum RuleType{
//     ForEach, Loop, InParallel, Parallelfor,
//     Switch, When, Extend, Reverse, Shuffle, 
//     Sort, Deal, Discard, Add, Timer, 
//     InputChoice, InputText, InputVote, 
//     Message, GlobalMessage, Scores
// };

// enum RuleParameters{
//     List, Element, Until, While, Value, Cases, 
//     Case, Target, Key, From, To, Count, Duration, 
//     Mode, Flag, Timeout, RulePrompt, Result, Choices,
//     Score, Ascending

struct GenRule{
	GenRule() {};
	GenRule(const string &name) : 
		rule_name{name} 
		{};

	string rule_name;
};

using RulesList = const vector<GenRule>;

template <typename T> 
struct ForEach : GenRule{
	ForEach() {};
	ForEach(const vector<T> &v, const T &el, RulesList r) : 
		GenRule{"ForEach"}, //passing it as is?
		list{v},
		element{el},
		rules_to_run{r}
		{};
	
	vector<T> list;
	T element;
	vector<GenRule> rules_to_run;
};

struct Loop : GenRule {
	//will figure out way to only accpet 1 of until or while
	// passing in empty arguments?
	Loop() {};
	Loop(const condition &untilCondition, const condition &whileCondition, RulesList r) :
	GenRule{"Loop"},
	untilCondition{untilCondition},
	whileCondition{whileCondition},
	rules_to_run{r}	
	{};
	
	condition untilCondition;
	condition whileCondition; // will this be an issue?
	vector<GenRule> rules_to_run;
};

struct Inparallel : GenRule {
	Inparallel() {};
	Inparallel(RulesList r) :
	GenRule{"Inparallel"},
	rules_to_run{r}	
	{};
	
	vector<GenRule> rules_to_run;
};

template <typename T> 
struct Parallelfor : GenRule{
	Parallelfor() {};
	Parallelfor(const vector<T> &v, const T &el, RulesList r) : 
		GenRule{"Parallelfor"}, 
		list{v},
		element{el},
		rules_to_run{r}
		{};
	

	vector<T> list;
	T element;
	vector<GenRule> rules_to_run;
};



#endif