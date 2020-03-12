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
    Rule(JsonDSL::RuleType name, std::vector<JsonDSL::RuleParameters> parameters, bool hasSetOfRules, JsonDSL::RuleParameters optional);
    int getParameterCount() const;
    bool hasParameter(const std::string&) const;

    using vecIterator = std::vector<JsonDSL::RuleParameters>::iterator;
    vecIterator getParametersBegin();
    vecIterator getParametersEnd();

    const JsonDSL::RuleType name;
    bool hasCases() const;
    const bool hasSetOfRules;
private:
    JsonDSL::RuleParameters optional;
    bool hasOptional;
    std::vector<JsonDSL::RuleParameters> parameters;

};

#endif