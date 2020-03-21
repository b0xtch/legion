#include "fieldValidators.h"
#include "jsonDSL.h"
#include <vector>
#include <string>

using json = nlohmann::json;

static JsonDSL dsl;

JsonDSL::VariableDataType checkForDataType(const nlohmann::json& j_object){
    if(j_object.is_number_integer()){
        return JsonDSL::VarInteger;
    }else if (j_object.is_boolean()){
        return JsonDSL::VarBoolean;
    }else if (j_object.is_string()){
        return JsonDSL::VarString;
    }else if (j_object.is_array()){
        return JsonDSL::VarList;
    }else if (j_object.is_object()){
        return JsonDSL::VarObject;
    }else{
        throw std::invalid_argument("Invalid data type being used in json");
    }
}

static bool isLiteralVar(JsonDSL::VariableDataType varType){
    bool isInt = varType == JsonDSL::VarInteger;
    bool isBool = varType == JsonDSL::VarBoolean;
    bool isString = varType == JsonDSL::VarString;

    return isInt || isBool || isString;
}

using varCollection = std::vector<std::string>;
using varMap = std::map<JsonDSL::VariableDataType, varCollection>;

static void collectLiteralVarsWithPrepend(json& j_object, const std::string& prependStr, const varMap& map){
    
}

static varMap setUpVarMap(){
    varMap map;
    map.insert(std::make_pair(JsonDSL::VarInteger, varCollection{}));
    map.insert(std::make_pair(JsonDSL::VarBoolean, varCollection{}));
    map.insert(std::make_pair(JsonDSL::VarString, varCollection{}));
    map.insert(std::make_pair(JsonDSL::VarList, varCollection{}));
    map.insert(std::make_pair(JsonDSL::VarObject, varCollection{}));
    return map;
}

static void collectAllVariables(const json& j_object){
    std::string configStr = dsl.getSpecString(JsonDSL::Configuration);
    std::string setupStr = dsl.getConfigString(JsonDSL::Setup);
    std::string perPlayerStr = dsl.getSpecString(JsonDSL::PerPlayer);
    std::string perAudienceStr = dsl.getSpecString(JsonDSL::PerAudience);
    std::string constStr = dsl.getSpecString(JsonDSL::Constants);
    std::string varStr = dsl.getSpecString(JsonDSL::Variables);

    json perPlayerVars = j_object[perPlayerStr];
    json perAudienceVars = j_object[perAudienceStr];
    json setupVars = j_object[configStr][setupStr];
    json constVars = j_object[constStr];
    json varVars = j_object[varStr];

    collectLiteralVarsWithPrepend(perPlayerVars, "players");
    collectLiteralVarsWithPrepend(perAudienceVars, "audience");
    
}

void VariableValidator::validateVariableUsage(const json& j_object){
    std::string rulesStr = dsl.getSpecString(JsonDSL::Rules);
    json rules = j_object[rulesStr];
    collectAllVariables(j_object);
}

VariableValidator::VariableValidator(){}