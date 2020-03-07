#include <map>
#include <string>
#include "jsonDSL.h"

JsonDSL::JsonDSL(){
    mapStringToSpecification.insert({"configuration", JsonDSL::Configuration});
    mapStringToSpecification.insert({"constants", JsonDSL::Constants});
    mapStringToSpecification.insert({"variables", JsonDSL::Variables});
    mapStringToSpecification.insert({"per-player", JsonDSL::PerPlayer});
    mapStringToSpecification.insert({"per-audience", JsonDSL::PerAudience});
    mapStringToSpecification.insert({"rules", JsonDSL::Rules});
    
    mapStringToConfigFields.insert({"name", JsonDSL::GameName});
    mapStringToConfigFields.insert({"player count", JsonDSL::PlayerCount});
    mapStringToConfigFields.insert({"audience", JsonDSL::AllowAudience});
    mapStringToConfigFields.insert({"setup", JsonDSL::Setup});

    mapStringToRule.insert({"foreach", JsonDSL::Foreach});
    mapStringToRule.insert({"loop", JsonDSL::Loop});
    mapStringToRule.insert({"inparallel", JsonDSL::InParallel});
    mapStringToRule.insert({"parallelfor", JsonDSL::Parallelfor});
    mapStringToRule.insert({"switch", JsonDSL::Switch});
    mapStringToRule.insert({"when", JsonDSL::When});
    mapStringToRule.insert({"extend", JsonDSL::Extend});
    mapStringToRule.insert({"reverse", JsonDSL::Reverse});
    mapStringToRule.insert({"shuffle", JsonDSL::Shuffle});
    mapStringToRule.insert({"sort", JsonDSL::Sort});
    mapStringToRule.insert({"deal", JsonDSL::Deal});
    mapStringToRule.insert({"discard", JsonDSL::Discard});
    mapStringToRule.insert({"add", JsonDSL::Add});
    mapStringToRule.insert({"timer", JsonDSL::Timer});
    mapStringToRule.insert({"input-choice", JsonDSL::InputChoice});
    mapStringToRule.insert({"input-text", JsonDSL::InputText});
    mapStringToRule.insert({"input-vote", JsonDSL::InputVote});
    mapStringToRule.insert({"message", JsonDSL::Message});
    mapStringToRule.insert({"global-message", JsonDSL::GlobalMessage});
    mapStringToRule.insert({"scores", JsonDSL::Scores});
    
    mapStringToRuleParameters.insert({"list", JsonDSL::List});
    mapStringToRuleParameters.insert({"element", JsonDSL::Element});
    mapStringToRuleParameters.insert({"until", JsonDSL::Until});
    mapStringToRuleParameters.insert({"while", JsonDSL::While});
    mapStringToRuleParameters.insert({"value", JsonDSL::Value});
    mapStringToRuleParameters.insert({"cases", JsonDSL::Cases});
    mapStringToRuleParameters.insert({"case", JsonDSL::Case});
    mapStringToRuleParameters.insert({"target", JsonDSL::Target});
    mapStringToRuleParameters.insert({"key", JsonDSL::Key});
    mapStringToRuleParameters.insert({"from", JsonDSL::From});
    mapStringToRuleParameters.insert({"to", JsonDSL::To});
    mapStringToRuleParameters.insert({"count", JsonDSL::Count});
    mapStringToRuleParameters.insert({"duration", JsonDSL::Duration});
    mapStringToRuleParameters.insert({"mode", JsonDSL::Mode});
    mapStringToRuleParameters.insert({"flag", JsonDSL::Flag});
    mapStringToRuleParameters.insert({"timeout", JsonDSL::Timeout});
    mapStringToRuleParameters.insert({"prompt", JsonDSL::RulePrompt});
    mapStringToRuleParameters.insert({"result", JsonDSL::Result});
    mapStringToRuleParameters.insert({"choices", JsonDSL::Choices});
    mapStringToRuleParameters.insert({"score", JsonDSL::Score});
    mapStringToRuleParameters.insert({"ascending", JsonDSL::Ascending});
    mapStringToRuleParameters.insert({"rule", JsonDSL::Rule});
    mapStringToRuleParameters.insert({"condition", JsonDSL::Condition});

    mapStringToTimerModes.insert({"exact", JsonDSL::TimerExact});
    mapStringToTimerModes.insert({"at most", JsonDSL::TimerAtMost});
    mapStringToTimerModes.insert({"track", JsonDSL::TimerTrack});

    mapStringToSetup.insert({"kind", JsonDSL::Kind});
    mapStringToSetup.insert({"prompt", JsonDSL::SetupPrompt});
    mapStringToSetup.insert({"integer", JsonDSL::KindInteger});
    mapStringToSetup.insert({"string", JsonDSL::KindString});
    mapStringToSetup.insert({"boolean", JsonDSL::KindBoolean});
    mapStringToSetup.insert({"question-answer", JsonDSL::KindQuestionAnswer});
    mapStringToSetup.insert({"multiple-choice", JsonDSL::KindMultipleChoice});

    minPlayerString = "min";
    maxPlayerString = "max";
}

