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
using Engine::Boolean;
Using Engine::game;

namespace Engine {
    
    template <typename T>
    EngineImpl<Type>::EngineImpl (const Type& input): 
    input(input) {}

    template <typename Type>
    Game EngineImpl<Type>::initalizeEngine(const Type& input) { 
        std::cout << "Engine Initalizing!" << endl;
        if(this->validGameConfig(input)){
            return buildGame();
        }; 
    }

    template <typename Type>
    bool EngineImpl<Type>::validGameConfig(const Type& input) { 
        // if(JsonValidator.validJson(input)) {
        //     this->input = input; 
        //     return true;
        // }

        this->input = std::move(input); 
        return true;
    } 
    
    template <typename Type>
    Game EngineImpl<Type>::buildGame() { 
        std::cout << "Building new game from the following configs..." << endl;
        std::cout << this->input << endl;

        // For testing TODO: Remove
        json data = readJsonFromFile("../../src/data.json");

        std::map<std::string, std::function<Value(const json& in)> > Game{
            {"configuration", [](const Type& in){ return setConfiguration(in); }},
            {"constants", [](const Type& in){ return setConstants(in); }},
            {"variables", [](const Type& in){ return setVariables(in); }},
            {"per-player", [](const Type& in){ return setPerPlayer(in); }},
            {"per-audience", [](const Type& in){ return setPerAudience(in); }},
            {"rules", [](const Type& in){ return setRules(in); }}
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

    template <typename Type>
    GenType<std::string, Game> EngineImpl<Type>::getGameConfig() const {
        return this->gameConfig;
    }
    
    template <typename Type>
    Boolean EngineImpl<Type>::setConfiguration(const Type& in) const noexcept {
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

        game.configuration = { std::move(configuration) };
        return true;
    }

    template <typename Type>
    Boolean EngineImpl<Type>::setConstants(const Type& in) const noexcept {
        Object vars;
        vars.values.emplace("constants", recursiveValueMap(in));

        game.constants = { vars };
        return true;
    }

    template <typename Type>
    Boolean EngineImpl<Type>::setVariables(const Type& in) const noexcept {
        Object vars;
        vars.values.emplace("variables", recursiveValueMap(in));

        game.variables = { vars };
        return true;
    }

    template <typename Type>
    Boolean EngineImpl<Type>::setPerPlayer(const Type& in) const noexcept {
        Object vars;
        vars.values.emplace("per-player", recursiveValueMap(in));

        game.perPlayer = { vars };
        return true;
    }

    template <typename Type>
    Boolean EngineImpl<Type>::setPerAudience(const Type& in) const noexcept {
        Object vars;
        vars.values.emplace("per-audience", recursiveValueMap(in));

        game.perAudience = { vars };
        return true;
    }

    template <typename Type>
    Boolean EngineImpl<Type>::setRules(const Type& in) const noexcept {
        Array vars;
        vars.values.emplace_back(recursiveValueMap(in));

        game.rules = { vars };
        std::visit(Interpreter{}, (Value) game.rules.rules);
        return true;
    }

    /////////////////////////////////////////////////////////////////////////////
    // Control Structures
    /////////////////////////////////////////////////////////////////////////////

    template <typename Type>
    void EngineImpl<Type>::findAndExecute() { }

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
