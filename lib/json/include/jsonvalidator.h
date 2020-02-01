#include <json.hpp>
#include "jsonDSL.h
#include <string>

class JsonValidator{
public:
    bool validJson(nlohmann::json& j_object);
    bool isKeyValuePairValid(JsonDSL::LanguageCommands, std::string);
};