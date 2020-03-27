#ifndef RULE_COLLECTION_H
#define RULE_COLLECTION_H

#include <string>
#include <vector>
#include <assert.h>
#include <map>
#include <chrono>


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

	// ForEach


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
		
		// void parallel(){
		// 	for(GenRule rule : rules_to_run){
		// 		for(element : list){
		// 			Visitors(rule);
		// 			//TODO
		// 		}
		// 	}
		// }

		
		void func(){
			// for(GenRule rule : rules_to_run){
			// 	std::for_each(std::execution::seq, std::begin(list), std::end(list), [&](auto object){
			// 		Engine::Interpreter::operator(object); //visit each and execute sequentially the objects inside parallelfor
			// 		//TODO Add Test
			// 	});
			// }

		}
		vector<T> list;
		T element;
		vector<GenRule> rules_to_run;
	};


	// helper for when
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
	*				Lists	
	*
	**************************************/

	// printList ... just to print for testing purposes
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

		void func(){
			for(int i = 0; i < count; i++){
				from.pop_back();
			}
		}

		vector<T> from;
		int count;
	};

	// list attributes
	// struct ListAttribute {
	// 	//WIP
	// }


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
	*				Timing	
	*
	**************************************/

	enum TimerMode {
		EXACT,
		AT_MOST,
		TRACK
	};

	typedef std::chrono::seconds seconds;

	struct Timer : GenRule {
		Timer() {};
		Timer(seconds &s, mode mode, RulesList &r, condition *flag) :
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
		
		void func(){
			std::vector<GenRule>::iterator iterator = std::begin(rules_to_run);
			auto startTime = std::chrono::high_resolution_clock::now();
			auto endTime = startTime + seconds;
			auto newEndTime;
			bool conditionToRun = startTime - endTime >= std::chrono::milliseconds(0);
			switch(mode){
				case TimerMode::AT_MOST: 
					while(conditionToRun){
						Engine::Interpreter:operator()(*iterator);
						if((iterator++) == std::end(rules_to_run)) break; std::cout << "The operation ended early\n";
					}
				case TimerMode::EXACT:
					while(conditionToRun){
						Engine::Interpreter::operator()(*iterator);
						if((iterator++) == std::end(rules_to_run) && conditionToRun)
							sleep(std::chrono::high_resolution_clock::now() - endTime); //in case the operation ended early
					}
				case TimerMode::TRACK:
					std::for_each(std::begin(rules_to_run), std::end(rules_to_run), [&](GenRule rule){
						Engine::Interpreter::operator()(rule);
					});
					newEndTime = std::chrono::high_resolution_clock::now();
					std::cout << "Track result: " << newEndTime - startTime << std::endl;
			}
		}

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

	typedef std::string message;

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

	/*************************************
	*
	*				Scores
	*
	**************************************/

	//vector of references to players (allows for scores to be updated)
	using playerVector = std::vector<std::reference_wrapper<Player>>;
	struct Scores : GenRule {
		//default ascending = false
		Scores(playerVector p) :
			GenRule{"Scores"},
			playerList{p},
			ascending{false}
			{};

		Scores(playerVector p, bool asc) :
			GenRule{"Scores"},
			playerList{p},
			ascending{asc} // false -> desc
			{};

		void add(Player &p){
			auto result = std::find(playerList.begin(), playerList.end(), p);
			if(result != playerList.end()){
				playerList.push_back(p);
			}
		}

		void remove(Player &p){
			auto result = std::find(playerList.begin(), playerList.end(), p);
			if(result != playerList.end()){
				playerList.erase(result);
			}
		}

		void func(){

			// clear scoresPairs
			scoresPairs.clear();
			
			//make playerList vector elements into pairs (points, name)
			for(auto p : playerList){

				//p.get because p is wrapped with type -> std::reference_wrapper
				auto tmpPair = std::make_pair(	p.get().getPlayerPoints(), 
												p.get().getPlayerName());

				scoresPairs.push_back(tmpPair);
			}

			//sort pairs asc or desc
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

		playerVector playerList;
		std::vector< std::pair <int,pName> > scoresPairs;
		bool ascending;
	};



}//RuleCollection

	
	GenRule() {};

	string rule_name;

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
};


#endif