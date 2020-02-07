#ifndef ENGINE_H
#define ENGINE_H
#include <string>
#include <json.hpp>
#include <iostream>  
#include <any>

using namespace std; 

// The main work is to split the string into the three components 
// "player", ".", and "count" and turn these into instructions: "player" => look up 
// "player" key; "." => expect it to be an inner map/object; "count" => look up 
// "count" key in inner map/object

// these enums are here for testing ill move them out once I am done
enum SetupTypes{
    Kind, SetupPrompt,
    KindInteger, KindString, KindBoolean,
    KindQuestionAnswer, KindMultipleChoice  
};

enum RuleTypes{
    Foreach, Loop, InParallel, Parallelfor,
    Switch, When, Extend, Reverse, Shuffle, 
    Sort, Deal, Discard, Add, Timer, 
    InputChoice, InputText, InputVote, 
    Message, GlobalMessage, Scores
};

template <typename G> 
struct GenType {
    GenType() : map() {};
    GenType(std::map<G, std::any> &param) : map(param) {};
    GenType(const G &param) : value(param) {};

    std::map<G, std::any> map;
    G value;
};

// {
//   "kind": <<data kind>>,
//   "prompt": <<Description of what data the owner should provide>>
// }
struct Setup: GenType<std::pair<std::string, std::any>> {
    Setup(const std::pair<std::string, std::any> &param) // change the any to json
        : GenType(param) {}; 

    // overload the cout to be able to cout a custom thing like the prompt for example
};


struct Interpreter {
    auto operator()(int& _in){_in += _in;}
    auto operator()(double& _in){_in += _in;}
    auto operator()(std::string& _in){_in += _in;}
    auto operator()(bool& ){std::cout << "bool item" << std::endl;}
    auto operator()(RuleTypes value){std::cout << "rules item" << " " << RuleTypes(value) << std::endl;}
    auto operator()(SetupTypes value){
        std::cout << "Upload your json!" << std::endl;

        //Setup( /* map[ make_pair("kind", json ] = "the prompt"; */ );
    }
};

template<typename... T>
struct Components{
    using component = std::variant<T...>;

    template<typename V>
    void visit(V&& visitor){
        for (auto& entity : entities){
            std::visit(visitor, entity);
        }
    }

    // Create a vector of variants
    std::vector<component> entities;
};


template <typename T> 
class Engine{ 
    public:
        Engine (T input);

        bool validGameConfig(T input);
        GenType<T> getGameConfig();

    private:
        T input;
        GenType<T> gameConfig;

        void initalizeEngine();
        void buildGame();
        void mapKeyToValue(T key, T value);
        T mapValueToFuntion(T value);

        // Control Structure Methods
        void findAndExecute(/* find a specific function and execute dynamically*/);
};

#endif