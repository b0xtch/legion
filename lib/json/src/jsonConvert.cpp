#include <iostream>
#include <string>
#include <typeinfo>

#include "json.hpp"

using json = nlohmann::json; 


std::string json_to_string(json obj){
	return obj.dump();
}

json string_to_json(std::string str){
	json res;
	try{
		res = json::parse(str);
		return res;
	} catch(std::exception const & ex){
		std::cout << "Invalid json entered \n";
		json obj_null = {}; 
		return obj_null;
	}
	
}

int main(int argc, char const *argv[])
{
	/* code */
	json j_object = {{"one", 1}, {"two", 2}};

/*
* GOOD STRING
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

 			    }
 			}

     )";

/*
* BAD STRING
*/
     // 	std::string s2 = R"(
     // {
     //     "Image": {
     //         "Width":  800,
     //         "Height": 600,
     //         "Title":  "View from 15th Floor",
     //         "Thumbnail": {
     //             "Url":    "http://www.example.com/image/481989943",
     //             "Height": 125,
     //             "Width":  100
     //         },
     //         "Animated" : false,
     //         "IDs": [116, 943, 234, 38793]

     // )";

	std::string s = json_to_string(j_object);

	std::cout << "string is: " << s << "\n";

	std::cout << "s2 before: " << s2 << "\n";

	std::cout << "making s2 a json obj... " << "\n";

	json obj = string_to_json(s2);

	std::cout << "s2 now json object obj " << "\n";

	std::cout << "getting image width (int)..." << "\n";

	auto tmp = obj["Image"]["Width"].get<int>();

	std::cout << "image width is:" << tmp << "\n";

	std::cout << "printing s3 = json_to_string(obj)..." << "\n";

	std::string s3 = json_to_string(obj);

	std::cout << "s3 (should be same as s2) is: ... \n" << s3 << "\n";



	return 0;
}
