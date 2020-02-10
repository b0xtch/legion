#include "json.hpp"
#include "jsonDSL.h"
#include <string>

#ifndef JSON_VALIDATOR_H
#define JSON_VALIDATOR_H

class JsonValidator{
public:
    bool validJson(const nlohmann::json& j_object);
};
#endif