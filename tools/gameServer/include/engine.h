#ifndef ENGINE_H
#define ENGINE_H
#include <string>
#include <json.hpp>
#include <iostream>  
#include <any>
#include <variant>
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
 * Map of string to structs => <"configuratin" -> configuration (type struct)>
*/

namespace Engine {

    /**
     * Support types 
     */
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
     * rules
     */

    struct ControlStructures {};

    struct ListOperations {};

    struct Arithmetic {
        using KindPair = std::pair<int, int>;
        
        Arithmetic(KindPair values, JsonDSL::Arithmetic operation)
        : values(values),
          operation(operation)
          {}

        KindPair values;
        JsonDSL::Arithmetic operation;
        int64_t result;
    };


    struct Timing {};

    struct HumanInput {};

    struct Output {};

    struct Rules {
        ControlStructures controlStructures;
        ListOperations listOperations;
        Arithmetic arithmetic;
        Timing timing;
        HumanInput humanInput;
        Output output;
    };

    /**
     * Interpreter for rules
    */
    template<typename E, typename A> // switch this to accept an varidic args
    struct Interpreter {
        Interpreter(const E& type): type(type) {}

        // template <typename T>
        // auto operator()(T&& value){
        //     component.entities.emplace_back(value);
        //     component.visit(arithmetic<E, T>{type});
        // }

        // Components<A> component;
        E type;
    };

    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>; 

    template<typename... T>
    struct Components{
        using component = std::variant<T...>;

        void visit(){
            for (auto& entity : entities){
                std::visit(overloaded {
                    [](int& value){value += value;},
                    // [](ControlStructures rule){},
                    // [](ListOperations rule){},
                    [](Arithmetic rule){
                        switch (rule.operation)
                        {
                        case ADD:
                            rule.result = rule.values.first + rule.values.second;
                            std::cout << rule.result << std::endl;
                            break;
                        case SUBTRACT:
                            rule.result = rule.values.first - rule.values.second;
                            std::cout << rule.result << std::endl;
                            break;
                        case MULTIPLY:
                            rule.result = rule.values.first * rule.values.second;
                            std::cout << rule.result << std::endl;
                            break;
                        case DIVIDE:
                            rule.result = rule.values.first / rule.values.second;
                            std::cout << rule.result << std::endl;
                            break;
                        default:
                            break;
                        }
                    },
                    // [](Timing rule){},
                    // [](HumanInput rule){},
                    // [](Output rule){}
                }, entity);
            }
        }
        
        template <typename V>
        void visit(V&& visitor){
            for (auto& entity : entities){
                std::visit(visitor, entity);
            }
        }

        std::vector<component> entities;
    };      

    struct PlayerCount {
        int64_t min;
        int64_t max;
    };

    // testing only
    enum SetupTypes {
        INTEGER,
        STRING,
        BOOLEAN,
        Q_A,
        M_C
    };

    // {
    //   "kind": <<data kind>>,
    //   "prompt": <<Description of what data the owner should provide>>
    // }
    struct Setup {
        Components<
            GenType<SetupTypes, std::string_view>, 
            int, 
            std::string_view, 
            bool
        > setup;
    };

    struct CVPA
        : GenType<std::string_view, Components<std::string_view, int64_t, bool> > {
        // constants, variables, perPlayer, perAudience are the same
        GenType constants;
        GenType variables;
        GenType perPlayer;
        GenType perAudience;
    };

    struct Configuration {
        std::string_view name;
        PlayerCount* playerCount;
        bool audience;
        Setup* setup;
    };

    /**
     * Main game component
    */
    struct Game {
        Configuration configuration;
        CVPA cvpa;
        Rules rules;
    };

    template <typename T> 
    class EngineImpl { 
        public:
            EngineImpl (const T& input);
            GenType<std::string_view, Game> getGameConfig() const;
            GenType<std::string_view, Game> initalizeEngine();

        private:
            T input;
            GenType<std::string_view, Game> gameConfig;

            // Domain level set functions
            Configuration& setConfiguration(const T& configuration);
            CVPA& setConstants(const T& constants);
            CVPA& setVariables(const T& variables);
            CVPA& setPerPlayer(const T& perPlayer);
            CVPA& setPerAudience(const T& perAudience);
            // Rules& setRules(const T& rules);

            // Parser Related methods
            bool validGameConfig(const T& input);
            GenType<std::string_view, Game> buildGame();
            void mapKeyToValue(const T& key, const T& value);
            T mapValueToFuntion(const T& value);

            // Game related methods
            void findAndExecute(/* find a specific function and execute dynamically*/);
    };

}

#endif