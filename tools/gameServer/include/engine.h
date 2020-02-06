#ifndef ENGINE_H
#define ENGINE_H
#include <string>
#include <json.hpp>
#include <iostream>  
#include <any>

using namespace std; 

template <typename G> 
struct GenType {
    GenType() : genMap() {};
    GenType(std::map<G, std::any> &param) : genMap(param) {};
    GenType(const G &param) : genValue(param) {};
    
    // go out of scope when not used
    std::map<G, std::any> genMap;
    G genValue;

    // auto get(std::ostream& o) override{
    //     o << genValue; 
    // }
};

// move to DSL
typedef enum {
    integer,
    string,
    boolean,
    question_answer,
    multiple_choice
} SetupType;

// "kind": <<data kind>>,
// "prompt": ""
struct Setup: GenType<std::any> {
    public:
        Setup () {};
        Setup(const int &number) : GenType(number) {}
        Setup(const std::string &str) : GenType(str) {}
        Setup(const bool &boo) : GenType(boo) {}
        Setup(const std::map<SetupType, std::any> &param) : GenType(param) {}

    private:
        std::string prompt;
};

template <typename T> 
class Engine{ 
    public:
        Engine (T json);

        bool validGameConfig(T json);
        GenType<T> getGameConfig();

    private:
        T json;
        GenType<T> gameConfig;

        void initalizeEngine();
        void buildGame();
        void mapKeyToValue(T key, T value);
        T mapValueToFuntion(T value);

        // Control Structure Methods
        void findAndExecute(/* find a specific function and execute dynamically*/);
};

#endif