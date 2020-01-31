#include <iostream>
#include <string>
#include <typeinfo>

#include "json.hpp"

using json = nlohmann::json; 


std::string json_to_string(json& obj){
	std::string res = obj.dump(2);
	if(res == "null") {
		std::cout << "json_to_string: Empty json object entered \n";
	}
	return res;
}

json string_to_json(std::string& str){
	json res;	//default as null
	try{
		res = json::parse(str);
		return res;
	} catch(std::exception const & e){
		std::cout << "Invalid json string entered\n";
		std::cout << "Exception message from json::parse: " << e.what() << "\n";
		return res;
	}
	
}

int main(int argc, char const *argv[])
{
	/* code */
	json j_object = {{"one", 1}, {"two", 2}};

/*
* GOOD STRING
*/
	// std::string s2 = R"(
 //     {
 //         "Image": {
 //             "Width":  800,
 //             "Height": 600,
 //             "Title":  "View from 15th Floor",
 //             "Thumbnail": {
 //                 "Url":    "http://www.example.com/image/481989943",
 //                 "Height": 125,
 //                 "Width":  100
 //             },
 //             "Animated" : false,
 //             "IDs": [116, 943, 234, 38793]

 // 			    }
 // 			}

 //     )";

/*
* BAD STRING
*/
     	std::string s2 = R"(
     {
         "Image": {
             "Width":  800,
             "Height": 600,
             "Title":  "View from 15th Floor",
             "Thumbnail": {
                 "Url":    "http://www.example.com/image/481989943",
                 "Height": 125,
                 "Width":  100
             },
             "Animated" : false,
             "IDs": [116, 943, 234, 38793]

     )";

	std::string s = json_to_string(j_object);

	std::cout << "string is: " << s << "\n";

	std::cout << "s2 before: " << s2 << "\n";

	std::cout << "making s2 a json obj... " << "\n";

	json obj = string_to_json(s2);

	std::cout << "s2 now json object obj " << "\n";

	// std::cout << "getting image width (int)..." << "\n";

	// auto tmp = obj["Image"]["Width"].get<int>();

	// std::cout << "image width is:" << tmp << "\n";

	std::cout << "printing s3 = json_to_string(obj)..." << "\n";

	std::string s3 = json_to_string(obj);

	std::cout << "s3 (should be same as s2) is: \n" << s3 << "\n";

	std::string user_input;

	std::cout << "\n\n Give string to use as json: \n";

	std::getline(std::cin, user_input);

	auto obj2 = string_to_json(user_input);

	std::string key;

	std::cout << "\n\n Give key to find in json: \n";

	std::cin >> key;

	auto value = obj2[key].dump();

	std::cout << "The value at " << key << " is: " << value << "\n";

	std::string s4 = json_to_string(obj2);

	std::cout << "s4 is: \n" << s4 << "\n";



	return 0;
}
