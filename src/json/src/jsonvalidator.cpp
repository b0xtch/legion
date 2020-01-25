#include "jsonvalidator.h"
#include <nlohmann/json.hpp>
#include "jsonDSL.h"

using json = nlohmann::json;

bool isJsonValid(json& j_object) {
    for (auto& x : json::iterator_wrapper(j_object))
    {
        if(!isKeyValuePairValid(x.key(), x.value())){
            return false;
        }
    }
}

bool isKeyValuePairValid(JsonDSL::LanguageCommands, auto value){}