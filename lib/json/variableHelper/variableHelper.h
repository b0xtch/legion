#include "jsonDSL.h"
#include <vector>
#include <string>
#include "json.hpp"
#include "../abseil-cpp/absl/strings/str_split.h"
#include <boost/algorithm/string.hpp>

using json = nlohmann::json;
using varCollection = std::vector<std::string>;
using varMap = std::map<JsonDSL::VariableDataType, varCollection>;


namespace VariableHelper {

    JsonDSL::VariableDataType getTypeFromJson(const nlohmann::json& j_object);

    bool isLiteralVar(JsonDSL::VariableDataType varType);

    std::optional<JsonDSL::VariableDataType> getLiteralTypeFromString(std::string expression, varMap& map);

    void collectTopLevelVarsWithPrepend(json& j_object, const std::string& prependStr, varMap& map);

    void collectTopLevelVars(json& j_object, varMap& map);

    void collectSetUpVars(json& setupJson, varMap& map);

    varMap getEmptyVarMap();

    void collectObjectVarsWithPrepend(json& j_object,const std::string& prependStr, varMap& map);

    void collectObjectVars(json& j_object, varMap& map);

    bool isFunctionCall(std::string expression);

    bool isValidFunctionCall(std::string expression, varMap& map);

    bool literalIsDefined(std::string literal, varMap& map);

    std::pair<size_t, size_t> getVarAccessLocations(const std::string& strVal);

    std::vector<std::string> extractStringVarAccesses(std::string strVal);

    bool isBooleanExpression(std::string expression, varMap& map);

    varMap collectVarNames(json j_object);

}