#include <string>
#include <map>
#include <utility>
#ifndef JSON_DSL_H
#define JSON_DSL_H

class JsonDSL{
public:
    JsonDSL();
    enum SpecificationFields{
        Configuration, Constants, Variables, 
        PerPlayer, PerAudience, Rules};

    enum ConfigFields{
        GameName, PlayerCount, AllowAudience, Setup,
        MinPlayers, MaxPlayers
    };

    enum RuleType{
        Foreach, Loop, InParallel, Parallelfor,
        Switch, When, Extend, Reverse, Shuffle, 
        Sort, Deal, Discard, Add, Timer, 
        InputChoice, InputText, InputVote, 
        Message, GlobalMessage, Scores
    };

    enum RuleParameters{
        List, Element, Until, While, Value, Cases, 
        Case, Target, Key, From, To, Count, Duration, 
        Mode, Flag, Timeout, RulePrompt, Result, Choices,
        Score, Ascending
    };

    enum TimerModes{
        TimerExact, TimerAtMost, TimerTrack
    };

    enum SetupFields{
        Kind, SetupPrompt,
        KindInteger, KindString, KindBoolean,
        KindQuestionAnswer, KindMultipleChoice  
    };


    bool isValidSpecificationField(const std::string&);
    bool isValidConfigField(const std::string&);
    bool isValidRule(const std::string&);
    bool isValidRuleParameter(const std::string&);
    bool isValidTimerMode(const std::string&);
    bool isValidSetupKind(const std::string&);

    std::pair<std::map<std::string, SpecificationFields>::iterator, 
                std::map<std::string, SpecificationFields>::iterator> getSpecificationIterator();
                
    std::pair<std::map<std::string, ConfigFields>::iterator, 
                std::map<std::string, ConfigFields>::iterator>getConfigIterator();

    std::pair<std::map<std::string, RuleType>::iterator, 
                std::map<std::string, RuleType>::iterator> getRuleIterator();

    std::pair<std::map<std::string, RuleParameters>::iterator, 
                std::map<std::string, RuleParameters>::iterator>getRuleParameterIterator();

    std::pair<std::map<std::string, TimerModes>::iterator, 
                std::map<std::string, TimerModes>::iterator>getTimerModeIterator();

    std::pair<std::map<std::string, SetupFields>::iterator, 
                std::map<std::string, SetupFields>::iterator>getSetupIterator();

private:
    std::map<std::string, SpecificationFields> mapStringToSpecification;
    std::map<std::string, ConfigFields> mapStringToConfigFields;
    std::map<std::string, RuleType> mapStringToRule;
    std::map<std::string, RuleParameters> mapStringToRuleParameters;
    std::map<std::string, TimerModes> mapStringToTimerModes;
    std::map<std::string, SetupFields> mapStringToSetup;
};
#endif