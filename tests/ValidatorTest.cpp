#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <iostream>
#include <iomanip>
#include <string>
#include "json.hpp"
#include "fieldValidators.h"
#include "ruleStructure.h"
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
        try{
            
            ConfigValidator confValidator = specValidator.validateSpecification(j_object);
            if(level == specification){
                return;
            }
            confValidator.validateConfig(j_object);

        }catch(std::invalid_argument e){
            FAIL() << "Invalid Argument error thrown when it shouldn't be";
        }catch(...){
            FAIL() << "Unexpected error thrown.";
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

TEST_F(ValidatorTests, detectMissingConfigFields){
    std::string jsonText = R"###({
    "configuration": {
        "player count": {"min": 0, "max": 0}
    },
    )###";
    jsonText = jsonText + emptySuffix;
    std::string expectedMessage = "Config Field: audience not found.";
    runValidatorWithThrow(jsonText, expectedMessage, config);
}

TEST_F(ValidatorTests, detectIllegalConfig){
    std::string jsonText = R"###({
    "configuration": {
        "name": "Zen Game",
        "player count": {"min": 10.2, "max": 0},
        "audience": {},
        "setup": { },
        "adasasdasdasdas": {}
    },
    )###";
    jsonText = jsonText + emptySuffix;
    std::string expectedMessage = "An illegal key was found in the configuration level of the json file";
    runValidatorWithThrow(jsonText, expectedMessage, config);
}

TEST_F(ValidatorTests, detectMissingPlayerConfigField){
    std::string jsonText = R"###({
    "configuration": {
        "name": "Zen Game",
        "player count": {"max": 0},
        "audience": {},
        "setup": { }
    },
    )###";

    jsonText = jsonText + emptySuffix;
    std::string expectedMessage = "Min player count not found";
    runValidatorWithThrow(jsonText, expectedMessage, config);
}

TEST_F(ValidatorTests, detectMinPlayersGreaterThanMaxPlayers){
    std::string jsonText = R"###({
    "configuration": {
        "name": "Zen Game",
        "player count": {"min": 10, "max": 0},
        "audience": {},
        "setup": { }
    },
    )###";
    jsonText = jsonText + emptySuffix;
    std::string expectedMessage = "The minimum number of players must be less than or equal to the number of max players";
    runValidatorWithThrow(jsonText, expectedMessage, config);
}

TEST_F(ValidatorTests, detectInvalidFloatPlayerValues){
    std::string jsonText = R"###({
    "configuration": {
        "name": "Zen Game",
        "player count": {"min": 10.2, "max": 0},
        "audience": {},
        "setup": { }
    },
    )###";
    jsonText = jsonText + emptySuffix;
    std::string expectedMessage = "The values inside the players count object are not both integers";
    runValidatorWithThrow(jsonText, expectedMessage, config);
}

TEST_F(ValidatorTests, detectInvalidStringPlayerValues){
    std::string jsonText = R"###({
    "configuration": {
        "name": "Zen Game",
        "player count": {"min": "str", "max": 0},
        "audience": {},
        "setup": { }
    },
    )###";
    jsonText = jsonText + emptySuffix;
    std::string expectedMessage = "The values inside the players count object are not both integers";
    runValidatorWithThrow(jsonText, expectedMessage, config);
}

TEST_F(ValidatorTests, detectValidConfig){
    std::string jsonText = R"###({
    "configuration": {
        "name": "Zen Game",
        "player count": {"min": 0, "max": 0},
        "audience": {},
        "setup": { }
    },
    )###";
    jsonText = jsonText + emptySuffix;
    runValidatorWithoutThrow(jsonText, config);
}

TEST(RuleStructureTests, parameterMethodsWork){
    JsonDSL dsl;
    auto map = ruleValidationHelper::getRuleMap();
    auto addObj = (*map.find(dsl.getRuleString(JsonDSL::Add))).second;
    EXPECT_EQ(addObj.getParameterCount(), 2);
    EXPECT_TRUE(addObj.hasParameter(JsonDSL::To));
    EXPECT_TRUE(addObj.hasParameter(JsonDSL::Value));
    EXPECT_FALSE(addObj.hasParameter(JsonDSL::RulePrompt));

}