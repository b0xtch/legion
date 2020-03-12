#include "fieldValidators.h"
#include "ruleStructure.h"
#include "jsonDSL.h"
#include <algorithm>
#include <exception>

using json = nlohmann::json;
using Rule = ruleValidationHelper::Rule;
using RuleMap = std::map<std::string, Rule>;

static JsonDSL dsl;

static void validateNecessaryParametersPresent(const json& ruleJson, Rule& ruleDefinition){
    auto paramItBegin = ruleDefinition.getParametersBegin();
    auto paramItEnd = ruleDefinition.getParametersEnd();

    auto findResult = std::find_if(paramItBegin, paramItEnd, 
        [&ruleJson](JsonDSL::RuleParameters dslEnum){
            std::string stringToCheck = dsl.getRuleParameterString(dslEnum);
            return !ruleJson.contains(stringToCheck);
        }
    );

    std::string ruleName = ruleJson[dsl.getRuleParameterString(JsonDSL::Rule)];

    if(findResult != paramItEnd){
        std::stringstream errorMsg;
        errorMsg << "Rule " << ruleName << " is missing parameter " << dsl.getRuleParameterString(*findResult) << ".";
        throw std::invalid_argument(errorMsg.str());
    }

    if(ruleDefinition.hasCases()){
        json cases = ruleJson[dsl.getRuleParameterString(JsonDSL::Cases)];
        
        std::string caseStr = dsl.getRuleParameterString(JsonDSL::Case);
        std::string conditionStr = dsl.getRuleParameterString(JsonDSL::Condition);

        std::string paramUsed = ruleDefinition.hasParameter(caseStr) ? caseStr : conditionStr;

        for(auto caseObj : cases){
            bool hasCaseField = caseObj.contains(paramUsed);
            bool hasNestedRules = caseObj.contains(dsl.getSpecString(JsonDSL::Rules));

            if(!hasCaseField || !hasNestedRules){
                std::stringstream errorMsg;
                errorMsg << ruleName << " has a cases field and each case much have a case/condition field and a rules field.";
                throw std::invalid_argument (errorMsg.str());
            }
        }

    }else if(ruleDefinition.hasSetOfRules){
        if(!ruleJson.contains(dsl.getSpecString(JsonDSL::Rules))){
            std::stringstream errorMsg;
            errorMsg << ruleName << " requires a rules field.";
            throw std::invalid_argument (errorMsg.str());
        }
    }
}

//call this function after validateNecessaryParametersPresent
//this function assumes that the cases parameter is defined 
//and that its contents contain valid data in the correct format
static void validateAllParametersAreValid(const json& ruleJson, const Rule& ruleDefinition){
    for(auto parameter : ruleJson.items()){
        if(!ruleDefinition.hasParameter(parameter.key())){
            throw std::invalid_argument("An illegal key was found inside one of the rules.");
        }
    }

    if(ruleDefinition.hasCases()){
        //A case is only valid if it holds 2 fields, if the two valid fields are present
        //then there must be an illegal key if the number of fields is greater than 2
        json cases = ruleJson[dsl.getRuleParameterString(JsonDSL::Cases)];
        int numberOfFieldsInsideCase = 2;
        for(auto caseObj : cases){
            if(caseObj.size() != numberOfFieldsInsideCase){
                throw std::invalid_argument("The case objects must contain only two fields, case/condition and rules.");
            }
        }
    }
}

//forward declaration of function for validateSingleRule to use
//necessary because both functions call each other in a recursive fashion
static void validateRulesStructure(const json& rulesJson, const RuleMap& ruleMap);

static void validateSingleRule(const json& ruleJson, const RuleMap& ruleMap){
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

    if(ruleDefiniton.hasCases()){
        json cases = ruleJson[dsl.getRuleParameterString(JsonDSL::Cases)];
        for(auto caseJson : cases){
            auto nestedRules = caseJson[dsl.getSpecString(JsonDSL::Rules)];
            validateRulesStructure(nestedRules, ruleMap);
        }
    } else if (ruleDefiniton.hasSetOfRules){
        json nestedRules = ruleJson[dsl.getSpecString(JsonDSL::Rules)];
        validateRulesStructure(nestedRules, ruleMap);
    }
}

static void validateRulesStructure(const json& rulesJson, const RuleMap& ruleMap){
    if(!rulesJson.is_array()){
        throw std::invalid_argument("The rules are expected to be stored in an array.");
    }

    for(auto rule : rulesJson){
        validateSingleRule(rule, ruleMap);
    }
}

VariableValidator RulesValidator::validateRules(const json& j_object){
    //cannot make global or global static or else this will segfault
    RuleMap ruleMap = ruleValidationHelper::getRuleMap();
    std::string rulesString = dsl.getSpecString(JsonDSL::Rules);
    json rules = j_object[rulesString];

    if(rules.size() != 0){
        validateRulesStructure(rules, ruleMap);
    }
    
    return VariableValidator();
}

RulesValidator::RulesValidator(){}

