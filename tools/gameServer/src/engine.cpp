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
    // Control Structures
    /////////////////////////////////////////////////////////////////////////////

    template <typename T> 
    void EngineImpl<T>::findAndExecute() { }
}
