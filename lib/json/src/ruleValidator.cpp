#include "fieldValidators.h"
#include "ruleStructure.h"
#include "jsonDSL.h"
#include <algorithm>
#include <exception>

using json = nlohmann::json;
using Rule = ruleValidationHelper::Rule;

static JsonDSL dsl;
static std::map<std::string, Rule> ruleMap = ruleValidationHelper::getRuleMap();

static void validateNecessaryParametersPresent(const json& ruleJson, const Rule& ruleDefinition){

}

static void validateAllParametersAreValid(const json& ruleJson, const Rule& ruleDefinition){

}

//function declaration for validateSingleRule to use as both functions
//call each other in a recursive fashion
static void validateRulesStructure(const json& rulesJson);

static void validateSingleRule(const json& ruleJson){
    //check that a rule key is present in the json and has a valid value
    std::string ruleString = dsl.getRuleParameterString(JsonDSL::Rule);
    
    if(!ruleJson.contains(ruleString)){
        throw std::invalid_argument("One of the rules are missing the rule field.");
    }

    std::string ruleName = ruleJson[ruleString];
    auto ruleIterator = ruleMap.find(ruleName);

    if(ruleIterator == ruleMap.end()){
        throw std::invalid_argument(ruleName.append(" is not a valid rule."));
    }

    //get the rule object from the iterator and then use the other validate methods
    //to ensure the other data in the rule is valid
    auto ruleDefiniton = (*ruleIterator).second;
    validateNecessaryParametersPresent(ruleJson, ruleDefiniton);
    validateAllParametersAreValid(ruleJson, ruleDefiniton);

    if(ruleDefiniton.hasCases){
        json cases = ruleJson[dsl.getRuleParameterString(JsonDSL::Cases)];
        for(auto caseJson : cases){
            auto nestedRules = caseJson[dsl.getSpecString(JsonDSL::Rules)];
            validateRulesStructure(nestedRules);
        }
    } else if (ruleDefiniton.hasSetOfRules){
        json nestedRules = ruleJson[dsl.getSpecString(JsonDSL::Rules)];
        validateRulesStructure(nestedRules);
    }
}

static void validateRulesStructure(const json& rulesJson){
    if(!rulesJson.is_array()){
        throw std::invalid_argument("The rules are expected to be stored in an array.");
    }

    for(auto rule : rulesJson){
        validateSingleRule(rule);
    }
}

VariableValidator RulesValidator::validateRules(const json& j_object){
    std::string rulesString = dsl.getSpecString(JsonDSL::Rules);
    json rules = j_object[rulesString];
    validateRulesStructure(rules);
    return VariableValidator();
}

RulesValidator::RulesValidator(){}

