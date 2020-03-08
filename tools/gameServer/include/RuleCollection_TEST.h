#ifndef RULE_COLLECTION_H
#define RULE_COLLECTION_H

#include <string>
#include <vector>
#include <assert.h>
#include <map>
#include <iostream>

namespace RuleCollection {

	class GenRule{
	public:
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

	enum ConditionType{
	    EQUAL,
	    GREATER,
	    LESS,
	    AND,
	    OR
	};

	// helper for loop
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
	class Loop : public GenRule {
	public:
		Loop(Condition<T> con, LoopType type, RulesList &r) :
		GenRule{"Loop"},
		loopCondition{con},
		type{type},
		rules_to_run{r}	
		{};
		
		Condition<T> loopCondition;
		LoopType type;
		RulesList rules_to_run;


		void func() override{
			std::cout << "Looping ..." << std::endl;
		    switch(type)
		    {
		        case LoopType::UNTIL:
		        	std::cout << "UNTIL ..." << std::endl;
		            while(!loopCondition()){
		                for(auto rule : rules_to_run){
		                	std::cout<<rule->rule_name <<std::endl;
		                    rule->func();
		                    if(loopCondition()) break;
		                }
		            }
		            break;
		        case LoopType::WHILE:
		        	std::cout << "WHILE ..." << std::endl;
		            while(loopCondition()){
		                for(auto rule : rules_to_run){
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

	class Arithmetic : public GenRule{
	public:
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

}

#endif