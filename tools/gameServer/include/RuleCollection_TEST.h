#ifndef RULE_COLLECTION_H
#define RULE_COLLECTION_H

#include <string>
#include <vector>
#include <assert.h>
#include <map>
#include <iostream>

namespace RuleCollection {

	struct GenRule{
		GenRule(const std::string &name) : 
			rule_name{name} 
			{};

		virtual void func() = 0;

		std::string rule_name;
	};

	using RulesList = std::vector<GenRule*>;

	/*************************************
	*
	*			Control Structures	
	*
	**************************************/

	template <typename T> 
	struct ForEach : GenRule{
		ForEach(std::vector<T> v, T el, RulesList r) : 
			GenRule{"ForEach"},
			list{v},
			element{el},
			rulesToRun{r}
			{};
		
		std::vector<T> list;
		T element;
		RulesList rulesToRun;

		//is this correct????
		// example:
		// for each round, global message broadcast the round number
		// value to be stored in global message struct
		void func() override{
			std::cout << "ForEach ..." << std::endl;
			for(auto listElement : list){
				for(auto rule : rulesToRun){
					rule->func();
				}
			}
		}
	};

	enum ConditionType{
	    EQUAL,
	    GREATER,
	    LESS,
	    AND,
	    OR
	};

	// helper for loop
	//templated so that loop can take any condition (int, floats, etc)
	template <typename T>
	struct Condition {
	    Condition(T &v1, T &v2, ConditionType type) :
	        value1{v1},
	        value2{v2},
	        type{type}
	        {};

	    T& value1;
	    T& value2;
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

	
	typedef int& destination;

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


	// Inparallel
	// Parallel for
	// Switch

	//When
	//look into boost variant
	template <typename T>
	struct Case : GenRule {
		Case(Condition<T> c, RulesList r) :
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

	// struct When : GenRule {

	// };





} // namespace RuleCollection

#endif