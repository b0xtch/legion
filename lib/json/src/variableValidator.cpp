#include "fieldValidators.h"
#include "jsonDSL.h"
#include <vector>
#include <string>
#include "../abseil-cpp/absl/strings/str_split.h"
#include <boost/algorithm/string.hpp>

using json = nlohmann::json;

static JsonDSL dsl;

static std::vector<std::string> intFuncs = {"upfrom"};
static std::vector<std::string> listFuncs = {"collect", "contains"};

void VariableValidator::validateVariableUsage(const json& j_object){
    std::string rulesStr = dsl.getSpecString(JsonDSL::Rules);
    json rules = j_object[rulesStr];
    //varMap map = collectVarNames(j_object);
}

VariableValidator::VariableValidator(){}