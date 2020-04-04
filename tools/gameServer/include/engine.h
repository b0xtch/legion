#ifndef ENGINE_H
#define ENGINE_H
#include <string>
#include <json.hpp>
#include <iostream>  
#include <any>
#include <variant>
#include "jsonDSL.h"
#include "RuleCollection.h"
//#include "absl/strings"
#include "game.h"
#include "player.h"

namespace Engine {
    //vector of all players, 
    //this will also have to connect to server 
    //in order to get realtime results
    using PlayerList = GameImpl::Game::getAllPlayers(); 
    using json       = nlohmann::json;
    using String     = std::string;
    using Integer    = int;
    using Boolean    = bool;
    using Key        = std::string;
    struct Object;
    struct Array;

    template <typename T> 
    struct rapper {
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
        std::map<Key, Value> values;
    };

    struct Array {
        std::vector<Value> values;
    };

    /**
     * Support types 
     */

    // main purpose of this map is to convert each rule object into its appropriate type
    // ex: 
    /**
     *  { "rule": "scores",
         "score": "wins",
        "ascending": false
        }

        to 

        struct Scores{
        ....
        }
    * */
    std::unordered_map<std::string, std::function<void(const Object &obj)>> RuleStructure{
        {"foreach",         [](const Object &obj){  

        }},
        {"loop",            [](const Object &obj){  

        }},
        {"inparallel",      [](const Object &obj){  

        }},
        {"parallelfor",     [](const Object &obj){  

        }},
        {"switch",          [](const Object &obj){  

        }},
        {"when",            [](const Object &obj){  

        }},
        {"extend",          [](const Object &obj){  

        }},
        {"reverse",         [](const Object &obj){  

        }},
        {"shuffle",         [](const Object &obj){  

        }},
        {"sort",            [](const Object &obj){  

        }},
        {"deal",            [](const Object &obj){  

        }},
        {"discard",         [](const Object &obj){  

        }},
        {"timer",           [](const Object &obj){  

        }},
        {"input-choice",    [](const Object &obj){  

        }},
        {"input-text",      [](const Object &obj){  

        }},
        {"input-vote",      [](const Object &obj){  

        }},
        {"message",         [](const Object &obj){  

        }},
        {"global-message",  [](const Object &obj){  
        std::cout << "here" << std::endl; // works

        GlobalMessage({obj});
        }},
        {"scores",          [](const Object &obj){  
        std::cout << "here" << std::endl; // works
        
        }},
    };

    template <typename K, typename V> 
    struct GenType {
        std::unordered_map<K, V> map;
        K key;
        V value;
    };


    /////////////////////////////////////////////////////////////////////////////
    // Casting From Json to DSL
    /////////////////////////////////////////////////////////////////////////////
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

    // STILL IN DEVELOPMENT
    struct BuildRule {
        void build() {
            auto lambda = [](auto&& variant) {
            return std::visit(
                [](auto&& arg) -> Value {
                    using T = std::decay_t<decltype(arg)>;
                    return arg;
                },
            variant);
            };

            auto it = lambda(Value(2)); // example code
        }
    };

    /**
     * Rules
     */

    // Mainly to interpret the values within the rules
    struct Interpreter {
        void operator()(std::monostate) const {  }
        void operator()(const String &str) const { 
            std::cout << str << std::endl; 
            // All values like:
            // Winners: {winners.elements.name}
            // Will be proccessed here


        }
        void operator()(const Integer num) const { 
            std::cout << num << std::endl; 
            // All values like: 0 



        }
        void operator()(const Array &array) const {
            // Loop over the an array and extract the values to do processing by recursive visiting
            if (!array.values.empty()) {
                std::for_each(array.values.begin(), array.values.end(), [this](const auto &arr) {
                    std::visit(*this, arr);
                });
            }
        }
        void operator()(const Object &object) const {
            // Loop over the an object and extract the key value to do processing by recursive visiting
            if (!object.values.empty()) {

                // Get type of rule. "rule": "scores"
                if ( auto rule{ obj.values.find( "rule" ) }; rule != std::end( obj.values )) {
                    auto[ key, type ] { *rule };

                    // TODO: Remove these 3 statments
                    std::cout << key << std::endl; 
                    std::visit(*this, type);
                    std::cout << std::endl; 

                    // I need to figure out how i can pass by refrence this obj change it and return it
                    std::visit([obj](auto&& arg){
                        using T = std::decay_t<decltype(arg)>; // similar to typeid().name()
                            if constexpr (std::is_same_v<T, String>) {
                                RuleStructure[arg](obj);
                            }
                        }, Value(type)
                    );

                    // This will be abstract because it will be used heavily by the rule map
                    std::for_each(object.values.begin(), object.values.end(), [this](const auto &obj) {
                        std::visit(*this, (Value) obj.first); // this visitation now visits this interpreter but will visit based on the string
                        std::visit(*this, obj.second);
                    });
                }
            }
        }
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
                    // [](ControlStructures rule){
                    //     switch(rule){
                    //         case ControlStructures::ForEach:
                    //         case ControlStructures::Loop:
                    //         case ControlStructures::Inparallel:
                    //         case ControlStructures::Parallelfor:
                    //         case ControlStructures::Switch:
                    //         case ControlStructures::When:
                    //         default: break;
                    //     }
                    // },
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
                    // [](Timing rule){
                    //     rule->func();
                    // },
                    [](InputChoice rule){
                        
                    },
                    // [](HumanInput rule){
                    //     switch(rule){
                    //         case HumanInput::InputChoice:
                                
                    //     }
                    // },
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

    struct HumanInput{
        
    };

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
        Value constants;
    };

    struct Variables {
        Value variables;
    };

    struct PerPlayer {
        Value perPlayer;
    };

    struct PerAudience {
        Value perAudience;
    };

    struct Configuration {
        String name;
        PlayerCount playerCount; // an object type can be used here i guess
        bool audience;
        Value setup;
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
    } game;

    template <typename Type> 
    class EngineImpl { 
        public:
            EngineImpl (const T& input);
            GenType<String, Game> getGameConfig() const noexcept;
            Game initalizeEngine(const Type input);
            
            // Parser Related methods
            bool validGameConfig(const Type& input);
            Game buildGame();

        private:
            Type input;
            GenType<String, Game> gameConfig;

            // Domain level set functions, these should never throw if we do our validation correctly
            Boolean setConfiguration(const Type& configuration) const noexcept;
            Boolean setConstants(const Type& constants) const noexcept;
            Boolean setVariables(const Type& variables) const noexcept;
            Boolean setPerPlayer(const T& perPlayer) const noexcept;
            Boolean setPerAudience(const Type& perAudience) const noexcept;
            Boolean setRules(const Type& rules) const noexcept;

            Object mapConfig() const noexcept;

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
