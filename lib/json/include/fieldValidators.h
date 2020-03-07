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
    friend class RulesValidator;
};

class RulesValidator{
public:
    VariableValidator validateRules(const nlohmann::json& j_object);
private:
    RulesValidator();
    friend class ConfigValidator;
};

class ConfigValidator{
public:
    RulesValidator validateConfig(const nlohmann::json& j_object);
private:
    ConfigValidator();
    friend class SpecificationValidator;
};

class SpecificationValidator{
public:
    ConfigValidator validateSpecification(const nlohmann::json& j_object);
};

#endif