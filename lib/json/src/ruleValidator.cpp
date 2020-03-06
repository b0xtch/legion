#include "fieldValidators.h"
#include "ruleStructure.h"
#include "jsonDSL.h"
#include <algorithm>
#include <exception>

using json = nlohmann::json;
using Rule = ruleValidationHelper::Rule;

static JsonDSL dsl;
static std::map<JsonDSL::RuleType, Rule> ruleMap = ruleValidationHelper::getRuleMap();

static void validateNecessaryParametersPresent(const json& ruleObject, const Rule& ruleDefinition){

}

static void validateAllParametersAreValid(const json& ruleObject, const Rule& ruleDefinition){

}

static void validateSingleRule(const json& ruleObject){

}

static void validateRulesStructure(const json& rulesObject){

}

VariableValidator RulesValidator::validateRules(const json& j_object){

}

RulesValidator::RulesValidator(){}

