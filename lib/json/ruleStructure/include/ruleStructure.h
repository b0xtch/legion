#include <string>
#include <vector>
#include <map>
#include "jsonDSL.h"

#ifndef RULE_STRUCTURE_H
#define RULE_STRUCTURE_H


namespace ruleValidationHelper{
    struct Rule;
    using RuleMap = std::map<JsonDSL::RuleType, Rule>;
    RuleMap getRuleMap();
}

struct ruleValidationHelper::Rule{
public:
    Rule(JsonDSL::RuleType name, std::vector<JsonDSL::RuleParameters> parameters, bool hasCases, bool hasSetOfRules);
    int getParameterCount() const;
    bool hasParameter(JsonDSL::RuleParameters) const;

    using vecIterator = std::vector<JsonDSL::RuleParameters>::iterator;
    vecIterator getParametersBegin();
    vecIterator getParametersEnd();

    const JsonDSL::RuleType name;
    const bool hasCases;
    const bool hasSetOfRules;
private:
    std::vector<JsonDSL::RuleParameters> parameters;

};

#endif