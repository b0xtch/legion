#include<utility>
#include<map>
#include<string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "engine.h"
#include <json.hpp>
#include <any>
#include "jsonvalidator.h"

using json = nlohmann::json;
using Engine::EngineImpl;
using Engine::GenType;

namespace Engine {
    
    template <typename T>
    EngineImpl<T>::EngineImpl (T& input) {
        if(this->validGameConfig(input)) this->initalizeEngineImpl(); 

        std::cout << "EngineImpl could not read game specification" << endl;
    }

    template <typename T> 
    bool EngineImpl<T>::validGameConfig(T& input) { 
        // if(JsonValidator.validJson(input)) {
        //     this->input = input; 
        //     return true;
        // }

        this->input = input; 
        return true;
    } 

    template <typename T> 
    void EngineImpl<T>::initalizeEngine() { 
        std::cout << "EngineImpl Initalizing!" << endl;

        for (auto& [key, value] : this->input.items()){
            this->mapKeyToValue(key, value); 
        }
    }
    
    template <typename T> 
    void EngineImpl<T>::buildGame() { 
        std::cout << "Building new game from the following configs..." << endl;
        std::cout << this->input << endl;

        // merge all the config variables together
        // This one of the last methods that will be called to construct
        // a game with all the game specification

    }

    template <typename T> 
    void EngineImpl<T>::mapKeyToValue(T& key, T& value){
        this->gameConfig.map[key] = this->mapValueToFuntion(value);
    }

    template <typename T> 
    T EngineImpl<T>::mapValueToFuntion(T& value){
        return value.flatten();
    }

    template <typename T> 
    GenType<T> EngineImpl<T>::getGameConfig(){
        for (const auto&[key, value] : this->gameConfig.map) {
            cout << key << " " << std::any_cast<T>(value) << endl;
        }

        return this->gameConfig;
    }

    /////////////////////////////////////////////////////////////////////////////
    // Main Parser from Type T to DSL
    /////////////////////////////////////////////////////////////////////////////
    Configuration& EngineImpl<T>::setConfiguration(const nlohmann::json& configuration) {
        Configuration configuration = Configuration();

        this->gameConfig["configuration"] = configuration;
        return configuration;
    }

    cvpa& EngineImpl<T>::setConstants(const nlohmann::json& constants){
        cvpa constants;

        this->gameConfig["constants"] = constants;
        return constants;
    }

    CVPA& EngineImpl<T>::setVariables(const nlohmann::json& variables){
        CVPA variables;

        this->gameConfig["variables"] = variables;
        return variables;
    }

    CVPA& EngineImpl<T>::setPerPlayer(const nlohmann::json& perPlayer){
        CVPA perPlayer;

        this->gameConfig["perPlayer"] = perPlayer;
        return perPlayer;
    }

    CVPA& EngineImpl<T>::setPerAudience(const nlohmann::json& perAudience){
        CVPA perAudience;

        this->gameConfig["perAudience"] = perAudience;
        return perAudience;
    }

    Rules& EngineImpl<T>::setRules(const nlohmann::json& rules){
        Rules rules;

        this->gameConfig["rules"] = rules;
        return rules;
    }

    /////////////////////////////////////////////////////////////////////////////
    // Control Structures
    /////////////////////////////////////////////////////////////////////////////

    template <typename T> 
    void EngineImpl<T>::findAndExecute() { }
}
