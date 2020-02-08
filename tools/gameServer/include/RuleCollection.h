#ifndef RULE_COLLETCION_H
#define RULE_COLLETCION_H

#include <string>
#include <vector>
#include "player.h"

using namespace std; 

typedef bool condition;
using RulesList = const vector<GenRule>;

 // { "rule": "foreach",
 //      "list": "configuration.Rounds.upfrom(1)",
 //      "element": "round",
 //      "rules": [

 //        { "rule": "global-message",
 //          "value": "Round {round}. Choose your weapon!"
 //        },

// _______________________________________________________________//

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
	GenRule(const string &name) : 
		rule_name{name} 
		{};

	string rule_name;
};

template <typename T> 
struct ForEach : GenRule{
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
	Loop(const condition &until, const condition &while, RulesList r) :
	GenRule{"Loop"},
	until{until},
	while{while},
	rules_to_run{r}	
	{};

	condition until;
	condition while; // will this be an issue?
	vector<GenRule> rules_to_run;
};

struct Inparallel : GenRule {
	Inparallel(RulesList r) :
	GenRule{"Inparallel"},
	rules_to_run{r}	
	{};

	vector<GenRule> rules_to_run;
};

template <typename T> 
struct Parallelfor : GenRule{
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