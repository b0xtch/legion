#include <iostream>
#include <algorithm>
#include <map>
#include "engine.h"
#include <json.hpp>
#include "jsonvalidator.h"

// for convenience
using json = nlohmann::json;

template <typename T>
Engine<T>::Engine (T json) {
    // just a placegholder json for testing
    json j1 = {
        {"configuration", {
            {"name", "Botch"},
            {"player count", {{"min", 0}, {"max", 0}}},
            {"audience", false},
            {"setup", { }}
        }},
        {"constants", {}},
        {"variables", {}},
        {"per-player", {}},
        {"per-audience", {}},
        {"rules", {}}
    };

    if(this->validGameConfig(json)){
        this->initalizeEngine();   
    } else{
        std::cout << "Engine could not read game specification" << endl;  
    }
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

    for (auto& [k, v] : this->json.items()){
        this->mapKeyToFunction(getCamelCase(k), v); 
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
std::string Engine<T>::getCamelCase(std::string &key){
    //example this_is => ThisIS
    // std::string stt{"this_is"}; 
    // optimize cause i dont know how to do this the correct way

    bool last = true;
    for (char& c : stt){
        c = last ? std::toupper(c) : std::tolower(c);
        last = !isalpha(c);
    }

    stt.erase( std::remove_if(stt.begin(), stt.end(),  
        [](char c) { 
            return !isalpha(c);
        }), stt.end()
    );

    return key;
}

template <typename T> 
void Engine<T>::mapKeyToFunction(std::string key, nlohmann::json value){
    // dynamically get the corresponding function that mataches the key 
    // and pass through the value


    /**
     * Try and use a lambda function like in python here
        [ capture list ] (parameters) -> return-type {   
            method definition
        } 
        You dont need define a return type explictly
     * 
    */
   //https://stackoverflow.com/questions/11284059/call-method-inside-lambda-expression
   //trying to implement something like link above

   this->setConfiguration(value); // will change once i figure out the code above


}

/////////////////////////////////////////////////////////////////////////////
// Think with the team how this can be merged into a generic type struct
/////////////////////////////////////////////////////////////////////////////

template <typename T> 
void Engine<T>::setConfiguration(nlohmann::json configuration){
    cout << "I am in: " << configuration << endl;
    this->configuration = configuration;
    // ill only be testing one json property this one for now as the plan is to 
    // make all the below as a generic type where we can merge these together

   std::map<Engine::Pair> setup;
}

template <typename T> 
void Engine<T>::setConstants(nlohmann::json constants){
    cout << "I am in: " << constants << endl;
    this-> constants = constants;
}

template <typename T> 
void Engine<T>::setVariables(nlohmann::json variables){
    cout << "I am in: " << variables << endl;
    this-> variables = variables;
}
template <typename T> 
void Engine<T>::setPerPlayer(nlohmann::json perPlayer){
    cout << "I am in: " << perPlayer << endl;
    this-> perPlayer = perPlayer;
}

template <typename T> 
void Engine<T>::setPerAudience(nlohmann::json perAudience){
    cout << "I am in: " << perAudience << endl;
    this-> perAudience = perAudience;
}

template <typename T> 
void Engine<T>::setRules(nlohmann::json rules){
    cout << "I am in: " << rules << endl;
    this-> rules = rules;
}

/////////////////////////////////////////////////////////////////////////////
// Functions in between
/////////////////////////////////////////////////////////////////////////////

template <typename T> 
void Engine<T>::mapKeyToValue(){
    // this function will call the specific Control Structures 
}

/////////////////////////////////////////////////////////////////////////////
// Control Structures
/////////////////////////////////////////////////////////////////////////////

template <typename T> 
void Engine<T>::findAndExecute() { }