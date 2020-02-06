#include "jsonvalidator.h"
#include <map>
#include <algorithm>
#include "jsonDSL.h"
#include <exception>

using json = nlohmann::json;
using specificationIterator = JsonDSL::specificationMap::left_const_iterator;

ConfigValidator SpecificationValidator::validateSpecification(const json& j_object){
    JsonDSL dsl;
    std::pair<specificationIterator, specificationIterator> mapIterator = dsl.getSpecificationIterator();
    
    std::for_each(mapIterator.first, mapIterator.second, 
        [&j_object](auto& pair){
            std::string fieldToBeChecked = pair.first;
            if(!j_object.contains(fieldToBeChecked)){
                throw std::invalid_argument("Top level specification field: " + fieldToBeChecked + " not found");
            }
        }
    );

    return ConfigValidator();
}
