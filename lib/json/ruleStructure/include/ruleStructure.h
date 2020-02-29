#include <string>
#include <vector>
#include <map>
#include "jsonDSL.h"

#ifndef RULE_STRUCTURE_H
#define RULE_STRUCTURE_H


namespace ruleStructure{
    class Rule;
    using StringToRuleMap = std::map<std::string, Rule>;
    StringToRuleMap getStringToRuleMap();
}

class ruleStructure::Rule{
public:
    Rule(std::string name, std::vector<JsonDSL::RuleParameters> parameters, bool hasCases, bool hasSetOfRules);
private:
    std::string name;
    std::vector<JsonDSL::RuleParameters> parameters;
    bool hasCases;
    bool hasSetOfRules;
};

#endif