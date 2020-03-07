#include <string>
#include <vector>
#include <map>
#include "jsonDSL.h"

#ifndef RULE_STRUCTURE_H
#define RULE_STRUCTURE_H


namespace ruleValidationHelper{
    struct Rule;
    using RuleMap = std::map<std::string, Rule>;
    RuleMap getRuleMap();
}

struct ruleValidationHelper::Rule{
public:
    Rule(JsonDSL::RuleType name, std::vector<JsonDSL::RuleParameters> parameters, bool hasSetOfRules);
    int getParameterCount() const;
    bool hasParameter(JsonDSL::RuleParameters) const;

    using vecIterator = std::vector<JsonDSL::RuleParameters>::iterator;
    vecIterator getParametersBegin();
    vecIterator getParametersEnd();

    const JsonDSL::RuleType name;
    bool hasCases() const;
    const bool hasSetOfRules;
private:
    std::vector<JsonDSL::RuleParameters> parameters;

};

#endif