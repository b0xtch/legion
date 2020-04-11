#include "jsonDSL.h"
#include <vector>
#include <string>
#include "json.hpp"
#include "../abseil-cpp/absl/strings/str_split.h"
#include <boost/algorithm/string.hpp>

using json = nlohmann::json;
using varCollection = std::vector<std::string>;
using varMap = std::map<JsonDSL::VariableDataType, varCollection>;

struct MethodProperties{
    JsonDSL::VariableDataType methodCallerType;
    JsonDSL::VariableDataType argumentType;
    std::string methodName;
};

using functionList = std::vector<MethodProperties>;

namespace VariableHelper {

    JsonDSL::VariableDataType getTypeFromJson(const json& j_object);

    std::optional<json> getJsonFieldVarIsFrom(const json& j_object, std::string varName);

    bool isLiteralVar(JsonDSL::VariableDataType varType);

    std::optional<JsonDSL::VariableDataType> getTypeFromLiteral(std::string expression, varMap& map);

    void collectTopLevelVarsWithPrepend(json& j_object, const std::string& prependStr, varMap& map);

    void collectTopLevelVars(json& j_object, varMap& map);

    void collectSetUpVars(json& setupJson, varMap& map);

    varMap getEmptyVarMap();

    void collectObjectVarsWithPrepend(json& j_object,const std::string& prependStr, varMap& map);

    void collectObjectVars(json& j_object, varMap& map);

    std::optional<std::vector<size_t>> isMethodCall(std::string expression);

    bool isValidMethodCall(std::string expression, varMap& map, functionList& funcList, const json& j_object);

    bool isIntLiteral(std::string expression);

    bool literalIsDefined(std::string literal, varMap& map);

    std::optional<JsonDSL::VariableDataType> checkVarExistenceAndType(std::string varExpr, varMap& map, const json& j_object);

    std::pair<size_t, size_t> getVarAccessLocations(const std::string& strVal);

    std::vector<std::string> extractStringVarAccesses(std::string strVal);

    bool isBooleanExpression(std::string expression, varMap& map);

    varMap collectVarNames(json j_object);

}