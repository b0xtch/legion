#include<utility>
#include<map>
#include<string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "engine.h"
#include <json.hpp>
#include <any>
#include <tuple>
// #include "jsonvalidator.h"

using json = nlohmann::json;
using Engine::GenType;
using Engine::Value;

namespace Engine {
    
    template <typename T>
    EngineImpl<T>::EngineImpl (const T& input): 
    input(input) {}

    template <typename T> 
    GenType<std::string, Game> EngineImpl<T>::initalizeEngine() { 
        std::cout << "Engine Initalizing!" << endl;
        if(this->validGameConfig(input)){
            for (auto& [key, value] : this->input.items()){
                this->mapKeyToValue(key, value); 
            }

            // return buildGame();
        }; 
    }

    template <typename T> 
    bool EngineImpl<T>::validGameConfig(const T& input) { 
        // if(JsonValidator.validJson(input)) {
        //     this->input = input; 
        //     return true;
        // }

        this->input = std::move(input); 
        return true;
    } 
    
    template <typename T> 
    GenType<std::string, Game> EngineImpl<T>::buildGame() { 
        std::cout << "Building new game from the following configs..." << endl;
        std::cout << this->input << endl;

        // For testing TODO: Remove
        json data = readJsonFromFile("../../src/data.json");

        std::map<std::string, std::function<Value(const json& in)> > Game{
            {"configuration", [](const json& in){ return setConfiguration(in); }},
            {"constants", [](const json& in){ return setConstants(in); }},
            {"variables", [](const json& in){ return setVariables(in); }},
            {"per-player", [](const json& in){ return setPerPlayer(in); }},
            {"per-audience", [](const json& in){ return setPerAudience(in); }},
            {"rules", [](const json& in){ return setRules(in); }}
        };

        // Passed on the key in the shallow level of the config call the appropriate fucntion and pass only that object
        for (const auto &[key, value] : data.items()) {
            Game[key](value);
        }

        return this->gameConfig;
    }

    // Just for testing now TODO: Remove
    json readJsonFromFile(const std::string& file_path) {
        std::ifstream input(file_path);

        if(input.fail()) {
            throw std::runtime_error("Unable to open file " + file_path);
        }

        return json::parse(input);
    }

    template <typename T> 
    void EngineImpl<T>::mapKeyToValue(const T& key, const T& value){
        std::cout << key << " " << this->mapValueToFuntion(value) << std::endl;
    }

    template <typename T> 
    T EngineImpl<T>::mapValueToFuntion(const T& value){
        return value.flatten();
    }

    template <typename T> 
    GenType<std::string, Game> EngineImpl<T>::getGameConfig() const {
        return this->gameConfig;
    }

    /////////////////////////////////////////////////////////////////////////////
    // Main Parser from Type T to DSL
    /////////////////////////////////////////////////////////////////////////////
    Object mapConfig(const json& json, Object& map) {
        if (json.is_object() || json.is_array()) {
            for (auto &[key, value] : json.items()) {
                if(value.is_string()){
                    map[key] = (String) json;
                }else if(value.is_number()){
                    map[key] = (Integer) json;
                }else if(value.is_boolean()){
                    map[key] = (Boolean) json;
                }else if (value.is_object()) {
                    mapConfig(value, map);
                }else if(value.is_array()){
                    mapConfig(value, map);
                }else{}
            }
        }
    }
    
    template <typename T> 
    Configuration EngineImpl<T>::setConfiguration(const T& in) {
        Object setup;
        setup.values.emplace("setup", recursiveValueMap(in["setup"]));

        Configuration configuration{
            in["name"],
            {
            in["player count"]["min"], 
            in["player count"]["max"]
            },
            in["audience"],
            setup
        };

        // this needs to be moved to a place where it is called when necessary\
        // but it shows that now you can recurse and get the values.
        // might not even use it as these are actionless unlike the rules
        std::visit(Interpreter{}, (Value) configuration.setup); 

        return configuration;
    }

    template <typename T> 
    Constants EngineImpl<T>::setConstants(const T& in){
        Object vars;
        consts.values.emplace("constants", recursiveValueMap(in));

        Constants constants{ vars };

        return constants;
    }

    template <typename T> 
    Variables EngineImpl<T>::setVariables(const T& in){
        Object vars;
        vars.values.emplace("variables", recursiveValueMap(in));

        Variables variables{ vars };

        return variables;
    }

    template <typename T> 
    PerPlayer EngineImpl<T>::setPerPlayer(const T& in){
        Object vars;
        vars.values.emplace("per-player", recursiveValueMap(in));

        PerPlayer perPlayer{ vars };
        
        // not sure yet how this and per audience ties into the overall players

        return perPlayer;
    }

    template <typename T> 
    PerAudience EngineImpl<T>::setPerAudience(const T& in){
        Object vars;
        vars.values.emplace("per-audience", recursiveValueMap(in));

        PerAudience perAudience{ vars };

        return perAudience;
    }

    template <typename T> 
    Rules EngineImpl<T>::setRules(const T& in){
        Array vars;

        vars.values.emplace_back(recursiveValueMap(in));
        Rules rules { vars };

        std::visit(Interpreter{}, (Value) rules.rules);

        return rules;
    }

    /////////////////////////////////////////////////////////////////////////////
    // Control Structures
    /////////////////////////////////////////////////////////////////////////////

    template <typename T> 
    void EngineImpl<T>::findAndExecute() { }

    /*************************************
    *
    *     RuleCollection Implemention  
    *
    **************************************/

    template <typename T>
    void forEachImpl(RuleCollection::ForEach &data){
        for(T &el : data.list){
            for(RuleCollection::GenRule rule : data.rules_to_run){
                std::apply(rule, el);
            }
        }
    }
}
