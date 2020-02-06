#include "jsonvalidator.h"
#include <nlohmann/json.hpp>
#include <map>
#include <algorithm>
#include "jsonDSL.h"

using json = nlohmann::json;

ConfigValidator SpecificationValidator::validateSpecification(const nlohmann::json& j_object){
    JsonDSL dsl = {};
    std::pair<std::map<std::string, JsonDSL::SpecificationFields>::iterator, 
            std::map<std::string, JsonDSL::SpecificationFields>::iterator> mapIterator = dsl.getSpecificationIterator();
    
    std::for_each(mapIterator->first, mapIterator->second, 
        [j_object&](std::pair<std::string, JsonDSL::SpecificationFields> pair){
            std::string fieldToBeChecked = pair->first;
            if(!j_object.contains(fieldToBeChecked)){
                throw std::invalid_argument;
            }
        }
    );

    return ConfigValidator config{};
}