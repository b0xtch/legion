#ifndef ENGINE_H
#define ENGINE_H
#include <string>
#include <json.hpp>
#include <iostream>  
#include <any>
#include <variant>
#include "jsonDSL.h"
#include "RuleCollection.h"
#include "absl/strings"

// for convenience
using json = nlohmann::json;
using String = std::string;
using Integer = int;
using Boolean = bool;
using Key = std::string;
struct Object;
struct Array;

template <typename T> struct rapper {
  rapper(T type) { 
    entities.emplace_back(std::move(type)); 
  }

  // user-defined conversion, i think this migth be an implicit conversion
  // An explicit conversion was tried but i am guessing because i am using templates
  // Its implicit 
  operator T() const { 
    return entities.front(); 
  }

  std::vector<T> entities;
};

template<typename... T>
using Type = std::variant<T...>;
using Value = Type<
  Integer,
  String, 
  Boolean,
  rapper<Array>, 
  rapper<Object>
>;

struct Object {
  std::unordered_map<Key, Value> values;
};

struct Array {
  std::vector<Value> values;
};

namespace Engine {

    /**
     * Support types 
     */
    template <typename K, typename V> 
    struct GenType {
        std::unordered_map<K, V> map;
        K key;
        V value;
    };

    Value recursiveValueMap(const json& json) {
        if(json.is_string()){
            return (Value) (String) json;
        }else if(json.is_number()){
            return (Value) (Integer) json;
        }else if(json.is_boolean()){
            return (Value) (Boolean) json;
        }else if (json.is_object()) {
            Object map;
            for(const auto&[key, value]: json.items()) {
            map.values.emplace(key, recursiveValueMap(value));
            }
            return (Value) map;
        }else if(json.is_array()){
            Array arr;
            for(const auto&[key, value]: json.items()) {
            arr.values.emplace_back(recursiveValueMap(value));
            }
            return (Value) arr;
        }
    }

    /**
     * Rules
     */

    struct Interpreter {
        void operator()(std::monostate) const { }
        void operator()(const String &str) const { }
        void operator()(const Integer num) const { }
        void operator()(const Array &array) const { }
        void operator()(const Object &object) const { }
    };

    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>; 

    template<typename... T>
    struct Components{
        using component = std::variant<T...>;

        void visit(){
            for (auto& entity : entities){
                std::visit(overloaded {
                    [](Integer& value){value += value;},
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
        using KindPair = std::pair<Integer, Integer>;
        
        Arithmetic(KindPair values, JsonDSL::Arithmetic operation)
        : values(values),
          operation(operation)
          {}

        Add add; // placeholder we might not need a Rulecollection for this
        KindPair values;
        JsonDSL::Arithmetic operation;
        Integer result;
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
     * Main game configuration
    */

    struct PlayerCount {
        Integer min;
        Integer max;
    };

    struct Constants {
        Object constants;
    };

    struct Variables {
        Object variables;
    };

    struct PerPlayer {
        Object perPlayer;
    };

    struct PerAudience {
        Object perAudience;
    };

    struct Configuration {
        String name;
        PlayerCount playerCount; // an object type can be used here i guess
        bool audience;
        Object setup;
    };

    /**
     * Main game component
    */
    struct Game {
        Configuration configuration;
        Constants constants;
        Variables variables;
        PerPlayer perPlayer;
        PerAudience perAudience;
        Rules rules;
    };

    template <typename T> 
    class EngineImpl { 
        public:
            EngineImpl (const T& input);
            GenType<String, Game> getGameConfig() const noexcept;
            GenType<String, Game> initalizeEngine();

        private:
            T input;
            GenType<String, Game> gameConfig;

            // Domain level set functions, these should never throw if we do our validation correctly
            Configuration setConfiguration(const T& configuration) const noexcept;
            Constants setConstants(const T& constants) const noexcept;
            Variables setVariables(const T& variables) const noexcept;
            PerPlayer setPerPlayer(const T& perPlayer) const noexcept;
            PerAudience setPerAudience(const T& perAudience) const noexcept;
            Rules setRules(const T& rules) const noexcept;

            // Parser Related methods
            bool validGameConfig(const T& input);
            GenType<String, Game> buildGame();
            void mapKeyToValue(const T& key, const T& value);
            T mapValueToFuntion(const T& value);

            // Game related methods
            void findAndExecute(/* find a specific function and execute dynamically*/);
    };

    /*************************************
    *
    *     RuleCollection Implemention  
    *
    **************************************/

    void forEachImpl(RuleCollection::ForEach &);
}

#endif
