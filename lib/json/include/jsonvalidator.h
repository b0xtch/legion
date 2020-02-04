#include "json.hpp"
#include "jsonDSL.h"
#include <string>

class JsonValidator{
public:
    bool validJson(nlohmann::json& j_object);
};

class RuleValidator{
public:
    nlohmann::json retrieveRule(nlohmann::json j_object);
    void validateRule(const nlohmann::json& j_object, JsonDSL::RuleType rule);
};

class ConfigValidator{
public:
    nlohmann::json retrieveConfig(nlohmann::json j_object);
    RuleValidator validateConfig(const nlohmann::json& j_object);
};

class SpecificationValidator{
public:
    ConfigValidator validateSpecification(const nlohmann::json& j_object);
};