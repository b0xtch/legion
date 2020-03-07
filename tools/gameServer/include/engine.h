#ifndef ENGINE_H
#define ENGINE_H
#include <string>
#include <json.hpp>
#include <iostream>  
#include <any>
#include <variant>
#include "jsonDSL.h"
#include "RuleCollection.h"

// for convenience
using json = nlohmann::json;
using String = std::string;
using Integer = int;
using Boolean = bool;
using Key = std::string;
using Object = std::map<std::string, std::variant<Integer, String, Boolean>>;
using Array = std::vector<std::variant<Integer, String, Boolean>>;

template<typename... T>
using Type = std::variant<T...>;
using Value = Type<
  Integer,
  String, 
  Boolean,
  Object,
  Array
>;

// ATTEMPT A RECURSIVE W/O boost
// template<typename... T>
// using Type = std::variant<T...>;
// struct RecursiveVariant;
// using Value = Type<
//   Integer,
//   String, 
//   Boolean,
//   std::vector<RecursiveVariant>,
//   std::unique_ptr<RecursiveVariant>,
//   std::unordered_map<Key, std::unique_ptr<RecursiveVariant>>
// >;
// struct RecursiveVariant {
//   Value values;
// };
// using Object = std::unordered_map<Key, Value>;
// using Array = std::vector<Value>;

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
     * Rules
     */

    // might delete as this is redunt now that componets are just rules now
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
                        switch (rule.operation) {
                            case JsonDSL::Arithmetic::ADD:      rule.result = rule.values.first + rule.values.second; break;
                            case JsonDSL::Arithmetic::SUBTRACT: rule.result = rule.values.first - rule.values.second; break;
                            case JsonDSL::Arithmetic::MULTIPLY: rule.result = rule.values.first * rule.values.second; break;
                            case JsonDSL::Arithmetic::DIVIDE:   rule.result = rule.values.first / rule.values.second; break;
                            default: break;
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

    struct ControlStructures {
        ForEach forEach;
        Loop loop;
        Inparallel inparallel;
        Parallelfor parallelfor;
        Switch switch;
        When when;
    };

    struct ListOperations {
        ListOP listOp;
        Deal deal;
        Discard discard;
    };

    struct Arithmetic {
        // This type accept a pair of number i can see might need to 
        // single values which we can easily support for now its a std::pair
        using KindPair = std::pair<int, int>;
        
        Arithmetic(KindPair values, JsonDSL::Arithmetic operation)
        : values(values),
          operation(operation)
          {}

        Add add; // placeholder we might not need a Rulecollection for this
        KindPair values;
        JsonDSL::Arithmetic operation;
        int64_t result;
    };

    struct Timing{
        Timer timer;
    };

    struct HumanInput{};

    struct Output {
        GlobalMessage globalMessage;
        ScoreBoard scoreboard;
        Scores scores;
    };

    struct Rules {
        ControlStructures controlStructures;
        ListOperations listOperations;
        Arithmetic arithmetic;
        Timing timing;
        HumanInput humanInput;
        Output output;
    }; 

    /**
     * Environment Types
    */

    struct PlayerCount {
        int64_t min;
        int64_t max;
    };

    struct CVPA
        : GenType<std::string, Components<std::string, int64_t, bool> > {
        // constants, variables, perPlayer, perAudience are the same
        GenType constants;
        GenType variables;
        GenType perPlayer;
        GenType perAudience;
    };

    /**
     * Main game configuration
    */

    struct Setup {

    };

    struct Configuration {
        std::string name;
        PlayerCount playerCount;
        bool audience;
        Setup setup;
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
            GenType<std::string, Game> getGameConfig() const noexcept;
            GenType<std::string, Game> initalizeEngine();

        private:
            T input;
            GenType<std::string, Game> gameConfig;

            // Domain level set functions, these should never throw if we do our validation correctly
            Configuration& setConfiguration(const T& configuration) const noexcept;
            CVPA& setConstants(const T& constants) const noexcept;
            CVPA& setVariables(const T& variables) const noexcept;
            CVPA& setPerPlayer(const T& perPlayer) const noexcept;
            CVPA& setPerAudience(const T& perAudience) const noexcept;
            Rules& setRules(const T& rules) const noexcept;

            // Parser Related methods
            bool validGameConfig(const T& input);
            GenType<std::string, Game> buildGame();
            void mapKeyToValue(const T& key, const T& value);
            T mapValueToFuntion(const T& value);

            // Game related methods
            void findAndExecute(/* find a specific function and execute dynamically*/);
    };

}

#endif