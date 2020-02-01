#include <iostream>
#include <string>
#include <exception>

#include "jsonConvert.h"
#include "json.hpp"


using json = nlohmann::json; 


std::string jsonConvert::json_to_string(json& obj){
	std::string res = obj.dump(1);
	if(res == "null") {
		std::cout << "json_to_string: Empty json object entered, returning null json \n";
	}
	return res;
}

json jsonConvert::string_to_json(std::string& str){
	json res;	//default as null json
	try{
		res = json::parse(str);
		return res;
	} catch(std::exception const & e){
		std::cout << "\tInvalid json string entered...\n";
		std::cout << "\tException message from json::parse: \n\t" << e.what() << "\n";
		return res;
	}
}

json jsonConvert::input_to_json(){
	std::string userInput;
	std::cout << "Input string to use as json: \n";
	std::getline(std::cin, userInput);
	return string_to_json(userInput);
}