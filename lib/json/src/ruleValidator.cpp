#include "fieldValidators.h"
#include "ruleStructure.h"
#include "jsonDSL.h"
#include <algorithm>
#include <exception>

using json = nlohmann::json;
using Rule = ruleValidationHelper::Rule;

static JsonDSL dsl;
static std::map<JsonDSL::RuleType, Rule> ruleMap = ruleValidationHelper::getRuleMap();

static void validateNecessaryParametersPresent(const json& ruleJson, const Rule& ruleDefinition){

}

static void validateAllParametersAreValid(const json& ruleJson, const Rule& ruleDefinition){

}

static void validateSingleRule(const json& ruleJson){
    std::string ruleString = dsl.getStringOfRuleParameterCommand(JsonDSL::Rule);
    
    if(!ruleJson.contains(ruleString)){
        throw std::invalid_argument("One of the rules are missing the rule field.");
    }

    std::string ruleName = ruleJson[ruleString];
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
    std::string rulesString = dsl.getStringOfSpecificationCommand(JsonDSL::Rules);
    json rules = j_object[rulesString];
    validateRulesStructure(rules);
    return VariableValidator();
}

RulesValidator::RulesValidator(){}

