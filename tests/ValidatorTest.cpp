#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "json.hpp"
#include "fieldValidators.h"
#include "ruleStructure.h"
#include <exception>

using json = nlohmann::json;

//TEST_F(ValidatorTests, ){}

//RUN TESTS WITH THE BASE social-gaming FOLDER AS THE WORKING DIRECTORY OR TESTS WILL FAIL
enum validatorLevels {
    specification, config, rule, variable
};

class ValidatorTests : public ::testing::Test{
protected:

    std::string directoryPrefix;
    void SetUp() override{
        directoryPrefix = "tests/testfiles/validatorJsons/";
    }
    void runValidatorWithThrow(std::string& fileLocation, const std::string& expectedMessage, validatorLevels level){
        std::ifstream jsonFile (fileLocation);
        json j_object = json::parse(jsonFile);
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
            if(level == rule){
                FAIL() << "Expected config validator to fail.";
            }

        }catch(std::invalid_argument e){
            EXPECT_EQ(e.what(), expectedMessage);
        }catch(...){
            FAIL() << "Expected invalid_argument exception.";
        }
    }

    void runValidatorWithoutThrow(std::string& fileLocation, validatorLevels level){
        std::ifstream jsonFile (fileLocation);
        json j_object = json::parse(jsonFile);
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
    std::string fileLocation = directoryPrefix + "simplestGameSpec.json";
    runValidatorWithoutThrow(fileLocation, rule);
}

TEST_F(ValidatorTests, detectMissingSpecification){
    std::string fileLocation = directoryPrefix + "specWithMissingField.json";
    std::string expectedMessage = "Top level specification Field: configuration not found.";
    runValidatorWithThrow(fileLocation, expectedMessage, specification);
}

TEST_F(ValidatorTests, detectIllegalSpecification){
    std::string fileLocation = directoryPrefix + "specWithIllegalKey.json";
    std::string expectedMessage = "An illegal key was found on the top level specification level of the json file";
    runValidatorWithThrow(fileLocation, expectedMessage, specification);
}

TEST_F(ValidatorTests, detectValidSpecification){
    std::string fileLocation = directoryPrefix + "validSpec.json";
    runValidatorWithoutThrow(fileLocation, specification);
}

TEST_F(ValidatorTests, detectMissingConfigFields){
    std::string fileLocation = directoryPrefix + "configWithMissingField.json";
    std::string expectedMessage = "Config Field: audience not found.";
    runValidatorWithThrow(fileLocation, expectedMessage, config);
}

TEST_F(ValidatorTests, detectIllegalConfig){
    std::string fileLocation = directoryPrefix + "configWithIllegalKey.json";
    std::string expectedMessage = "An illegal key was found in the configuration level of the json file";
    runValidatorWithThrow(fileLocation, expectedMessage, config);
}

TEST_F(ValidatorTests, detectMissingPlayerConfigField){
    std::string fileLocation = directoryPrefix + "configWithPlayerFieldMissing.json";
    std::string expectedMessage = "Min player count not found";
    runValidatorWithThrow(fileLocation, expectedMessage, config);
}

TEST_F(ValidatorTests, detectMinPlayersGreaterThanMaxPlayers){
    std::string fileLocation = directoryPrefix + "configWithMinGTMax.json";
    std::string expectedMessage = "The minimum number of players must be less than or equal to the number of max players";
    runValidatorWithThrow(fileLocation, expectedMessage, config);
}

TEST_F(ValidatorTests, detectInvalidFloatPlayerValues){
    std::string fileLocation = directoryPrefix + "configWithFloatPlayerCount.json";
    std::string expectedMessage = "The values inside the players count object are not both integers";
    runValidatorWithThrow(fileLocation, expectedMessage, config);
}

TEST_F(ValidatorTests, detectNegativePlayerCounts){
    std::string fileLocation = directoryPrefix + "configWithNegativePlayers.json";
    std::string expectedMessage = "Player count values are not both greater than or equal to 0";
    runValidatorWithThrow(fileLocation, expectedMessage, config);
}

TEST_F(ValidatorTests, detectInvalidStringPlayerValues){
    std::string fileLocation = directoryPrefix + "configWithNonNumericPlayerCount.json";
    std::string expectedMessage = "The values inside the players count object are not both integers";
    runValidatorWithThrow(fileLocation, expectedMessage, config);
}

TEST_F(ValidatorTests, detectValidConfig){
    std::string fileLocation = directoryPrefix + "configValid.json";
    runValidatorWithoutThrow(fileLocation, config);
}

TEST_F(ValidatorTests, detectValidSingleRule){
    std::string fileLocation = directoryPrefix + "ruleSingleValid.json";
    runValidatorWithoutThrow(fileLocation, rule);
}

TEST_F(ValidatorTests, detectInvalidRuleName){
    std::string fileLocation = directoryPrefix + "ruleInvalidName.json";
    runValidatorWithThrow(fileLocation, "ad is not a valid rule." ,rule);
}

TEST_F(ValidatorTests, detectMissingParameterInRule){
    std::string fileLocation = directoryPrefix + "ruleMissingParameter.json";
    runValidatorWithThrow(fileLocation, "Rule add is missing parameter to." ,rule);
}

TEST_F(ValidatorTests, detectValidNestedRules){
    std::string fileLocation = directoryPrefix + "ruleNestedValid.json";
    runValidatorWithoutThrow(fileLocation,rule);
}

TEST_F(ValidatorTests, detectMissingRuleParameter){
    std::string fileLocation = directoryPrefix + "ruleWithoutRule.json";
    std::string expectedMessage = "One of the rules are missing the rule field.";
    runValidatorWithThrow(fileLocation, expectedMessage, rule);
}

TEST_F(ValidatorTests, detectWhenRulesNotInArray){
    std::string fileLocation = directoryPrefix + "ruleStoredWithinObject.json";
    std::string expectedMessage = "The rules are expected to be stored in an array.";
    runValidatorWithThrow(fileLocation, expectedMessage, rule);
}

TEST_F(ValidatorTests, detectMissingNestedRules){
    std::string fileLocation = directoryPrefix + "ruleMissingNestedRules.json";
    std::string expectedMessage = "foreach requires a rules field.";
    runValidatorWithThrow(fileLocation, expectedMessage, rule);

}

TEST_F(ValidatorTests, detectValidRockPaperScissors){
    std::string fileLocation = directoryPrefix + "rockPaperScissors.json";
    runValidatorWithoutThrow(fileLocation,rule);
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