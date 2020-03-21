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
        GameName, PlayerCount, AllowAudience, Setup
    };

    enum PlayerRestriction{
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
        Score, Ascending, Rule, Condition
    };

    enum TimerMode{
        TimerExact, TimerAtMost, TimerTrack
    };

    enum SetupFields{
        Kind, SetupPrompt,
        KindInteger, KindString, KindBoolean,
        KindQuestionAnswer, KindMultipleChoice  
    };

    enum VariableDataType{
        VarInteger, VarBoolean, VarString, VarList, VarObject
    };

    // TODO:: get it to kevs standard
    enum Arithmetic {
        ADD,
        SUBTRACT,
        MULTIPLY,
        DIVIDE
    };

    typedef boost::bimap<std::string, SpecificationFields> specificationMap;
    typedef boost::bimap<std::string, ConfigFields> configMap;
    typedef boost::bimap<std::string, RuleType> ruleMap;
    typedef boost::bimap<std::string, RuleParameters> ruleParameterMap;
    typedef boost::bimap<std::string, TimerMode> timerModeMap;
    typedef boost::bimap<std::string, SetupFields> setupMap;

    bool isValidSpecificationField(const std::string&);
    bool isValidConfigField(const std::string&);
    bool isValidPlayerRestrictionField(const std::string&);
    bool isValidRule(const std::string&);
    bool isValidRuleParameter(const std::string&);
    bool isValidTimerMode(const std::string&);
    bool isValidSetupKind(const std::string&);

    std::pair<specificationMap::left_const_iterator, 
                specificationMap::left_const_iterator> getSpecBeginEndIterators();
                
    std::pair<configMap::left_const_iterator, 
                configMap::left_const_iterator>getConfigBeginEndIterators();

    std::pair<ruleMap::left_const_iterator, 
                ruleMap::left_const_iterator> getRuleBeginEndIterators();

    std::pair<ruleParameterMap::left_const_iterator, 
                ruleParameterMap::left_const_iterator>getRuleParameterBeginEndIterators();

    std::pair<timerModeMap::left_const_iterator, 
                timerModeMap::left_const_iterator>getTimerModeBeginEndIterators();

    std::pair<setupMap::left_const_iterator, 
                setupMap::left_const_iterator>getSetupBeginEndIterators();


    std::string getSpecString(SpecificationFields specification);
    std::string getConfigString(ConfigFields config);
    std::string getPlayerRestrictionString(PlayerRestriction restriction);
    std::string getRuleString(RuleType rule);
    std::string getRuleParameterString(RuleParameters paramter);
    std::string getTimerModeString(TimerMode timerMode);
    std::string getSetupString(SetupFields setup);

private:
    specificationMap mapStringToSpecification;
    configMap mapStringToConfigFields;
    ruleMap mapStringToRule;
    ruleParameterMap mapStringToRuleParameters;
    timerModeMap mapStringToTimerModes;
    setupMap mapStringToSetup;
    std::string minPlayerString;
    std::string maxPlayerString;
};
#endif