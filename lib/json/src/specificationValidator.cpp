#include "fieldValidators.h"
#include "jsonDSL.h"
#include <algorithm>
#include <exception>

using json = nlohmann::json;
using specificationIterator = JsonDSL::specificationMap::left_const_iterator;

ConfigValidator SpecificationValidator::validateSpecification(const json& j_object){
    SpecificationValidator::validateAllFieldsAreValid(j_object);
    SpecificationValidator::validateAllNecessaryFieldsPresent(j_object);
    return ConfigValidator();
}

void SpecificationValidator::validateAllNecessaryFieldsPresent(const json& j_object){
    JsonDSL dsl;
    std::pair<specificationIterator, specificationIterator> mapIterator = dsl.getSpecBeginEndIterators();
    
    auto it = std::find_if(mapIterator.first, mapIterator.second, 
        [&j_object](auto& pair){
            std::string fieldToBeChecked = pair.first;
            return !j_object.contains(fieldToBeChecked);
        }
    );

    if(it != mapIterator.second){
        throw std::invalid_argument("Top level specification Field: " + it->first + " not found.");
    }
}

void SpecificationValidator::validateAllFieldsAreValid(const json& j_object){
    JsonDSL dsl;
    for(auto jsonItem : j_object.items()){
        if(!dsl.isValidSpecificationField(jsonItem.key())){
            throw std::invalid_argument("An illegal key was found on the top level specification level of the json file");
        }
    }
}