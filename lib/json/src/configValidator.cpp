#include "fieldValidators.h"
#include "jsonDSL.h"
#include <algorithm>
#include <exception>

using json = nlohmann::json;
using configIterator = JsonDSL::configMap::left_const_iterator;

RuleValidator ConfigValidator::validateConfig(const json& j_object){
    ConfigValidator::validateAllFieldsAreValid(j_object);
    ConfigValidator::validateAllNecessaryFieldsPresent(j_object);
    ConfigValidator::validateNumPlayersRestrictionValid(j_object);
    return RuleValidator();
}

void ConfigValidator::validateAllFieldsAreValid(const json& j_object){
    JsonDSL dsl;
    std::pair<configIterator, configIterator> mapIterator = dsl.getConfigIterator();
    json configurations = j_object[dsl.getStringOfSpecificationCommand(JsonDSL::Configuration)];
    
    std::string minPlayerStr = dsl.getStringOfConfigCommand(JsonDSL::MinPlayers);
    std::string maxPlayerStr = dsl.getStringOfConfigCommand(JsonDSL::MaxPlayers);

    std::for_each(mapIterator.first, mapIterator.second, 
        [&configurations, &dsl, &minPlayerStr, &maxPlayerStr](auto& pair) {
            std::string fieldToBeChecked = pair.first;
            bool isMinOrMax = fieldToBeChecked.compare(minPlayerStr) || fieldToBeChecked.compare(maxPlayerStr);
            if(!configurations.contains(fieldToBeChecked) && !isMinOrMax){
                throw std::invalid_argument("Config field: " + fieldToBeChecked + " not found");
            }
        }
    );
}

void ConfigValidator::validateAllNecessaryFieldsPresent(const json& j_object){
    JsonDSL dsl;
    json configuration = j_object[dsl.getStringOfSpecificationCommand(JsonDSL::Configuration)];
    for(auto jsonItem : configuration.items()){
        if(!dsl.isValidConfigField(jsonItem.key())){
            throw std::invalid_argument("An illegal key was found in the configuration level of the json file");
        }
    }
}

void ConfigValidator::validateNumPlayersRestrictionValid(const json& j_object){
    JsonDSL dsl;
    std::string configuration = dsl.getStringOfSpecificationCommand(JsonDSL::Configuration);
    std::string playerCount = dsl.getStringOfConfigCommand(JsonDSL::PlayerCount);
    std::string minPlayerStr = dsl.getStringOfConfigCommand(JsonDSL::MinPlayers);
    std::string maxPlayerStr = dsl.getStringOfConfigCommand(JsonDSL::MaxPlayers);
    
    json minPlayersJson = j_object[configuration][playerCount][minPlayerStr];
    json maxPlayersJson = j_object[configuration][playerCount][maxPlayerStr];

    if(!minPlayersJson.is_number_integer() || !maxPlayersJson.is_number_integer()){
        throw std::invalid_argument("The values inside the players count object are not both integers");
    }

    int minPlayersInt = minPlayersJson.get<int>();
    int maxPlayersInt = maxPlayersJson.get<int>();

    if(minPlayersInt < 0 || maxPlayersInt < 0){
        throw std::invalid_argument("Player count values are not both greater than or equal to 0");
    }
    
    if(maxPlayersInt < minPlayersInt){
        throw std::invalid_argument("The minimum number of players must be less than or equal to the number of max players");
    }
}