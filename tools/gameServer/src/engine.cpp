#include<utility>
#include<map>
#include<string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "engine.h"
#include <json.hpp>
#include <any>
// #include "jsonvalidator.h"

// for convenience
using json = nlohmann::json;

template <typename T>
Engine<T>::Engine (T json) {
    if(this->validGameConfig(json)) this->initalizeEngine(); 

    std::cout << "Engine could not read game specification" << endl;
}

template <typename T> 
bool Engine<T>::validGameConfig(T json) { 
    // if(JsonValidator.validJson(json)) {
    //     this->json = json; 
    //     return true;
    // }

    this->json = json; 
    return true;
} 

template <typename T> 
void Engine<T>::initalizeEngine() { 
    std::cout << "Engine Initalizing!" << endl;

    for (auto& [key, value] : this->json.items()){
        this->mapKeyToValue(key, value); 
    }
}
  
template <typename T> 
void Engine<T>::buildGame() { 
    std::cout << "Building new game from the following configs..." << endl;
    std::cout << this->json << endl;

    // merge all the config variables together
    // This one of the last methods that will be called to construct
    // a game with all the game specification

}

template <typename T> 
void Engine<T>::mapKeyToValue(T key, T value){
    this->gameConfig.genMap[key] = this->mapValueToFuntion(value);
}

template <typename T> 
T Engine<T>::mapValueToFuntion(T value){
    return value.flatten();
}

template <typename T> 
GenType<T> Engine<T>::getGameConfig(){
    for (const auto&[key, value] : this->gameConfig.genMap) {
        cout << key << " " << std::any_cast<T>(value) << endl;
    }

    return this->gameConfig;
}

/////////////////////////////////////////////////////////////////////////////
// Control Structures
/////////////////////////////////////////////////////////////////////////////

template <typename T> 
void Engine<T>::findAndExecute() { }