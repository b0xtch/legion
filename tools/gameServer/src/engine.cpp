#include<utility>
#include<map>
#include<string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "engine.h"
#include <json.hpp>
// #include "jsonvalidator.h"

using json = nlohmann::json;
using Engine::GenType;

namespace Engine {
    
    template <typename T>
    EngineImpl<T>::EngineImpl (T& input): 
    input(input) {}

    template <typename T> 
    Game EngineImpl<T>::initalizeEngine() { 
        std::cout << "Engine Initalizing!" << endl;

        if(this->validGameConfig(input)){
            for (auto& [key, value] : this->input.items()){
                // this->mapKeyToValue(key, value); 
            }

            return buildGame();
        }; 
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
    Game EngineImpl<T>::buildGame() { 
        std::cout << "Building new game from the following configs..." << endl;
        std::cout << this->input << endl;

        // merge all the config variables together
        // This one of the last methods that will be called to construct
        // a game with all the game specification

        return this->gameConfig;
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
    Game EngineImpl<T>::getGameConfig() const {
        return this->gameConfig;
    }

    /////////////////////////////////////////////////////////////////////////////
    // Main Parser from Type T to DSL
    /////////////////////////////////////////////////////////////////////////////
    template <typename T> 
    Configuration& EngineImpl<T>::setConfiguration(const T& in) {
        Configuration configuration = Configuration();

        // this->gameConfig["configuration"] = configuration;
        return configuration;
    }

    template <typename T> 
    CVPA& EngineImpl<T>::setConstants(const T& in){
        CVPA constants;

        // this->gameConfig["constants"] = constants;
        return constants;
    }

    template <typename T> 
    CVPA& EngineImpl<T>::setVariables(const T& in){
        CVPA variables;

        // this->gameConfig["variables"] = variables;
        return variables;
    }

    template <typename T> 
    CVPA& EngineImpl<T>::setPerPlayer(const T& in){
        CVPA perPlayer;

        // this->gameConfig["perPlayer"] = perPlayer;
        return perPlayer;
    }

    template <typename T> 
    CVPA& EngineImpl<T>::setPerAudience(const T& in){
        CVPA perAudience;

        // this->gameConfig["perAudience"] = perAudience;
        return perAudience;
    }

    // template <typename T> 
    // Rules& EngineImpl<T>::setRules(const T& in){
    //     Rules rules;

    //     this->gameConfig["rules"] = rules;
    //     return rules;
    // }

    /////////////////////////////////////////////////////////////////////////////
    // Control Structures
    /////////////////////////////////////////////////////////////////////////////

    template <typename T> 
    void EngineImpl<T>::findAndExecute() { }
}
