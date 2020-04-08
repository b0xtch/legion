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
    using PlayerList    = GameImpl::Game::getAllPlayers();
    using AudienceList  = GameImpl::Game::getAllAudience();
    using json          = nlohmann::json;
    using String        = std::string;
    using Integer       = int;
    using Boolean       = bool;
    using Key           = std::string;
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
        {"add",          [](const Object &obj){  
            std::cout << "Creating Rule: add" << std::endl;
            auto rule = obj.values.find("rule")->second;
            auto to = obj.values.find("to")->second;
            auto value = obj.values.find("value")->second;

            // std::visit(BuildRule{}, rule);
            // std::cout << std::endl;
            // std::visit(BuildRule{}, to);
            // std::cout << std::endl;
            // std::visit(BuildRule{}, value);
            // std::cout << std::endl;

            int b = 1;
            RuleCollection::Arithmetic addition(b, 10, RuleCollection::MathOperation::ADD);
            components.entities.emplace_back(addition);
        }},
        {"foreach",         [](const Object &obj){  
            std::cout << "Creating Rule: foreach" << std::endl;
            // { "rule": "foreach",
            //   "list": <<list, list expression, or name of a list object>>,
            //   "element": << name for list element object within the rules below >>
            //   "rules": [
            //     << Rules to execute on every element of the given list >>
            //   ]
            // }
            auto rule = obj.values.find("rule")->second;
            auto list = obj.values.find("list")->second;
            auto element = obj.values.find("element")->second;
            Value rules = obj.values.find("rules")->second;
            // components.entities.emplace_back();
        }},
        {"loop",            [](const Object &obj){  
            // { "rule": "loop",
            //   "until": << Condition that may fail >>,
            //   OR
            //   "while": << Condition that may fail >>,

            //   "rules": [
            //     << Rules to execute on every element of the given list >>
            //   ]
            // }
            std::cout << "Creating Rule: loop" << std::endl;
            auto rule = obj.values.find("rule")->second;
            String untilOrWhile;
            auto type = Value(0);

            if ( auto rule{ obj.values.find( "until") }; rule != std::end( obj.values )) {
                untilOrWhile = "until";
                type = obj.values.find(untilOrWhile)->second;
            }else{
                untilOrWhile = "while";
                type = obj.values.find(untilOrWhile)->second;
            }
            Value rules = obj.values.find("rules")->second;

            std::vector<RuleCollection::GenRule*> r;

            int b = 1;
            int max1 = 100;
            RuleCollection::Condition<int> end_condition1 {b, max1, RuleCollection::ConditionType::GREATER};
            RuleCollection::Loop<int> loopobj(end_condition1, RuleCollection::LoopType::UNTIL, r);
            // components.entities.emplace_back();
        }},
        {"inparallel",      [](const Object &obj){  
            std::cout << "Creating Rule: inparallel" << std::endl;
            auto rule = obj.values.find("rule")->second;
        }},
        {"parallelfor",     [](const Object &obj){  
            std::cout << "Creating Rule: parallelfor" << std::endl;
            auto rule = obj.values.find("rule")->second;
            // components.entities.emplace_back();
        }},
        {"switch",          [](const Object &obj){  
            std::cout << "Creating Rule: switch" << std::endl;
            auto rule = obj.values.find("rule")->second;
            // components.entities.emplace_back();
        }},
        {"when",            [](const Object &obj){  
            // { "rule": "when",
            //   "cases": [
            //     { "condition": << Boolean guard for rules >>,
            //       "rules": [ << Rules to execute when the value and case match >> ]
            //     },
            //     ...
            //   ]
            // }
            std::cout << "Creating Rule: when" << std::endl;
            auto rule = obj.values.find("rule")->second;
            // components.entities.emplace_back();
        }},
        {"extend",          [](const Object &obj){  
            std::cout << "Creating Rule: extend" << std::endl;
            auto rule = obj.values.find("rule")->second;
            Array list {{5,60,-800,26,32,-46}};

            auto it = game.constants.constants.values.begin();
            const auto &[key, value] = *it;

            RuleCollection::Extend<Value> listOpExtend {100, list.values};
            components.entities.emplace_back(listOpExtend);
        }},
        {"reverse",         [](const Object &obj){  
            std::cout << "Creating Rule: reverse" << std::endl;
            auto rule = obj.values.find("rule")->second;
            Array list {{5,60,-800,26,32,-46}};

            RuleCollection::Reverse listOpReverse {list.values};
            components.entities.emplace_back(listOpReverse);
        }},
        {"shuffle",         [](const Object &obj){  
            std::cout << "Creating Rule: shuffle" << std::endl;
            auto rule = obj.values.find("rule")->second;
            Array list {{5,60,-800,26,32,-46}};

            RuleCollection::Shuffle listOpShuffle {list.values};
            components.entities.emplace_back(listOpShuffle);
        }},
        {"sort",            [](const Object &obj){  
            std::cout << "Creating Rule: sort" << std::endl;
            auto rule = obj.values.find("rule")->second;
            Array list {{5,60,-800,26,32,-46}};

            RuleCollection::Shuffle listOpSort {list.values};
            components.entities.emplace_back(listOpSort);
        }},
        {"deal",            [](const Object &obj){  
            std::cout << "Creating Rule: deal" << std::endl;
            auto rule = obj.values.find("rule")->second;
            // components.entities.emplace_back();
        }},
        {"discard",         [](const Object &obj){  
            std::cout << "Creating Rule: discard" << std::endl;
            auto rule = obj.values.find("rule")->second;
            // components.entities.emplace_back();
        }},
        {"timer",           [](const Object &obj){  
            std::cout << "Creating Rule: timer" << std::endl;
            auto rule = obj.values.find("rule")->second;
            // components.entities.emplace_back();
        }},
        {"input-choice",    [](const Object &obj){  
            std::cout << "Creating Rule: input-choice" << std::endl;
            auto rule = obj.values.find("rule")->second;
            // components.entities.emplace_back();
        }},
        {"input-text",      [](const Object &obj){  
            std::cout << "Creating Rule: input-text" << std::endl;
            auto rule = obj.values.find("rule")->second;
            // components.entities.emplace_back();
        }},
        {"input-vote",      [](const Object &obj){  
            std::cout << "Creating Rule: input-vote" << std::endl;
            auto rule = obj.values.find("rule")->second;
            // components.entities.emplace_back();
        }},
        {"message",         [](const Object &obj){  
            std::cout << "Creating Rule: message" << std::endl;
            auto rule = obj.values.find("rule")->second;
            // components.entities.emplace_back();
        }},
        {"global-message",  [](const Object &obj){  
            std::cout << "Creating Rule: global-message" << std::endl;
            auto rule = obj.values.find("rule")->second;


            // components.entities.emplace_back();
        }},
        {"scores",          [](const Object &obj){  
            std::cout << "Creating Rule: scores" << std::endl;
            auto rule = obj.values.find("rule")->second;
            auto score = obj.values.find("score")->second;
            auto ascending = obj.values.find("ascending")->second;

            // Temp Player list until we can programmatically get them @benny
            // Player p1{1, "Alex"};
            // Player p2{2, "Bob"};
            // Player p3{3, "Chuck"};
            // Player p4{4, "Dennis"};
            // Player p5{5, "Earl"};
            // RuleCollection::playerVector playerList = {p1,p2,p3,p4};

            // RuleCollection::Scores scores {playerList};
            // components.entities.emplace_back(scores);
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
            return (String) json;
        }else if(json.is_number()){
            return  (Integer) json;
        }else if(json.is_boolean()){
            return (Boolean) json;
        }else if (json.is_object()) {
            Object map;
            for(const auto&[key, value]: json.items()) {
                map.values.emplace(key, recursiveValueMap(value));
            }
            return map;
        }else if(json.is_array()){
            Array arr;
            for(const auto&[key, value]: json.items()) {
                arr.values.emplace_back(recursiveValueMap(value));
            }
            return arr;
        }
    }

    /**
     * Rules
     */

    // Mainly to interpret the values within the rules
    struct Interpreter {
        void operator()(std::monostate) const {  }
        void operator()(const String &str) const { 
            std::cout << str << std::endl; 
        }
        void operator()(const Integer num) const { 
            std::cout << num << std::endl; 
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
    struct Components {
        using Rules = Type<
        Integer,
        String,
        Boolean,
        rapper<Array>, 
        rapper<Object>,
        RuleCollection::Arithmetic,
        RuleCollection::When<Integer>,
        RuleCollection::Loop<Integer>,
        RuleCollection::Extend<Value>,
        RuleCollection::Reverse<Value>, 
        RuleCollection::Shuffle<Value>,
        RuleCollection::Sort<Integer>
        // RuleCollection::Scores
        >;

        void visit(){
            for (auto& entity : entities){
                std::visit(overloaded {
                    [](std::monostate){},
                    [](const Integer& rule){},
                    [](const String &str){},
                    [](const Boolean& rule){},
                    [](const Object& rule){},
                    [](const Array& rule){},
                    [](RuleCollection::Arithmetic rule){ rule.func(); },
                    [](RuleCollection::When<Integer> rule){ rule.func(); },
                    [](RuleCollection::Loop<Integer> rule){ rule.func(); },
                    [](RuleCollection::Extend<Value> rule){ rule.func(); },
                    [](RuleCollection::Reverse<Value> rule){ rule.func(); },
                    [](RuleCollection::Shuffle<Value> rule){ rule.func(); },
                    [](RuleCollection::Sort<Integer> rule){ rule.func(); },
                    // [this](RuleCollection::Scores rule){ rule.func(); },
                }, entity);
            }
        }
        
        template <typename V>
        void visit(V&& visitor){
            for (auto& entity : entities){
                std::visit(visitor, entity);
            }
        }

        std::vector<Rules> entities;
    } components;  

    struct BuildRule {
        void operator()(std::monostate) const { }
        void operator()(const String& str) const { 
            std::cout << str; 
        }
        void operator()(const Integer& num) const { 
            std::cout << num; 
        }
        void operator()(const Boolean& boo) const { 
            std::cout << boo; 
        }
        void operator()(const Array &arr) const {
            std::cout << "here" << std::endl;
            if (!arr.values.empty()) {
                std::for_each(arr.values.begin(), arr.values.end(), [this](const auto &arr) {
                    std::visit(*this, arr);
                });
            }
        }
        void operator()(const Object &obj) const {
            std::cout << "here" << std::endl;
            if (!obj.values.empty()) {
                std::for_each(obj.values.begin(), obj.values.end(), [this](const auto &obj) {
                    std::visit(*this, obj.second);
                });
            }
        }
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
