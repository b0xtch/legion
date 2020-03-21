#include "fieldValidators.h"
#include "jsonDSL.h"
#include <vector>
#include <string>
#include "../abseil-cpp/absl/strings/str_split.h"
#include <boost/algorithm/string.hpp>

using json = nlohmann::json;

static JsonDSL dsl;

static std::vector<std::string> intFuncs = {"upfrom"};
static std::vector<std::string> listFuncs = {"collect", "contains", "elements"};

std::optional<JsonDSL::VariableDataType> getLiteralTypeFromString(std::string expression, varMap& map){
    auto boolVars = map[JsonDSL::VarBoolean];
    auto strVars = map[JsonDSL::VarString];
    auto intVars = map[JsonDSL::VarInteger];

    auto boolIt = std::find(boolVars.begin(), boolVars.end(), expression);
    auto strIt = std::find(strVars.begin(), strVars.end(), expression);
    auto intIt = std::find(intVars.begin(), intVars.end(), expression);

    bool boolFound = boolIt != boolVars.end();
    bool strFound = strIt != strVars.end();
    bool intFound = intIt != intVars.end();

    if(boolFound){
        return JsonDSL::VarBoolean;
    }else if(strFound){
        return JsonDSL::VarString;
    }else if(intFound){
        return JsonDSL::VarInteger;
    }else {
        return {};
    }
}

JsonDSL::VariableDataType getTypeFromJson(const nlohmann::json& j_object){
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

static void collectLiteralVarsWithPrepend(json& j_object, const std::string& prependStr, varMap& map){
    for(auto jsonItem : j_object.items()){
        JsonDSL::VariableDataType varType = getTypeFromJson(jsonItem);

        if(!isLiteralVar(varType)){
            continue;
        }

        std::stringstream varName;
        varName << prependStr << jsonItem.key();

        map[varType].push_back(varName.str());
        j_object.erase(jsonItem.key());
    }
}

static void collectLiteralVars(json& j_object, varMap& map){
    collectLiteralVarsWithPrepend(j_object, "", map);
}

static varMap getEmptyVarMap(){
    varMap map;
    map.insert(std::make_pair(JsonDSL::VarInteger, varCollection{}));
    map.insert(std::make_pair(JsonDSL::VarBoolean, varCollection{}));
    map.insert(std::make_pair(JsonDSL::VarString, varCollection{}));
    map.insert(std::make_pair(JsonDSL::VarList, varCollection{}));
    map.insert(std::make_pair(JsonDSL::VarObject, varCollection{}));
    return map;
}

static void collectListAndObjects(json& j_object, varMap& map){

}

bool isBooleanExpression(std::string expression, varMap& map){
    varCollection& bools = map[JsonDSL::VarBoolean];
    auto boolIt = std::find(bools.begin(), bools.end(), expression);
    if(boolIt != bools.end()){
        return true;
    }

    std::string compareOp = "==";
    auto compareLoc = expression.find(compareOp);
    
    if(compareLoc == std::string::npos){
        return false;
    }

    std::vector<std::string> args = absl::StrSplit(expression, compareOp);

    //assuming that boolean expressions do not include && or || as it's not listed in spec
    if(args.size() != 2){
        return false;
    }

    boost::trim(args.at(0));
    boost::trim(args.at(1));

    auto firstVarType = getLiteralTypeFromString(args.at(0), map);
    auto secondVarType = getLiteralTypeFromString(args.at(1), map);

    if(!firstVarType.has_value || !secondVarType.has_value){
        return false;
    }
    
    return firstVarType.value == secondVarType.value;
}


static varMap collectAllVariables(const json& j_object){
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

    varMap map = getEmptyVarMap();

    collectLiteralVarsWithPrepend(perPlayerVars, "players.", map);
    collectLiteralVarsWithPrepend(perAudienceVars, "audience.", map);
    collectLiteralVars(setupVars, map);
    collectLiteralVars(constVars, map);
    collectLiteralVars(varVars, map);
    
    return map;
}

void VariableValidator::validateVariableUsage(const json& j_object){
    std::string rulesStr = dsl.getSpecString(JsonDSL::Rules);
    json rules = j_object[rulesStr];
    varMap map = collectAllVariables(j_object);
}

VariableValidator::VariableValidator(){}