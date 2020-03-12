#include "ruleStructure.h"

using RuleMap = std::map<std::string, ruleValidationHelper::Rule>;

JsonDSL dsl;

using Rule = ruleValidationHelper::Rule;

Rule::Rule(JsonDSL::RuleType name, std::vector<JsonDSL::RuleParameters> parameters, bool hasSetOfRules)
    : name(name), parameters(parameters), hasSetOfRules(hasSetOfRules), hasOptional(false) {}

Rule::Rule(JsonDSL::RuleType name, std::vector<JsonDSL::RuleParameters> parameters, 
    bool hasSetOfRules, JsonDSL::RuleParameters optional)
    : name(name), parameters(parameters), hasSetOfRules(hasSetOfRules), 
    optional(optional), hasOptional(true) {}

int Rule::getParameterCount() const {
    int nestedRules = hasSetOfRules ? 1 : 0;
    return parameters.size() + nestedRules;
}

bool Rule::hasParameter(const std::string& parameter) const {

    bool isOptionalParameter = hasOptional && parameter == dsl.getRuleParameterString(optional);
    bool isNestedRules = hasSetOfRules && parameter == dsl.getSpecString(JsonDSL::Rules);
    bool isRuleParameter = parameter == dsl.getRuleParameterString(JsonDSL::Rule);

    if(isOptionalParameter||isNestedRules||isRuleParameter){
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

    ruleValidationHelper::Rule Foreach = {
        JsonDSL::Foreach,
        {JsonDSL::List, JsonDSL::Element},
        containsOtherRules
    };

    ruleValidationHelper::Rule Loop = {
        JsonDSL::Loop,
        {JsonDSL::Until, JsonDSL::While},
        containsOtherRules
    };

    ruleValidationHelper::Rule Inparallel = {
        JsonDSL::InParallel,
        {},
        containsOtherRules
    };

    ruleValidationHelper::Rule Parallelfor = {
        JsonDSL::Parallelfor,
        {JsonDSL::List, JsonDSL::Element},
        containsOtherRules
    };

    ruleValidationHelper::Rule Switch = {
        JsonDSL::Switch,
        {JsonDSL::Value, JsonDSL::List, JsonDSL::Cases},
        containsOtherRules
    };

    ruleValidationHelper::Rule When = {
        JsonDSL::When,
        {JsonDSL::Cases},
        containsOtherRules
    };

    ruleValidationHelper::Rule Extend = {
        JsonDSL::Extend,
        {JsonDSL::Target, JsonDSL::List},
        !containsOtherRules
    };

    ruleValidationHelper::Rule Reverse = {
        JsonDSL::Reverse,
        {JsonDSL::List},
        !containsOtherRules
    };

    ruleValidationHelper::Rule Shuffle = {
        JsonDSL::Shuffle,
        {JsonDSL::List},
        !containsOtherRules
    };

    ruleValidationHelper::Rule Sort = {
        JsonDSL::Sort,
        {JsonDSL::List},
        !containsOtherRules,
        JsonDSL::Key
    };

    ruleValidationHelper::Rule Deal = {
        JsonDSL::Deal,
        {JsonDSL::From, JsonDSL::To, JsonDSL::Count},
        !containsOtherRules
    };

    ruleValidationHelper::Rule Discard = {
        JsonDSL::Discard,
        {JsonDSL::From, JsonDSL::Count},
        !containsOtherRules
    };

    ruleValidationHelper::Rule Add = {
        JsonDSL::Add,
        {JsonDSL::To, JsonDSL::Value},
        !containsOtherRules
    };

    ruleValidationHelper::Rule Timer = {
        JsonDSL::Timer,
        {JsonDSL::Duration, JsonDSL::Mode},
        containsOtherRules,
        JsonDSL::Flag
    };

    ruleValidationHelper::Rule InputChoice = {
        JsonDSL::InputChoice,
        {JsonDSL::To, JsonDSL::RulePrompt, JsonDSL::Choices, JsonDSL::Result},
        !containsOtherRules,
        JsonDSL::Timeout
    };

    ruleValidationHelper::Rule InputText = {
        JsonDSL::InputText,
        {JsonDSL::To, JsonDSL::RulePrompt, JsonDSL::Result},
        !containsOtherRules,
        JsonDSL::Timeout
    };

    ruleValidationHelper::Rule InputVote = {
        JsonDSL::InputVote,
        {JsonDSL::To, JsonDSL::RulePrompt, JsonDSL::Choices, JsonDSL::Result},
        !containsOtherRules,
        JsonDSL::Timeout
    };

    ruleValidationHelper::Rule Message = {
        JsonDSL::Message,
        {JsonDSL::To, JsonDSL::Value},
        !containsOtherRules
    };

    ruleValidationHelper::Rule GlobalMessage = {
        JsonDSL::GlobalMessage,
        {JsonDSL::Value},
        !containsOtherRules
    };

    ruleValidationHelper::Rule Score = {
        JsonDSL::Scores,
        {JsonDSL::Score, JsonDSL::Ascending},
        !containsOtherRules
    };

    RuleMap map;
    map.insert(std::make_pair(dsl.getRuleString(Foreach.name), Foreach));
    map.insert(std::make_pair(dsl.getRuleString(Loop.name), Loop));
    map.insert(std::make_pair(dsl.getRuleString(Inparallel.name), Inparallel));
    map.insert(std::make_pair(dsl.getRuleString(Parallelfor.name), Parallelfor));
    map.insert(std::make_pair(dsl.getRuleString(Switch.name), Switch));
    map.insert(std::make_pair(dsl.getRuleString(When.name), When));
    map.insert(std::make_pair(dsl.getRuleString(Extend.name), Extend));
    map.insert(std::make_pair(dsl.getRuleString(Reverse.name), Reverse));
    map.insert(std::make_pair(dsl.getRuleString(Shuffle.name), Shuffle));
    map.insert(std::make_pair(dsl.getRuleString(Sort.name), Sort));
    map.insert(std::make_pair(dsl.getRuleString(Deal.name), Deal));
    map.insert(std::make_pair(dsl.getRuleString(Discard.name), Discard));
    map.insert(std::make_pair(dsl.getRuleString(Add.name), Add));
    map.insert(std::make_pair(dsl.getRuleString(Timer.name), Timer));
    map.insert(std::make_pair(dsl.getRuleString(InputChoice.name), InputChoice));
    map.insert(std::make_pair(dsl.getRuleString(InputText.name), InputText));
    map.insert(std::make_pair(dsl.getRuleString(InputVote.name), InputVote));
    map.insert(std::make_pair(dsl.getRuleString(Message.name), Message));
    map.insert(std::make_pair(dsl.getRuleString(GlobalMessage.name), GlobalMessage));
    map.insert(std::make_pair(dsl.getRuleString(Score.name), Score));
    return map;
}