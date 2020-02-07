#include "json.hpp"
#include <string>

using json = nlohmann::json; 

namespace jsonConvert {
	std::string json_to_string(json& obj);
	json string_to_json(std::string& str);
	json input_to_json();
}