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

bool Rule::hasParameter(const std::string& parameter) const {

    if(hasSetOfRules && parameter == dsl.getSpecString(JsonDSL::Rules)){
        return true;
    }

    auto it = std::find_if(parameters.begin(), parameters.end(), 
        [parameter](JsonDSL::RuleParameters dslEnum){
            return parameter == dsl.getRuleParameterString(dslEnum);
        }
    );
    return it != parameters.end();
}

bool Rule::hasCases() const{
    return hasParameter(dsl.getRuleParameterString(JsonDSL::Cases));
}

using vecIterator = std::vector<JsonDSL::RuleParameters>::iterator;
vecIterator Rule::getParametersBegin(){
    return parameters.begin();
}

vecIterator Rule::getParametersEnd(){
    return parameters.end();
}


RuleMap ruleValidationHelper::getRuleMap(){
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