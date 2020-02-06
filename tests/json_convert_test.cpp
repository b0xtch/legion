#include <iostream>
#include <string>
#include <exception>
#include <assert.h>
#include <typeinfo>

#include "jsonConvert.h"
#include "json.hpp"

using json = nlohmann::json; 

int main(int argc, char const *argv[])
{

/*
* GOOD STRING
*/


	std::string good_string = R"(
	{
	  "name": "Metro Vancouver",
	  "cities": ["Vancouver", "Burnaby", "Surrey", "Richmond", "Coquitlam"],
	  "areaCodes": [604, 778],
	  "population": 2463000,
	  "location": {
	  	"province": "British Columbia",
	  	"country": "Canada",
	  	"continent": "North America"
	  },
	  "weedLegal": true
	}
     )";

/*
* BAD STRING
* missing closing } at the end
*/
    std::string bad_string = R"(
	{
	  "name": "Metro Vancouver",
	  "cities": ["Vancouver", "Burnaby", "Surrey", "Richmond", "Coquitlam"],
	  "areaCodes": [604, 778],
	  "population": 2463000,
	  "location": {
	  	"province": "British Columbia",
	  	"country": "Canada",
	  	"continent": "North America"
	  },
	  "weedLegal": true
     )";

    //good string
	std::cout << "\n\ngood_string: " << good_string << "\n";
	std::cout << "making good_string into a json obj... " << "\n";
	json good_obj = jsonConvert::string_to_json(good_string);
	assert(good_obj != nullptr);
	std::cout << "Success! good_string --> good_obj" << "\n";

	std::string tmp_str;
	std::cout << "\n \ntesting getting json values" << "\n";
	std::cout << "getting name (str)..." << "\n";
	tmp_str = good_obj["name"].get<std::string>();
	assert(typeid(tmp_str) == typeid(std::string));
	std::cout << "Success! good_obj name is: \t" << tmp_str << "\n";

	int tmp_int;
	std::cout << "\ngetting population (int)..." << "\n";
	tmp_int = good_obj["population"].get<int>();
	assert(typeid(tmp_int) == typeid(int));
	std::cout << "Success! good_obj population is: \t" << tmp_int << "\n";

	bool tmp_bool;
	std::cout << "\ngetting weedLegal (bool)..." << "\n";
	tmp_bool = good_obj["weedLegal"].get<bool>();
	assert(typeid(tmp_bool) == typeid(bool));
	std::cout << "Success! good_obj weedLegal is: \t" << tmp_bool << "\n";

	std::cout << "\n\nconverting good_obj back to string..." << "\n";
	std::cout << "printing s2 = json_to_string(good_obj)..." << "\n";
	std::string s2 = jsonConvert::json_to_string(good_obj);
	if(s2 != "null"){
		std::cout << "Success! s2 converted back into json object\n";
	} else {
		std::cout << "Failed! good_obj failed to convert to string\n";
	}


	//bad string
	std::cout << "\n\nmaking bad_string into a json obj... \nexpecting errors: \n";
	json bad_obj = jsonConvert::string_to_json(bad_string);
	std::cout << "bad_string --> bad_obj" << "\n";
	if(bad_obj == nullptr){
		std::cout<< "Success! bad string turned into null json object\n";
	} else {
		std::cout<< "Failed to convert null bad_obj\n";
	}

	std::cout << "\n\nconverting bad_obj back to string...\n";
	std::string s3 = jsonConvert::json_to_string(bad_obj);
	if(s3 == "null"){
		std::cout << "Success! null json bad_obj converted into s3 = \"null\" \n";
	} else {
		std::cout << "Failed! bad_obj failed to convert to string\n";
	}

	//user input string --> json
	std::cout << "\n\n testing user input --> json:\n";
	json user_obj = jsonConvert::input_to_json();
	if(user_obj == nullptr){
		std::cout<< "Bad input, user_obj is null...\n";
	} else {
		std::cout<< "user_obj is valid...\n";
	}


	std::string key;
	std::cout << "\n\ntest if json object works...\n";
	std::cout << "Give key to find in json: \n";
	std::cin >> key;
	if(user_obj == nullptr){
		auto value = user_obj[key].dump();
		assert(value == "null");
		std::cout << "user_obj is null --> value is null \n";
	} else {
		auto value = user_obj[key].dump();
		assert(value != "null");
		std::cout << "The value at " << key << " is: " << value << "\n";
	}

	std::cout << "\n\nEND\n";

	return 0;
}