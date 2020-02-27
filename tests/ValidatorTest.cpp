#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <iostream>
#include <iomanip>
#include <string>
#include "json.hpp"
#include "fieldValidators.h"
#include <exception>

using json = nlohmann::json;

enum validatorLevels {
    specification, config, rule, variable
};

class ValidatorTests : public ::testing::Test{
protected:
    std::string emptySuffix;
    void SetUp() override{
        emptySuffix = R"###(
            "constants": {},
            "variables": {},
            "per-player": {},
            "per-audience": {},
            "rules":{}}
        )###";
    }

    void runValidatorWithThrow(std::string& jsonText, const std::string& expectedMessage, validatorLevels level){
        json j_object = json::parse(jsonText);
        SpecificationValidator specValidator;
        try{
            ConfigValidator confValidator = specValidator.validateSpecification(j_object);
            if(level == specification){
                FAIL() << "Expected specification validator to fail";
            }
            confValidator.validateConfig(j_object);
        }catch(std::invalid_argument e){
            EXPECT_EQ(e.what(), expectedMessage);
        }catch(...){
            FAIL() << "Expected invalid_argument exception";
        }
    }

    void runValidatorWithoutThrow(std::string& jsonText, validatorLevels level){
        json j_object = json::parse(jsonText);
        SpecificationValidator specValidator;
        ASSERT_NO_THROW(specValidator.validateSpecification(j_object));
        if(level == specification){
            return;
        }
    }
};

TEST_F(ValidatorTests, detectMissingSpecification){
    std::string jsonText = R"###({
    )###";
    jsonText = jsonText + emptySuffix;
    std::string expectedMessage = "Top level specification Field: configuration not found.";
    runValidatorWithThrow(jsonText, expectedMessage, specification);
}

TEST_F(ValidatorTests, detectIllegalSpecification){
    std::string jsonText = R"###({
    "asdasdasdasdasd":{},
    "configuration": {
        "name": "Zen Game",
        "player count": {"min": 0, "max": 0},
        "audience": {},
        "setup": { }
    },
    )###";

    jsonText = jsonText + emptySuffix;
    std::string expectedMessage = "An illegal key was found on the top level specification level of the json file";
    runValidatorWithThrow(jsonText, expectedMessage, specification);
}

TEST_F(ValidatorTests, detectValidSpecification){
    std::string jsonText = R"###({
    "configuration": {},
    )###";

    jsonText = jsonText + emptySuffix;
    runValidatorWithoutThrow(jsonText, specification);
}

// void test3(){
//     std::string jsontext = R"###({
//     "configuration": {
//         "name": "Zen Game",
//         "player count": {"max": 0},
//         "audience": {},
//         "setup": { }
//     },
//     )###";
//     jsontext = jsontext + suffixExtra;
//     json j = json::parse(jsontext);
//     SpecificationValidator spec;
//     spec.validateSpecification(j).validateConfig(j);
// }

// void test4(){
//     std::string jsontext = R"###({
//     "configuration": {
//         "name": "Zen Game",
//         "player count": {"min": 10, "max": 0},
//         "audience": {},
//         "setup": { }
//     },
//     )###";
//     jsontext = jsontext + suffixExtra;
//     json j = json::parse(jsontext);
//     SpecificationValidator spec;
//     spec.validateSpecification(j).validateConfig(j);
// }

// void test5(){
//     std::string jsontext = R"###({
//     "configuration": {
//         "name": "Zen Game",
//         "player count": {"min": 10.2, "max": 0},
//         "audience": {},
//         "setup": { }
//     },
//     )###";
//     jsontext = jsontext + suffixExtra;
//     json j = json::parse(jsontext);
//     SpecificationValidator spec;
//     spec.validateSpecification(j).validateConfig(j);
// }

// void test6(){
//     std::string jsontext = R"###({
//     "configuration": {

//         "player count": {"min": 0, "max": 0}

//     },
//     )###";
//     jsontext = jsontext + suffixExtra;
//     json j = json::parse(jsontext);
//     SpecificationValidator spec;
//     spec.validateSpecification(j).validateConfig(j);
// }

// void test7(){
//     std::string jsontext = R"###({
//     "configuration": {
//         "name": "Zen Game",
//         "player count": {"min": 10.2, "max": 0},
//         "audience": {},
//         "setup": { },
//         "adasasdasdasdas": {}
//     },
//     )###";
//     jsontext = jsontext + suffixExtra;
//     json j = json::parse(jsontext);
//     SpecificationValidator spec;
//     spec.validateSpecification(j).validateConfig(j);
// }