bool JsonDSL::isValidSpecificationField(const std::string& strSpecificationFIeld){
    return mapStringToSpecification.left.count(strSpecificationFIeld) == 1;
}

bool JsonDSL::isValidConfigField(const std::string& strConfigField){
    return mapStringToConfigFields.left.count(strConfigField) == 1;
}

bool JsonDSL::isValidPlayerRestrictionField(const std::string& strRestrictionField){
    return minPlayerString.compare(strRestrictionField) || maxPlayerString.compare(strRestrictionField);
}

bool JsonDSL::isValidRule(const std::string& strRule){
    return mapStringToRule.left.count(strRule) == 1;
}

bool JsonDSL::isValidRuleParameter(const std::string& strRuleParameter){
    return mapStringToRuleParameters.left.count(strRuleParameter) == 1;
}

bool JsonDSL::isValidTimerMode(const std::string& strTimerMode){
    return mapStringToTimerModes.left.count(strTimerMode) == 1;
}

bool JsonDSL::isValidSetupKind(const std::string& strSetupKind){
    return mapStringToSetup.left.count(strSetupKind) == 1;
}

std::pair<JsonDSL::specificationMap::left_const_iterator, 
            JsonDSL::specificationMap::left_const_iterator> JsonDSL::getSpecBeginEndIterators(){
    return std::make_pair(mapStringToSpecification.left.begin(), mapStringToSpecification.left.end());
}

std::pair<JsonDSL::configMap::left_const_iterator, 
            JsonDSL::configMap::left_const_iterator> JsonDSL::getConfigBeginEndIterators(){
    return std::make_pair(mapStringToConfigFields.left.begin(), mapStringToConfigFields.left.end());
}

std::pair<JsonDSL::ruleMap::left_const_iterator, 
            JsonDSL::ruleMap::left_const_iterator> JsonDSL::getRuleBeginEndIterators(){
    return std::make_pair(mapStringToRule.left.begin(), mapStringToRule.left.end());
}

std::pair<JsonDSL::ruleParameterMap::left_const_iterator, 
            JsonDSL::ruleParameterMap::left_const_iterator> JsonDSL::getRuleParameterBeginEndIterators(){
    return std::make_pair(mapStringToRuleParameters.left.begin(), mapStringToRuleParameters.left.end());
}

std::pair<JsonDSL::timerModeMap::left_const_iterator, 
            JsonDSL::timerModeMap::left_const_iterator> JsonDSL::getTimerModeBeginEndIterators(){
    return std::make_pair(mapStringToTimerModes.left.begin(), mapStringToTimerModes.left.end());
}

std::pair<JsonDSL::setupMap::left_const_iterator, 
            JsonDSL::setupMap::left_const_iterator> JsonDSL::getSetupBeginEndIterators(){
    return std::make_pair(mapStringToSetup.left.begin(), mapStringToSetup.left.end());
}

std::string JsonDSL::getSpecString(JsonDSL::SpecificationFields specification){
    return mapStringToSpecification.right.find(specification)->second;
}

std::string JsonDSL::getConfigString(JsonDSL::ConfigFields config){
    return mapStringToConfigFields.right.find(config)->second;
}

std::string JsonDSL::getPlayerRestrictionString(JsonDSL::PlayerRestriction restriction){
    if(restriction == JsonDSL::MinPlayers){
        return minPlayerString;
    } else{
        return maxPlayerString;
    }
}

std::string JsonDSL::getRuleString(JsonDSL::RuleType rule){
    return mapStringToRule.right.find(rule)->second;
}

std::string JsonDSL::getRuleParameterString(JsonDSL::RuleParameters ruleParameter){
    return mapStringToRuleParameters.right.find(ruleParameter)->second;
}

std::string JsonDSL::getTimerModeString(JsonDSL::TimerMode timerMode){
    return mapStringToTimerModes.right.find(timerMode)->second;
}

std::string JsonDSL::getSetupString(JsonDSL::SetupFields setup){
    return mapStringToSetup.right.find(setup)->second;
}