#include "Utils.h"

#include <fstream>
#include <sstream>
#include <random>
#include <stdexcept>
#include <boost/filesystem.hpp>

#include "json.hpp"

namespace Utils {

    std::string generateSessionId (std::size_t length) {
        // Random string generator of size length
        // https://inversepalindrome.com/blog/how-to-create-a-random-string-in-cpp
        const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        std::random_device random_device;
        std::mt19937 generator(random_device());
        std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);
        std::string random_string;
        for (std::size_t i = 0; i < length; ++i) {
            random_string += CHARACTERS[distribution(generator)];
        }
        return random_string;
    }

    std::string loadFile(const std::string& filename) {
        std::ifstream fileStream;
        fileStream.open(filename);

        if (!fileStream.good()) {
            throw std::runtime_error("File stream for " + filename + " has encountered an error!");
        }

        std::stringstream contents{};
        contents << fileStream.rdbuf();
        return contents.str();
    }

    std::vector<std::string> listFiles(const std::string& directory) {
        // https://gist.github.com/vivithemage/9517678#gistcomment-2316153
        std::vector<std::string> files{};
        if (boost::filesystem::exists(directory)) {
            for (const auto& file : boost::filesystem::directory_iterator(directory)) {
                files.push_back(file.path().generic_string());
            }
        }
        return files;
    }

    std::string getGameName(const std::string& filename) {
        std::string filedata = loadFile(filename);

        using json = nlohmann::json;

        json jsonObj;
        json configuration;
        std::string gameName;
        try {
            jsonObj = json::parse(filedata);
            configuration = jsonObj["configuration"];
            gameName = configuration["name"].get<std::string>();
        }
        catch (json::parse_error& e) {
            throw std::runtime_error("JSON Parse error");
        }
        catch (json::type_error& e) {
            throw std::runtime_error("JSON Type error");
        }

        return gameName;
    }

    json makeJsonCommand(const std::string& input) {
        vector<std::string> possibleCommands;
        possibleCommands.push_back("!createsession");
        possibleCommands.push_back("!joinsession");
        possibleCommands.push_back("!leavesession");
        possibleCommands.push_back("!gameinput");
        possibleCommands.push_back("!whisper");
        possibleCommands.push_back("!requestgames");
        ossibleCommands.push_back("!chat");

        std::stringstream commandStream;
        commandStream << "{ \"command\": \""; // Start the json object and declare the command field

        size_t endOfCommand = input.find(" ");
        std::string firstWord = input.substr(0,endOfCommand);
        if ( std::find(possibleCommands.begin(), possibleCommands.end(), firstWord) != possibleCommands.end() ) {
            commandStream << firstWord;
        } else {
            commandStream << "!chat";
            endOfCommand = 0;
        }

        commandStream << "\", \"data\": \""; // Declare the data field
        commandStream << input.substr(endOfCommand, string::npos); // Get the data value
        commandStream << "\" }"; // End of the json object

        json message = json::parse( commandStream.str() );

        return message;
    }

}
