#ifndef RULE_COLLECTION_TEST_H
#define RULE_COLLECTION_TEST_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <random>
#include <utility>

#include "player.h"

namespace RuleCollection {

	struct GenRule{
		GenRule(const std::string &name) : 
			rule_name{name} 
			{};

		virtual void func() = 0;

		std::string rule_name;
	};

	using RulesList = std::vector<GenRule*>&;

	/*************************************
	*
	*			Control Structures	
	*
	**************************************/

	enum ConditionType{
	    EQUAL,
	    GREATER,
	    LESS,
	    AND,
	    OR
	};

	// helper for loop
	//templated so that loop can compare any types (int, floats, bool, etc)
	// NOTE: value2 is passed by copy to allow literal comparisons
	// LOGIC: value1 [logic sign] value2
	// Example: value1 == value2
	template <typename T>
	struct Condition {
	    Condition(T &v1, T v2, ConditionType type) :
	        value1{v1},
	        value2{v2},
	        type{type}
	        {};

	    T& value1;
	    T value2;
	    ConditionType type;

	    bool operator()() const{
	        switch(type)
	        {
	            case ConditionType::EQUAL:
	                return (value1 == value2);
	            case ConditionType::GREATER:
	                return (value1 > value2);
	            case ConditionType::LESS:
	                return (value1 < value2);
	            case ConditionType::AND:
	                return (value1 && value2);
	            case ConditionType::OR:
	                return (value1 || value2);
	        }
	    }
	};


	enum LoopType{
		UNTIL,
		WHILE
	};

	//templated so that loop can take any condition (int, floats, etc)
	template <typename T>
	struct Loop : GenRule {
		Loop(Condition<T> con, LoopType type, RulesList &r) :
		GenRule{"Loop"},
		loopCondition{con},
		type{type},
		rulesToRun{r}	
		{};
		
		Condition<T> loopCondition;
		LoopType type;
		RulesList rulesToRun;


		void func() override{
			std::cout << "Looping ..." << std::endl;
		    switch(type)
		    {
		        case LoopType::UNTIL:
		        	std::cout << "UNTIL ..." << std::endl;
		            while(!loopCondition()){
		                for(auto rule : rulesToRun){
		                	std::cout<<rule->rule_name <<std::endl;
		                    rule->func();
		                    if(loopCondition()) break;
		                }
		            }
		            break;
		        case LoopType::WHILE:
		        	std::cout << "WHILE ..." << std::endl;
		            while(loopCondition()){
		                for(auto rule : rulesToRun){
		                	std::cout << "Running rule " << rule->rule_name <<std::endl;
		                    rule->func();
		                    if(!loopCondition()) break;

		                }
		            } 
		            break; 
		    }

		}
			

	};


	// Inparallel
	// Parallel for
	// Switch

	//When
	template <typename T>
	struct Case : GenRule {
		Case(Condition<T> &c, RulesList r) :
			GenRule{"Case"},
			whenCondition {c},
			rulesToRun {r}
			{};

		Condition<T> whenCondition;
		RulesList rulesToRun;

		void func() override {
			if(whenCondition()){
				for(auto rules : rulesToRun){
					rules->func();
				}
			}
		}
	};


	template <typename T>
	struct When : GenRule {
		When(std::vector<Case<T>> &c) :
			GenRule{"When"},
			casesToCheck {c} 
			{};

			std::vector<Case<T>> casesToCheck;

			void func() override {
				for(auto currentCase : casesToCheck){
					if(currentCase.whenCondition()){
						std::cout << "Case valid" << std::endl;
						currentCase.func();
						break;
					}
				}
			}

	};

	/*************************************
	*
	*		Arithmetic Operations	
	*
	**************************************/

	
	using destination = int&;

	enum MathOperation {
	    ADD,
	    SUBTRACT,
	    MULTIPLY,
	    DIVIDE
	};

	struct Arithmetic : GenRule{
	    Arithmetic(destination to, int value, MathOperation op) :
		    GenRule{"Arithmetic"},
				to{to},
				value{value},
				op{op}
				{};

	    destination to;
	    int value;
	    MathOperation op;
	    int result;

