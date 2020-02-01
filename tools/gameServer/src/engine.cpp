#include <iostream>
#include "engine.h"
#include <json.hpp>
#include "jsonvalidator.h"

// for convenience
using json = nlohmann::json;

template <typename T>
Engine<T>::Engine (T json) {
    if(this->validGameConfig(json)) this->initalizeEngine(); 

    cout << "Engine could not read game specification" < endl;
}

template <typename T> 
bool Engine<T>::validGameConfig(T json) { 
    if(JsonValidator.validJson(json)) {
        this->json = json; 
        return true;
    }
    return false;
} 

template <typename T> 
void Engine<T>::initalizeEngine() { 
    std::cout << "Engine Initalizing!" << endl;

    /**
     * Try and use a lambda function like in python here
        [ capture list ] (parameters) -> return-type {   
            method definition
        } 
        You dont need define a return type explictly
     * 
    */
    for (auto& [key, val] : this->json.items()){
        std::cout << "key: " << key << ", value:" << val << '\n';
    }

}
  
template <typename T> 
void Engine<T>::buildGame() { 
    std::cout << "Building new game from the following configs..." << endl;
    std::cout << this->json << endl;

    // merge all the config variables together

}

template <typename T> 
void Engine<T>::mapKeyToValue(){

}

template <typename T> 
void Engine<T>::setConfiguration(nlohmann::json configuration){

}

template <typename T> 
void Engine<T>::setConstants(nlohmann::json constants){

}

template <typename T> 
void Engine<T>::setVariables(nlohmann::json variables){

}
template <typename T> 
void Engine<T>::setPerPlayer(nlohmann::json perPlayer){

}

template <typename T> 
void Engine<T>::setPerAudience(nlohmann::json perAudience){

}

template <typename T> 
void Engine<T>::setRules(nlohmann::json rules){

}

/////////////////////////////////////////////////////////////////////////////
// Control Structures
/////////////////////////////////////////////////////////////////////////////

template <typename T> 
void Engine<T>::findAndExecute() { }