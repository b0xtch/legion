#ifndef RULE_COLLECTION_H
#define RULE_COLLECTION_H

#include <string>
#include <vector>
#include <assert.h>
#include <map>

// #include "player.h"

using namespace std; 

typedef bool condition;
typedef int destination;
typedef int source;
typedef int seconds;
typedef string mode;
typedef string message;

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
	GenRule() {};

	string rule_name;
};

using RulesList = vector<GenRule>;

//Visitor struct to support variant visit
template<class... Ts> struct overloaded : Ts... {using Ts::operator()...;};
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

//Visitors struct is based off of Mahmoud's (Botch's) work in engine.h
template<typename ...Ts>
struct Visitors : Ts...{
	Visitors(const Ts&... args) : Ts(args)... {}
	using Visitor = std::variant<T...>;

	void visit(){
		for(auto& entity : entities){
			std::visit(overloaded{
				[](Parallelfor rule){
					rule.parallel;
				}
				[](Foreach rule){}
				[](Loop rule){}
				[](Inparallel rule){}
				[](Switch rule){}
				[](When rule){}
				//... TODO: Add on more rules and their features
			}, entity);
		}
	}

	template<typename V>
	void visit(V&& visitor){
		for(auto& entity : entities){
			std::visit(visitor, entity);
		}
	}

	std::vector<Visitor> entities;
};

template<typename ...Ts>
auto make_visitor(Ts... lambdas){
	return Visitors<Ts...>(lambdas...);
}


/*************************************
*
*			Control Structures	
*
**************************************/

template <typename T> 
struct ForEach : GenRule{
	ForEach(vector<T> v, T el, RulesList r) : 
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
	Loop(const condition &untilCondition, const condition &whileCondition, RulesList &r) :
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
	Parallelfor(const vector<T> &v, const T &el, RulesList &r) : 
		GenRule{"Parallelfor"}, 
		list{v},
		element{el},
		rules_to_run{r}
		{};
	
	void parallel(){
		for(GenRule rule : rules_to_run){
			for(element : list){
				Visitors(rule);
				//TODO
			}
		}
	}
	vector<T> list;
	T element;
	vector<GenRule> rules_to_run;
};

template <typename T> 
struct Switch : GenRule {
	Switch() {};
	Switch(T &value, const vector<T> v, RulesList &cases) :
		GenRule{"Switch"},
		value{value},
		list{v},
		cases{cases}
		{};

	T value;
	vector<T> list;
	vector<GenRule> cases;
};

struct When : GenRule {
	When() {};
	When(const condition &c, RulesList &r) :
		GenRule{"When"},
		runCondition{c},
		rules_to_run{r}
		{};

	condition runCondition;
	vector<GenRule> rules_to_run;
};

/*************************************
*
*				Lists	
*
**************************************/

// List operations
// for extend, reverse, shuffle, sort
template <typename T> 
struct ListOP : GenRule{
	ListOP() {};
	ListOP(string name, const T *target, vector<T> &list) : 
		GenRule {name},
		target {target},
		list {list}
		{
			//only extend utilizes a target
			if(name != "Extend"){
				assert(target == nullptr);
			}
		};

	T target;
	vector<T> list;
};

template <typename T> 
struct Deal : GenRule{
	Deal() {};
	Deal(vector<T> &from, vector<T> &to, int count) :
		GenRule{"Deal"},
		from{from},
		to{to},
		count{count}
		{};

	vector<T> from, to;
	int count;
};

template <typename T> 
struct Discard : GenRule{
	Discard() {};
	Discard(vector<T> &from, int count) :
		GenRule{"Discard"},
		from{from},
		count{count}
		{};

	vector<T> from;
	int count;
};

// list attributes
// struct ListAttribute {
// 	//WIP
// }


/*************************************
*
*		Arithmatic Operation	
*
**************************************/

struct Add : GenRule{
	Add() {};
	Add(destination &to, source &from) :
		GenRule{"Add"},
		to{to},
		from{from}
		{};

	destination to;
	source from;
};

/*************************************
*
*				Timing	
*
**************************************/

struct Timer : GenRule {
	Timer() {};
	Timer(seconds &s, mode &mode, RulesList &r, condition *flag) :
		GenRule{"Timer"},
		seconds{s},
		mode{mode},
		rules_to_run{r},
		flag{flag}
		{
			// if(mode != "track"){
			// 	assert flag == nullptr;
			// }
		};

	seconds seconds;
	mode mode;
	vector<GenRule> rules_to_run;
	condition flag;
};


/*************************************
*
*			Human Input	
*
**************************************/

// template <typename T> 
// struct InputText : GenRule {
// 	InputText() {};
// 	//change to from type Player to type accepting both audience and player
// 	InputText(Player *to, const message &prompt, T &ret, seconds &timeout) :
// 		GenRule{"InputText"},
// 		to{to},
// 		prompt{prompt};
// 		ret{ret},
// 		timeout{timeout}
// 		{};

// 	Player *to;
// 	message prompt;
// 	T ret;
// 	seconds timeout;
// };

// template <typename T, typename Y> 
// struct InputVote : GenRule {
// 	InputVote() {};
// 	//change to from type Player to type accepting both audience and player
// 	InputVote(
// 		const vector<Player> &to, 
// 		const message &prompt, 
// 		const vector<T> &choices, 
// 		Y &ret,
// 		seconds &timeout) :

// 		GenRule{"InputVote"},
// 		to{to},
// 		prompt{prompt};
// 		ret{ret},
// 		timeout{timeout}
// 		{};

// 	vector<Player> to;
// 	message prompt;
// 	vector<T> choices;
// 	Y ret;
// 	seconds timeout;
// };

/*************************************
*
*			Output	
*
**************************************/

// struct Message : GenRule {
// 	Message() {};
// 	//need to support python style () later
// 	Message(const vector<Player> &to, const message &value) :
// 		GenRule{"Message"},
// 		to{to},
// 		value{value}
// 		{};

// 	vector<Player> to;
// 	message value;
// };

struct GlobalMessage : GenRule {
	GlobalMessage() {};
	//need to support python style () later
	GlobalMessage(const message &value) :
		GenRule{"GlobalMessage"},
		value{value}
		{};

	message value;
};

//WIP
class ScoreBoard {
public:
	// addScore();
	// getScore();
	ScoreBoard();

private:
	// map<Player, int> scoreboard;
};

struct Scores : GenRule {
	Scores() {};
	Scores(ScoreBoard &s, const condition &asc) :
		GenRule{"Scores"},
		scoreboard{s},
		ascending{asc} // false -> desc
		{};

	ScoreBoard scoreboard;
	condition ascending;
};

#endif