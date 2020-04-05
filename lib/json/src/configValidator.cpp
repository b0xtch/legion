#include "fieldValidators.h"
#include "jsonDSL.h"
#include <algorithm>
#include <exception>

using json = nlohmann::json;
using configIterator = JsonDSL::configMap::left_const_iterator;

static JsonDSL dsl;

static void validateAllNecessaryFieldsPresent(const json& j_object){
    std::pair<configIterator, configIterator> mapIterator = dsl.getConfigBeginEndIterators();
    std::string configuration = dsl.getSpecString(JsonDSL::Configuration);
    json configurations = j_object[configuration];

    if (configurations.size() > JsonDSL::ConfigFields(0)) {
        throw std::invalid_argument("Config has extra fields that are not required.");
    } else if (configurations.size() < JsonDSL::ConfigFields(0)) {
        throw std::invalid_argument("Config does not have all required fields.");
    }

    auto it = std::find_if(mapIterator.first, mapIterator.second,
        [&configurations](auto& pair) {
            std::string fieldToBeChecked = pair.first;
            return !configurations.contains(fieldToBeChecked);
        }
    );

    if(it != mapIterator.second){
        throw std::invalid_argument("Config Field: " + it->first + " not found.");
    }

    std::string playerCount = dsl.getConfigString(JsonDSL::PlayerCount);
    std::string minPlayerStr = dsl.getPlayerRestrictionString(JsonDSL::MinPlayers);
    std::string maxPlayerStr = dsl.getPlayerRestrictionString(JsonDSL::MaxPlayers);

    json playerCountConfig = j_object[configuration][playerCount];
    if(!playerCountConfig.contains("min")){
        throw std::invalid_argument("Min player count not found");
    }

    if(!playerCountConfig.contains("max")){
        throw std::invalid_argument("Max player count not found");
    }

}

static void validateAllFieldsAreValid(const json& j_object){
    json configuration = j_object[dsl.getSpecString(JsonDSL::Configuration)];
    for(auto jsonItem : configuration.items()){
        if(!dsl.isValidConfigField(jsonItem.key())){
            throw std::invalid_argument("An illegal key was found in the configuration level of the json file");
        }
    }
}

static void validateNumPlayersRestrictionValid(const json& j_object){
    std::string configuration = dsl.getSpecString(JsonDSL::Configuration);
    std::string playerCount = dsl.getConfigString(JsonDSL::PlayerCount);
    std::string minPlayerStr = dsl.getPlayerRestrictionString(JsonDSL::MinPlayers);
    std::string maxPlayerStr = dsl.getPlayerRestrictionString(JsonDSL::MaxPlayers);

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

RulesValidator ConfigValidator::validateConfig(const json& j_object){
    //validateAllFieldsAreValid(j_object);
    validateAllNecessaryFieldsPresent(j_object);
    validateNumPlayersRestrictionValid(j_object);
    return RulesValidator();
}

ConfigValidator::ConfigValidator(){}
