#include<map>
#include<string>
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
    mapStringToConfigFields.insert({"min", JsonDSL::MinPlayers});
    mapStringToConfigFields.insert({"max", JsonDSL::MaxPlayers});

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

}

bool JsonDSL::isValidSpecificationField(const std::string& strSpecificationFIeld){
    return mapStringToSpecification.count(strSpecificationFIeld) == 1;
}

bool JsonDSL::isValidConfigField(const std::string& strConfigField){
    return mapStringToConfigFields.count(strConfigField) == 1;
}

bool JsonDSL::isValidRule(const std::string& strRule){
    return mapStringToRule.count(strRule) == 1;
}

bool JsonDSL::isValidRuleParameter(const std::string& strRuleParameter){
    return mapStringToRuleParameters.count(strRuleParameter) == 1;
}

bool JsonDSL::isValidTimerMode(const std::string& strTimerMode){
    return mapStringToTimerModes.count(strTimerMode) == 1;
}

bool JsonDSL::isValidSetupKind(const std::string& strSetupKind){
    return mapStringToSetup.count(strSetupKind) == 1;
}