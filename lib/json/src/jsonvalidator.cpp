#include "jsonvalidator.h"
#include <map>
#include <algorithm>
#include "jsonDSL.h"
#include <exception>

using json = nlohmann::json;
using specificationIterator = std::map<std::string, JsonDSL::SpecificationFields>::iterator;

ConfigValidator SpecificationValidator::validateSpecification(const json& j_object){
    JsonDSL dsl;
    std::pair<specificationIterator, specificationIterator> mapIterator = dsl.getSpecificationIterator();
    
    std::for_each(mapIterator.first, mapIterator.second, 
        [&j_object](std::pair<std::string, JsonDSL::SpecificationFields> pair){
            std::string fieldToBeChecked = pair.first;
            if(!j_object.contains(fieldToBeChecked)){
                throw std::invalid_argument("Top level specification field: " + fieldToBeChecked + " not found");
            }
        }
    );

    return ConfigValidator();
}