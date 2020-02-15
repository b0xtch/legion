#include "json.hpp"
#include "jsonDSL.h"
#include <string>

#ifndef FIELD_VALIDATORS_H
#define FIELD_VALIDATORS_H

class VariableValidator{
public:
    void validateVariableUsage(const nlohmann::json& j_object);
private:
    std::vector<std::pair<std::string, JsonDSL::VariableDataType>> findVariableUsage(const nlohmann::json& j_object);private:
    JsonDSL::VariableDataType checkForDataType(const nlohmann::json& j_object);
};

class RuleValidator{
public:
    VariableValidator validateRules(const nlohmann::json& j_object, JsonDSL::RuleType rule);
};

class ConfigValidator{
public:
    RuleValidator validateConfig(const nlohmann::json& j_object);
private:
    void validateAllFieldsAreValid(const nlohmann::json& j_object);
    void validateAllNecessaryFieldsPresent(const nlohmann::json& j_object);
    void validateNumPlayersRestrictionValid(const nlohmann::json& j_object);
};

class SpecificationValidator{
public:
    ConfigValidator validateSpecification(const nlohmann::json& j_object);
private:
    void validateAllFieldsAreValid(const nlohmann::json& j_object);
    void validateAllNecessaryFieldsPresent(const nlohmann::json& j_object);
};

#endif