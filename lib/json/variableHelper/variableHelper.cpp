#include "variableHelper.h"

using json = nlohmann::json;

static JsonDSL dsl;

using varCollection = std::vector<std::string>;
using varMap = std::map<JsonDSL::VariableDataType, varCollection>;


namespace VH = VariableHelper;

JsonDSL::VariableDataType VH::getTypeFromJson(const nlohmann::json& j_object){
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

bool VH::isLiteralVar(JsonDSL::VariableDataType varType){
    bool isInt = varType == JsonDSL::VarInteger;
    bool isBool = varType == JsonDSL::VarBoolean;
    bool isString = varType == JsonDSL::VarString;

    return isInt || isBool || isString;
}

std::optional<JsonDSL::VariableDataType> VH::getLiteralTypeFromString(std::string expression, varMap& map){
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

void VH::collectTopLevelVarsWithPrepend(json& j_object, const std::string& prependStr, varMap& map){
    for(auto jsonItem : j_object.items()){
        JsonDSL::VariableDataType varType = VH::getTypeFromJson(jsonItem);

        std::stringstream varName;
        varName << prependStr << jsonItem.key();

        map[varType].push_back(varName.str());
        j_object.erase(jsonItem.key());
    }
}

void VH::collectTopLevelVars(json& j_object, varMap& map){
    VH::collectTopLevelVarsWithPrepend(j_object, "", map);
}


//function to extract the user defined variables inside setup
//and place them in the appropriate vector based on its type
void VH::collectSetUpVars(json& setupJson, varMap& map){
    for(auto jsonItem : setupJson.items()){
        JsonDSL::VariableDataType varType = VH::getTypeFromJson(jsonItem);

        bool notObject = varType != JsonDSL::VarObject;

        std::string kindField = dsl.getSetupString(JsonDSL::Kind);
        bool notUserDef = jsonItem.value().contains(kindField);
        
        if(notObject || notUserDef){
            continue;
        }

        std::string intVar = dsl.getSetupString(JsonDSL::KindInteger);
        std::string boolVar = dsl.getSetupString(JsonDSL::KindBoolean);
        std::string strVar = dsl.getSetupString(JsonDSL::KindString);
        std::string listVar = dsl.getSetupString(JsonDSL::KindMultipleChoice);
        std::string objVar = dsl.getSetupString(JsonDSL::KindQuestionAnswer);

        if(jsonItem.value()[kindField] == intVar){
            varType = JsonDSL::VarInteger;
        } else if (jsonItem.value()[kindField] == boolVar){ 
            varType = JsonDSL::VarBoolean;
        } else if (jsonItem.value()[kindField] == strVar){ 
            varType = JsonDSL::VarString;
        } else if (jsonItem.value()[kindField] == listVar){ 
            varType = JsonDSL::VarList;
        } else if (jsonItem.value()[kindField] == objVar){ 
            varType = JsonDSL::VarObject;
        }

        std::stringstream varName;
        varName << "Configuration." << jsonItem.key();

        map[varType].push_back(varName.str());
        setupJson.erase(jsonItem.key());
    }
}

varMap VH::getEmptyVarMap(){
    varMap map;
    map.insert(std::make_pair(JsonDSL::VarInteger, varCollection{}));
    map.insert(std::make_pair(JsonDSL::VarBoolean, varCollection{}));
    map.insert(std::make_pair(JsonDSL::VarString, varCollection{}));
    map.insert(std::make_pair(JsonDSL::VarList, varCollection{}));
    map.insert(std::make_pair(JsonDSL::VarObject, varCollection{}));
    return map;
}

void VH::collectObjectVarsWithPrepend(json& j_object,const std::string& prependStr, varMap& map){
    for(auto jsonItem : j_object.items()){
        JsonDSL::VariableDataType varType = VH::getTypeFromJson(jsonItem);

        if(varType != JsonDSL::VarObject){
            continue;
        }

        std::string objName = prependStr + jsonItem.key();
        VH::collectTopLevelVarsWithPrepend(jsonItem.value(), objName, map);

    }
}

void VH::collectObjectVars(json& j_object, varMap& map){
    VH::collectObjectVarsWithPrepend(j_object, "", map);
}

bool VH::isFunctionCall(std::string expression){

}

bool VH::isValidFunctionCall(std::string expression, varMap& map){

}

bool VH::literalIsDefined(std::string literal, varMap& map){
    std::vector<varCollection> literalMaps = {map[JsonDSL::VarBoolean], map[JsonDSL::VarInteger], map[JsonDSL::VarString]};
    auto it = std::find_if(literalMaps.begin(), literalMaps.end(), 
        [&literal](const varCollection& collection){
            return std::find(collection.begin(), collection.end(), literal) != collection.end();
        });
    return it != literalMaps.end();
}

std::pair<size_t, size_t> VH::getVarAccessLocations(const std::string& strVal){
    std::string accessStart = "{";
    std::string accessEnd = "}";

    auto startLoc = strVal.find(accessStart);
    auto endLoc = strVal.find(accessEnd);
    
    bool accessNotFound = startLoc == std::string::npos || endLoc == std::string::npos;
    bool endIsBeforeStart = startLoc > endLoc;

    if (accessNotFound || endIsBeforeStart){
        startLoc = std::string::npos;
        endLoc = std::string::npos;
    }

    return std::make_pair(startLoc, endLoc);
}

std::vector<std::string> VH::extractStringVarAccesses(std::string strVal){
    std::vector<std::string> vars;
    auto varAccess = VH::getVarAccessLocations(strVal);
    size_t notFoundValue = std::string::npos;

    while(varAccess.first != notFoundValue){
        auto start = varAccess.first;
        auto end = varAccess.second;

        std::string var = strVal.substr(start+1, end-start-1);
        vars.push_back(var);
        strVal = strVal.substr(end + 1);
        varAccess = VH::getVarAccessLocations(strVal);
    }

    return vars;
}

bool VH::isBooleanExpression(std::string expression, varMap& map){
    varCollection& bools = map[JsonDSL::VarBoolean];

    std::string trimmedExpr = expression;
    boost::trim(trimmedExpr);

    auto boolIt = std::find(bools.begin(), bools.end(), trimmedExpr);
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

    auto firstVarType = VH::getLiteralTypeFromString(args.at(0), map);
    auto secondVarType = VH::getLiteralTypeFromString(args.at(1), map);

    if(!firstVarType.has_value() || !secondVarType.has_value()){
        return false;
    }
    
    return firstVarType.value() == secondVarType.value();
}


varMap VH::collectVarNames(json j_object){
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

    varMap map = VH::getEmptyVarMap();

    map[JsonDSL::VarBoolean].push_back("true");
    map[JsonDSL::VarBoolean].push_back("false");

    map[JsonDSL::VarList].push_back("players");
    map[JsonDSL::VarList].push_back("audience");

    VH::collectSetUpVars(setupVars, map);
    VH::collectTopLevelVarsWithPrepend(setupVars, "Configuration.", map);
    VH::collectTopLevelVars(constVars, map);
    VH::collectTopLevelVars(varVars, map);

    return map;
}
