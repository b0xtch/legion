#include <string>
#include <map>
#include <utility>
#include "boost/bimap.hpp"

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

    // TODO:: get it to kevs standard
    enum Arithmetic {
        upFrom,
        downFrom,
        add
    };

    typedef boost::bimap<std::string, SpecificationFields> specificationMap;
    typedef boost::bimap<std::string, ConfigFields> configMap;
    typedef boost::bimap<std::string, RuleType> ruleMap;
    typedef boost::bimap<std::string, RuleParameters> ruleParameterMap;
    typedef boost::bimap<std::string, TimerModes> timerModeMap;
    typedef boost::bimap<std::string, SetupFields> setupMap;

    bool isValidSpecificationField(const std::string&);
    bool isValidConfigField(const std::string&);
    bool isValidRule(const std::string&);
    bool isValidRuleParameter(const std::string&);
    bool isValidTimerMode(const std::string&);
    bool isValidSetupKind(const std::string&);

    std::pair<specificationMap::left_const_iterator, 
                specificationMap::left_const_iterator> getSpecificationIterator();
                
    std::pair<configMap::left_const_iterator, 
                configMap::left_const_iterator>getConfigIterator();

    std::pair<ruleMap::left_const_iterator, 
                ruleMap::left_const_iterator> getRuleIterator();

    std::pair<ruleParameterMap::left_const_iterator, 
                ruleParameterMap::left_const_iterator>getRuleParameterIterator();

    std::pair<timerModeMap::left_const_iterator, 
                timerModeMap::left_const_iterator>getTimerModeIterator();

    std::pair<setupMap::left_const_iterator, 
                setupMap::left_const_iterator>getSetupIterator();


    std::string getStringOfSpecificationCommand(SpecificationFields specification);
    std::string getStringOfConfigCommand(ConfigFields config);
    std::string getStringOfRuleCommand(RuleType rule);
    std::string getStringOfRuleParameterCommand(RuleParameters paramter);
    std::string getStringOfTimerModesCommand(TimerModes timerMode);
    std::string getStringOfSetupCommand(SetupFields setup);

private:
    specificationMap mapStringToSpecification;
    configMap mapStringToConfigFields;
    ruleMap mapStringToRule;
    ruleParameterMap mapStringToRuleParameters;
    timerModeMap mapStringToTimerModes;
    setupMap mapStringToSetup;
};
#endif