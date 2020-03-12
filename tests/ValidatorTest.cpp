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

//TEST_F(ValidatorTests, ){}


enum validatorLevels {
    specification, config, rule, variable
};

class ValidatorTests : public ::testing::Test{
protected:
    std::string emptySuffix;
    std::string validConfig;
    std::string emptyRules;
    std::string emptyVariables;
    
    void SetUp() override{

        emptyVariables = R"###(
            "constants": {},
            "variables": {},
            "per-player": {},
            "per-audience": {},
        )###";

        emptyRules = R"###(
            "rules":{}}
        )###";

        emptySuffix = emptyVariables + emptyRules;

        validConfig = R"###({
            "configuration": {
                "name": "Zen Game",
                "player count": {"min": 0, "max": 0},
                "audience": {},
                "setup": { }
            },
        )###";
    }

    void runValidatorWithThrow(std::string& jsonText, const std::string& expectedMessage, validatorLevels level){
        json j_object = json::parse(jsonText);
        SpecificationValidator specValidator;
        try{

            ConfigValidator confValidator = specValidator.validateSpecification(j_object);
            if(level == specification){
                FAIL() << "Expected specification validator to fail.";
            }

            RulesValidator ruleValidator = confValidator.validateConfig(j_object);
            if(level == config){
                FAIL() << "Expected config validator to fail.";
            }

            ruleValidator.validateRules(j_object);

        }catch(std::invalid_argument e){
            EXPECT_EQ(e.what(), expectedMessage);
        }catch(...){
            FAIL() << "Expected invalid_argument exception.";
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

            RulesValidator ruleValidator = confValidator.validateConfig(j_object);
            if(level == config){
                return;
            }

            ruleValidator.validateRules(j_object);

        }catch(std::invalid_argument e){
            FAIL() << "Invalid Argument error thrown when it shouldn't be: " << e.what();
        }catch(...){
            FAIL() << "Unexpected error thrown.";
        }
    }
};

TEST_F(ValidatorTests, detectSimplestValidGameSpec){
    std::string jsonText = R"###(
        {
        "configuration": {
            "name": "Zen Game",
            "player count": {"min": 0, "max": 0},
            "audience": false,
            "setup": { }
        },
        "constants": {},
        "variables": {},
        "per-player": {},
        "per-audience": {},
        "rules": {}
        }
    )###";
    runValidatorWithoutThrow(jsonText, rule);
}

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
    std::string jsonText = validConfig + emptySuffix;
    runValidatorWithoutThrow(jsonText, config);
}

TEST_F(ValidatorTests, detectValidSingleRule){
    std::string jsonText = validConfig + emptyVariables;
    jsonText = jsonText + R"###(
    "rules": [
        { "rule": "add",
          "to": "randomVar",
          "value": 10
        }
    ]}
    )###";
    runValidatorWithoutThrow(jsonText, rule);
}

TEST_F(ValidatorTests, detectInvalidRuleName){
    std::string jsonText = validConfig + emptyVariables;
    jsonText = jsonText + R"###(
    "rules": [
        { "rule": "ad",
          "to": "randomVar",
          "value": 10
        }
    ]}
    )###";
    runValidatorWithThrow(jsonText, "ad is not a valid rule." ,rule);
}

TEST_F(ValidatorTests, detectMissingFieldInRule){
    std::string jsonText = validConfig + emptyVariables;
    jsonText = jsonText + R"###(
    "rules": [
        { "rule": "add",
          "value": 10
        }
    ]}
    )###";
    runValidatorWithThrow(jsonText, "Rule add is missing parameter to." ,rule);
}

TEST_F(ValidatorTests, detectValidNestedRules){
    std::string jsonTextStart = validConfig + emptyVariables;

    jsonTextStart = jsonTextStart + R"###(
    "rules": [
        { "rule": "parallelfor",
          "list": "list",
          "element": "element",
          "rules": [
    )###";

    std::string addRuleFinal = R"###(
    { "rule": "add",
        "to": "randomVar",
        "value": 10
    } )###";
        
    std::string addRule = addRuleFinal + ",\n";

    std::string jsonTextEnd = jsonTextStart + 
        addRule + addRule + addRule + addRuleFinal + "]}]}";

    runValidatorWithoutThrow(jsonTextEnd,rule);
}

TEST(RuleStructureTests, parameterMethodsWork){
    JsonDSL dsl;
    auto map = ruleValidationHelper::getRuleMap();
    auto addObj = (*map.find(dsl.getRuleString(JsonDSL::Add))).second;
    EXPECT_EQ(addObj.getParameterCount(), 2);
    EXPECT_TRUE(addObj.hasParameter(dsl.getRuleParameterString(JsonDSL::To)));
    EXPECT_TRUE(addObj.hasParameter(dsl.getRuleParameterString(JsonDSL::Value)));
    EXPECT_FALSE(addObj.hasParameter(dsl.getRuleParameterString(JsonDSL::RulePrompt)));

}