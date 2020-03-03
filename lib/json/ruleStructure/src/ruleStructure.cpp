#include "ruleStructure.h"

using RuleMap = std::map<JsonDSL::RuleType, ruleValidationHelper::Rule>;

JsonDSL dsl;

using Rule = ruleValidationHelper::Rule;

Rule::Rule(JsonDSL::RuleType name, std::vector<JsonDSL::RuleParameters> parameters, bool hasCases, bool hasSetOfRules)
    : name(name), parameters(parameters), hasCases(hasCases), hasSetOfRules(hasSetOfRules) {}

int Rule::getParameterCount() const {
    int caseOrRules = hasCases || hasSetOfRules ? 1 : 0;
    return parameters.size() + caseOrRules;
}

bool Rule::hasParameter(JsonDSL::RuleParameters parameter) const {
    auto it = std::find(parameters.begin(), parameters.end(), parameter);
    return it != parameters.end();
}

RuleMap ruleValidationHelper::getRuleMap(){
    bool containsCases = true;
    bool containsOtherRules = true;

    ruleValidationHelper::Rule Add = {
        JsonDSL::Add,
        {JsonDSL::To, JsonDSL::Value},
        !containsCases,
        !containsOtherRules
    };

    ruleValidationHelper::Rule Parallelfor = {
        JsonDSL::Parallelfor,
        {JsonDSL::List, JsonDSL::Element},
        !containsCases,
        containsOtherRules
    };

    RuleMap map;
    map.insert(std::make_pair(Add.name, Add));
    map.insert(std::make_pair(Parallelfor.name, Parallelfor));
    return map;
}