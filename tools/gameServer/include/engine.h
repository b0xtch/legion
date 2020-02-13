#ifndef ENGINE_H
#define ENGINE_H
#include <string>
#include <json.hpp>
#include <iostream>  
#include <any>
#include <variant>
// #include "jsonDSL.h"

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
 * Map of string to structs => <"configuratin" -> configuration (type struct)>
*/

namespace Engine {


    // logic
    // ===============================================================

    //not sure if this is good design, needs to do more research. A few things depend on this type now
    template <typename K, typename V> 
    struct GenType {

        // K set(const G& key, const G& value) const {
        //     map[key] = value;
        // }

        // V get(const G& key) const {
        //     return map[key];
        // }

        std::unordered_map<K, V> map;
        K key;
        V value;
    };

    /**
     * Each individual rule is a map of attributes describing the rule. 
     * Lists of rules define a sequence of operations in which each rule 
     * must be performed in sequential order.
    */
    template<typename T, typename... Args>
    struct Rules: GenType<T, Args> {
        std::string add {"rules"};

        // List of all the rules under Rules struct\
        // Control Structures
        struct ControlStructures {};

        // List Operations
        struct ListOperations {};

        // Arithmetic Operations -> example provided using the arithmetic struct provided above
        struct Arithmetic {}; // basically the one below this main struct

        // Timing
        struct Timing {};

        // Human Input
        struct HumanInput {};

        // Output
        struct Output {}; 
    };

    // mostly for arithmetic operations
    template<typename E, typename A>
    struct arithmetic {
        arithmetic(const E& type): type(type) {}
        
        template<typename T>
        auto& operator()(const T& value) {
            switch (type) {
                case upFrom:
                    value++;
                    break;
                case downFrom:
                    value--;
                    break;
            }
        }

        auto& operator()(const A& value){
            switch (type) {
                case add:
                    std::cout << value.first + value.second << std::endl;
                    return value;
                    break;
            }
        }

        E type;
    };

    template<typename E, typename A> // switch this to accept an varidic args
    struct Interpreter {
        Interpreter(const E& type): type(type) {}

        template <typename T>
        auto operator()(T&& value){
            component.entities.emplace_back(value);
            component.visit(arithmetic<E, T>{type});
        }

        Components<A>* component;
        E type;
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
                    [](JsonDSL::SpecificationFields value){
                        std::cout << "SpecificationFields" << std::endl;
                    },
                    [](JsonDSL::ConfigFields value){
                        std::cout << "ConfigFields" << std::endl;
                    },
                    [](JsonDSL::RuleType value){
                        std::cout << "RuleType" << std::endl;
                    },
                    [](JsonDSL::RuleParameters value){
                        std::cout << "RuleParameters" << std::endl;
                    },
                    [](JsonDSL::TimerModes value){
                        std::cout << "TimerModes" << std::endl;
                    },
                    [](JsonDSL::SetupFields value){
                        std::cout << "Upload your json!" << std::endl;
                        //Setup( /* map[ make_pair("kind", json ] = "the prompt"; */ );
                    }
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
        struct PlayerCount {
        int min;
        int max;
    };

    // {
    //   "kind": <<data kind>>,
    //   "prompt": <<Description of what data the owner should provide>>
    // }
    using KindPair = std::pair<std::string, std::string>; // TODO this bad
    struct Setup: GenType<std::string, KindPair> {
        Setup(const KindPair &param)
            : Setup(param) {};
    };

    struct CVPA
        : GenType<std::string, Components<std::string, int, bool> >
    {
        // constants, variables, perPlayer, perAudience are the same
        GenType constants;
        GenType variables;
        GenType per_player;
        GenType per_audience;
    };

    struct Configuration {
        std::string name;
        PlayerCount* playecount;
        bool audience;
        Setup* setup;

    } configuration;

    struct Game {
        Components<
        Configuration,
        CVPA
        // Rules don't even touch rules now
        > components;
    };

    template <typename T> 
    class EngineImpl { 
        public:
            EngineImpl (T& input);
            Game getGameConfig() const;

            Game initalizeEngine();

        private:
            T input;
            GenType<std::string, Game> gameConfig;

            // Domain level set functions
            Configuration& setConfiguration(const T& configuration);
            CVPA& setConstants(const T& constants);
            CVPA& setVariables(const T& variables);
            CVPA& setPerPlayer(const T& perPlayer);
            CVPA& setPerAudience(const T& perAudience);
            // Rules& setRules(const T& rules);

            // Parser Related methods
            bool validGameConfig(T& input);
            Game buildGame();
            void mapKeyToValue(T& key, T& value);
            T mapValueToFuntion(T& value);


            // Game related methods
            void findAndExecute(/* find a specific function and execute dynamically*/);
    };

}

#endif