	    virtual void func() override{
	    	switch(op)
	    	{
	    		case MathOperation::ADD:
	    			std::cout<< "Adding... "<< value << " to " << to <<std::endl;
	    			to += value;
	    			std::cout<< "Resulting value: " << to << "\n" << std::endl;
	    			break;
	    		case MathOperation::SUBTRACT:
	    			std::cout<< "Subtracting... "<< value << " from " << to <<std::endl;
	    			to -= value;
	    			std::cout<< "Resulting value: " << to << "\n" << std::endl;
	    			break;
	    		case MathOperation::MULTIPLY:
	    			std::cout<< "Multiplying... "<< value << " by " << to <<std::endl;
	    			to *= value;
	    			std::cout<< "Resulting value: " << to << "\n" << std::endl;
	    			break;
	    		case MathOperation::DIVIDE:
	    			std::cout<< "Dividing... "<< value << " into " << to <<std::endl;
	    			to /= value;
	    			std::cout<< "Resulting value: " << to << "\n" << std::endl;
	    	}

	    }
	};




	/*************************************
	*
	*			List Operations	
	*
	**************************************/

	//just to print for testing purposes
	template <typename T>
	void printList(std::vector<T> &list){
		for(auto x : list){ std::cout << x << " "; }
		std::cout << std::endl;
	}

	template <typename T>
	struct Extend : GenRule {
		Extend(T v, std::vector<T> &l) :
			GenRule{"Extend"},
			value {v},
			list {l}
			{};

		T value;
		std::vector<T> &list;

		void func() override { 
			list.push_back(value);
			
			std::cout << "\nExtended List: " << std::endl;
			printList<T>(list);
		}
	};

	template <typename T>
	struct Reverse : GenRule {
		Reverse(std::vector<T> &l) :
			GenRule{"Reverse"},
			list {l}
			{};

		std::vector<T> &list;

		void func() override { 
			std::reverse(list.begin(), list.end()); 

			std::cout << "\nReversed List: " << std::endl;
			printList<T>(list);
		}
	};

	template <typename T>
	struct Shuffle : GenRule {
		Shuffle(std::vector<T> &l) :
			GenRule{"Shuffle"},
			list {l}
			{};

		std::vector<T> &list;

		void func() override { 
			std::random_device rand;
			std::shuffle(list.begin(), list.end(), rand); 

			std::cout << "\nShuffled List: " << std::endl;
			printList<T>(list);
		}
	};

	template <typename T>
	struct Sort : GenRule {
		Sort(std::vector<T> &l) :
			GenRule{"Sort"},
			list {l}
			{};

		std::vector<T> &list;

		void func() override { 
			std::stable_sort(list.begin(), list.end()); 

			std::cout << "\nSorted List: " << std::endl;
			printList<T>(list);
		}
	};

	//list attributes

	// dependent on Player.h
	//Deal
	//Discard

	/*************************************
	*
	*				Scores
	*
	**************************************/

	struct  ScoreMap {
		ScoreMap() :
			scoreboard{} {};

		ScoreMap(std::vector<Player> playerList){
				for(auto p : playerList){
					// scoreboard.insert({p.getPlayerPoints(), p.getPlayerName()});
					scoreboard[p.getPlayerName()] = p.getPlayerPoints();
				}
			}

		void add(Player &p){
			scoreboard[p.getPlayerName()] = p.getPlayerPoints();
		}

		void remove(Player &p){
			scoreboard.erase(p.getPlayerName());
		}

		std::map<pName, int> scoreboard;
	};



	struct Scores : GenRule {
		//default ascending = false
		Scores(ScoreMap &s) :
			GenRule{"Scores"},
			scores{s},
			ascending{false}
			{};

		Scores(ScoreMap &s, bool asc) :
			GenRule{"Scores"},
			scores{s},
			ascending{asc} // false -> desc
			{};

		void func(){
			
			//make map key-value elements into pairs
			for(const auto& [key, value] : scores.scoreboard){
				scoresPairs.push_back(std::make_pair(value, key));
			}

			//sort pairs
			if(ascending){
				std::sort(scoresPairs.begin(), scoresPairs.end());
			}
			else{
				std::sort(
					scoresPairs.begin(), 
					scoresPairs.end(), 
					[](const std::pair<int,pName> &a, const std::pair<int,pName> &b){
						return a.first > b.first;
					});
			}

			//print scoreboard
			int i = 1;
			for(auto p : scoresPairs){
				std::cout << i << ") \t" << p.second << "\t\t" << p.first << std::endl;
				i++;
			}

		}

		ScoreMap &scores;
		std::vector< std::pair <int,pName> > scoresPairs;
		bool ascending;
	};


} // namespace RuleCollection

#endif