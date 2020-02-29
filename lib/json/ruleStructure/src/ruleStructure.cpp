#include "ruleStructure.h"

using StringToRuleMap = std::map<std::string, ruleStructure::Rule>;

ruleStructure::Rule::Rule(std::string name, std::vector<JsonDSL::RuleParameters> parameter, bool hasCases, bool hasSetOfRules)
    : name(name), parameters(parameters), hasCases(hasCases), hasSetOfRules(hasSetOfRules) {}

StringToRuleMap ruleStructure::getStringToRuleMap(){}