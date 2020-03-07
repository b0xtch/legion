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
		GenRule() {};

		virtual void func(){}

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

	struct Condition {
	    Condition(int &v1, int &v2, ConditionType t) :
	        value1{v1},
	        value2{v2},
	        type{t}
	        {};

	    int& value1;
	    int& value2;
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

	class Loop : public GenRule {
	public:
		Loop(Condition con, LoopType type, RulesList &r) :
		GenRule{"Loop"},
		con{con},
		type{type},
		rules_to_run{r}	
		{};
		
		Condition con;
		LoopType type;
		RulesList rules_to_run;


		virtual void func() override{
			std::cout << "Looping ..." << std::endl;
		    switch(type)
		    {
		        case LoopType::UNTIL:
		        	std::cout << "UNTIL ..." << std::endl;
		            while(!con()){
		                for(auto rule : rules_to_run){
		                	std::cout<<rule->rule_name <<std::endl;
		                    rule->func();
		                    if(con()) break;
		                }
		            }
		            break;
		        case LoopType::WHILE:
		        	std::cout << "WHILE ..." << std::endl;
		            while(con()){
		                for(auto rule : rules_to_run){
		                	std::cout << "Running rule " << rule->rule_name <<std::endl;
		                    rule->func();
		                    if(!con()) break;

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
				// v1{v1},
				value{value},
				op{op}
				{};

	    destination to;
	    // int v1;
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