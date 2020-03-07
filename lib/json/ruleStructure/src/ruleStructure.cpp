#include "ruleStructure.h"

using RuleMap = std::map<std::string, ruleValidationHelper::Rule>;

JsonDSL dsl;

using Rule = ruleValidationHelper::Rule;

Rule::Rule(JsonDSL::RuleType name, std::vector<JsonDSL::RuleParameters> parameters, bool hasSetOfRules)
    : name(name), parameters(parameters), hasSetOfRules(hasSetOfRules) {}

int Rule::getParameterCount() const {
    int nestedRules = hasSetOfRules ? 1 : 0;
    return parameters.size() + nestedRules;
}

bool Rule::hasParameter(JsonDSL::RuleParameters parameter) const {
    auto it = std::find(parameters.begin(), parameters.end(), parameter);
    return it != parameters.end();
}

bool Rule::hasCases() const{
    return hasParameter(JsonDSL::Cases);
}

using vecIterator = std::vector<JsonDSL::RuleParameters>::iterator;
vecIterator Rule::getParametersBegin(){
    return parameters.begin();
}

vecIterator Rule::getParametersEnd(){
    return parameters.end();
}


RuleMap ruleValidationHelper::getRuleMap(){
    bool containsCases = true;
    bool containsOtherRules = true;

    ruleValidationHelper::Rule Add = {
        JsonDSL::Add,
        {JsonDSL::To, JsonDSL::Value},
        !containsOtherRules
    };

    ruleValidationHelper::Rule Parallelfor = {
        JsonDSL::Parallelfor,
        {JsonDSL::List, JsonDSL::Element, JsonDSL::Cases},
        containsOtherRules
    };

    RuleMap map;
    map.insert(std::make_pair(dsl.getRuleString(Add.name), Add));
    map.insert(std::make_pair(dsl.getRuleString(Parallelfor.name), Parallelfor));
    return map;
}