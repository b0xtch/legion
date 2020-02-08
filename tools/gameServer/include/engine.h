#ifndef ENGINE_H
#define ENGINE_H
#include <string>
#include <json.hpp>
#include <iostream>  
#include <any>
#include "jsonDSL.h"

using namespace std; 

// The main work is to split the string into the three components 
// "player", ".", and "count" and turn these into instructions: "player" => look up 
// "player" key; "." => expect it to be an inner map/object; "count" => look up 
// "count" key in inner map/object
/**
 * 
 * Create a way to output from each struct type
 * Implement the rest of the JsonDsl types just put them into lambda functions with (ENUM& )
 * change any type in setup and GenType struct
 * define a desgin of hiercarhy
*/

namespace Engine {

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
        using KindPair = std::pair<std::string, std::any>;

        Setup(const KindPair &param) // change the any to json
            : GenType(param) {}; 

        // overload the cout to be able to cout a custom thing like the KindPair for example
    };

    // mostly for arithmetic operations
    template<typename E>
    struct arithmetic {
        arithmetic(const E& value): value(value) {}
        auto operator()(Arithmetic& type){
            switch (type){
                case upFrom:
                    std::cout << "adding one" << std::endl;
                    value += 1;
                    break;
                case downFrom:
                    std::cout << "minus one" << std::endl;
                    value -= 1;
                    break;
                default:
                    break;
            }
        }

        E value;
    };

    template<typename E>
    struct Interpreter {
        Interpreter(const E& value){
            component.entities.emplace_back(value);
        }

        template <class T>
        auto operator()(T&& value){
            component.visit(arithmetic<T>{value});
        }

        Components<E> component;
    };

    /**
     * Convert my interpreter into self executing lambda functions
     * from this cool article on dev
     * still grasping how to use variants for each component of our game
     * 
     * https://dev.to/tmr232/that-overloaded-trick-overloading-lambdas-in-c17
    */

    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>; 

    template<typename... T>
    struct Components {
        using component = std::variant<T...>;

        // These are the main types that we are going to 
        // have and translate to from type T
        // JsonDSL::SpecificationFields;
        // JsonDSL::ConfigFields;
        // JsonDSL::RuleType;
        // JsonDSL::RuleParameters;
        // JsonDSL::TimerModes;
        // JsonDSL::SetupFields;

        void visit(){
            for (auto& entity : entities){
                std::visit(overloaded {
                    [](int& value){value += value;},
                    [](double& value){value += value;},
                    [](std::string& value){std::cout << "bool item" << std::endl;},
                    [](bool& ){std::cout << "bool item" << std::endl;},
                    [](JsonDSL::RuleType value){
                        std::cout << "rules item" << " " << RuleTypes(value) << std::endl;
                    },
                    [](JsonDSL::SetupFields value){
                        std::cout << "Upload your json!" << std::endl;

                        //Setup( /* map[ make_pair("kind", json ] = "the prompt"; */ );
                    }
                    // .. add th rest once I have clear understanding of whats happening
                }, entity);
            }
        }

        // overlaoded function to take a cutom visitor i.e struct interpreter
        template <typename V>
        void visit(V&& visitor){
            for (auto& entity : entities){
                std::visit(visitor, entity);
            }
        }

        // Create a vector of variants
        std::vector<component> entities;
    };


    template <typename T> 
    class EngineImpl { 
        public:
            EngineImpl (T& input);

            bool validGameConfig(T& input);
            GenType<T> getGameConfig();

        private:
            T input;
            GenType<T> gameConfig;

            void initalizeEngine();
            void buildGame();
            void mapKeyToValue(T& key, T& value);
            T mapValueToFuntion(T& value);

            // Control Structure Methods
            void findAndExecute(/* find a specific function and execute dynamically*/);
    };

}

#endif