#include "json.hpp"
#include "jsonDSL.h"
#include <string>

#ifndef FIELD_VALIDATORS_H
#define FIELD_VALIDATORS_H

class VariableValidator{
public:
    void validateVariableUsage(const nlohmann::json& j_object);
private:
    VariableValidator();
    friend class RuleValidator;
};

class RuleValidator{
public:
    VariableValidator validateRules(const nlohmann::json& j_object, JsonDSL::RuleType rule);
private:
    RuleValidator();
    friend class ConfigValidator;
};

class ConfigValidator{
public:
    RuleValidator validateConfig(const nlohmann::json& j_object);
private:
    ConfigValidator();
    friend class SpecificationValidator;
};

class SpecificationValidator{
public:
    ConfigValidator validateSpecification(const nlohmann::json& j_object);
};

#endif