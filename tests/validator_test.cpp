#include<iostream>
#include <iomanip>
#include <string>
#include "json.hpp"
#include "fieldValidators.h"
#include <exception>
//TODO: convert tests to googletest suite
using json = nlohmann::json;

std::string suffixExtra = R"###(
    "constants": {},
    "variables": {},
    "per-player": {},
    "per-audience": {},
    "rules":{}}
)###";


void test1(){
    std::string jsontext = R"###({

)###";
    jsontext = jsontext + suffixExtra;
    json j = json::parse(jsontext);
    SpecificationValidator spec;
    spec.validateSpecification(j);
}

void test2(){
    std::string jsontext = R"###({
    "asdasdasdasdasd":{},
    "configuration": {
        "name": "Zen Game",
        "player count": {"min": 0, "max": 0},
        "audience": {},
        "setup": { }
    },
)###";
    jsontext = jsontext + suffixExtra;
    json j = json::parse(jsontext);
    SpecificationValidator spec;
    spec.validateSpecification(j);
}

void test3(){
    std::string jsontext = R"###({
    "configuration": {
        "name": "Zen Game",
        "player count": {"max": 0},
        "audience": {},
        "setup": { }
    },
    )###";
    jsontext = jsontext + suffixExtra;
    json j = json::parse(jsontext);
    SpecificationValidator spec;
    spec.validateSpecification(j).validateConfig(j);
}

void test4(){
    std::string jsontext = R"###({
    "configuration": {
        "name": "Zen Game",
        "player count": {"min": 10, "max": 0},
        "audience": {},
        "setup": { }
    },
    )###";
    jsontext = jsontext + suffixExtra;
    json j = json::parse(jsontext);
    SpecificationValidator spec;
    spec.validateSpecification(j).validateConfig(j);
}

void test5(){
    std::string jsontext = R"###({
    "configuration": {
        "name": "Zen Game",
        "player count": {"min": 10.2, "max": 0},
        "audience": {},
        "setup": { }
    },
    )###";
    jsontext = jsontext + suffixExtra;
    json j = json::parse(jsontext);
    SpecificationValidator spec;
    spec.validateSpecification(j).validateConfig(j);
}

void test6(){
    std::string jsontext = R"###({
    "configuration": {

        "player count": {"min": 0, "max": 0}

    },
    )###";
    jsontext = jsontext + suffixExtra;
    json j = json::parse(jsontext);
    SpecificationValidator spec;
    spec.validateSpecification(j).validateConfig(j);
}

void test7(){
    std::string jsontext = R"###({
    "configuration": {
        "name": "Zen Game",
        "player count": {"min": 10.2, "max": 0},
        "audience": {},
        "setup": { },
        "adasasdasdasdas": {}
    },
    )###";
    jsontext = jsontext + suffixExtra;
    json j = json::parse(jsontext);
    SpecificationValidator spec;
    spec.validateSpecification(j).validateConfig(j);
}

int main(){
    std::vector<std::string> testStrings;
    
    try{
        test1();
    }catch(std::invalid_argument e){
        std::cout << e.what() << "\n";
    }
    try{
        test2();
    }catch(std::invalid_argument e){
        std::cout << e.what() << "\n";
    }
    try{
        test3();
    }catch(std::invalid_argument e){
        std::cout << e.what() << "\n";
    }
    try{
        test4();
    }catch(std::invalid_argument e){
        std::cout << e.what() << "\n";
    }
    try{
        test5();
    }catch(std::invalid_argument e){
        std::cout << e.what() << "\n";
    }
    try{
        test6();
    }catch(std::invalid_argument e){
        std::cout << e.what() << "\n";
    }
    try{
        test7();
    }catch(std::invalid_argument e){
        std::cout << e.what() << "\n";
    }

    std::string jsontext = R"###({
    "configuration": {
        "name": "Zen Game",
        "player count": {"min": 10.2, "max": 0},
        "audience": {},
        "setup": { }
    },
    )###";
}

