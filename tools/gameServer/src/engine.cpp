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
- course grain subsytems
- boundries and constraints

four major comp:


// for convenience
using json = nlohmann::json;

template <typename T>
Engine<T>::Engine (T input) {
    if(this->validGameConfig(input)) this->initalizeEngine(); 

    std::cout << "Engine could not read game specification" << endl;
}

template <typename T> 
bool Engine<T>::validGameConfig(T input) { 
    // if(JsonValidator.validJson(input)) {
    //     this->input = input; 
    //     return true;
    // }

    this->input = input; 
    return true;
} 

template <typename T> 
void Engine<T>::initalizeEngine() { 
    std::cout << "Engine Initalizing!" << endl;

    for (auto& [key, value] : this->input.items()){
        this->mapKeyToValue(key, value); 
    }
}
  
template <typename T> 
void Engine<T>::buildGame() { 
    std::cout << "Building new game from the following configs..." << endl;
    std::cout << this->input << endl;

    // merge all the config variables together
    // This one of the last methods that will be called to construct
    // a game with all the game specification

}

template <typename T> 
void Engine<T>::mapKeyToValue(T key, T value){
    this->gameConfig.map[key] = this->mapValueToFuntion(value);
}

template <typename T> 
T Engine<T>::mapValueToFuntion(T value){
    return value.flatten();
}

template <typename T> 
GenType<T> Engine<T>::getGameConfig(){
    for (const auto&[key, value] : this->gameConfig.map) {
        cout << key << " " << std::any_cast<T>(value) << endl;
    }

    return this->gameConfig;
}

/////////////////////////////////////////////////////////////////////////////
// Control Structures
/////////////////////////////////////////////////////////////////////////////

template <typename T> 
void Engine<T>::findAndExecute() { }