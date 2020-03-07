#include "fieldValidators.h"
#include "jsonDSL.h"

using json = nlohmann::json;

JsonDSL::VariableDataType checkForDataType(const nlohmann::json& j_object){
    if(j_object.is_number_integer()){
        return JsonDSL::VarInteger;
    }else if (j_object.is_boolean()){
        return JsonDSL::VarBoolean;
    }else if (j_object.is_string()){
        return JsonDSL::VarString;
    }else if (j_object.is_structured()){
        return JsonDSL::VarStructured;
    }else{
        throw std::invalid_argument("Invalid data type being used in json");
    }
}