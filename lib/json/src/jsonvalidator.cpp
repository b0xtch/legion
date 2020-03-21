#include "jsonvalidator.h"
#include "fieldValidators.h"

bool JsonValidator::validJson(const nlohmann::json& j_object){
    bool isValid = true;
    try{
        SpecificationValidator{}.validateSpecification(j_object).validateConfig(j_object).validateRules(j_object).validateVariableUsage(j_object);
    }catch(std::invalid_argument e){
        isValid = false;
    }

    return isValid;
}