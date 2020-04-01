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

    namespace PMConstants {
        const std::string KEY_COMMAND = "command";
        const std::string KEY_DATA = "data";

        const std::string TYPE_SERVER_STOP = "serverstop";

        const std::string TYPE_CREATE_SESSION = "!createsession";
        const std::string TYPE_JOIN_SESSION = "!joinsession";
        const std::string TYPE_LEAVE_SERVER = "!leaveserver";
        const std::string TYPE_LEAVE_SESSION = "!leavesession";
        const std::string TYPE_CHAT = "!chat";
        const std::string TYPE_WHISPER = "!whisper";
        const std::string TYPE_REQUEST_GAMES = "!requestgames";
        const std::string TYPE_GAME_INPUT = "!gameinput";

    };

    const std::vector<const std::string> possibleCommands;
    possibleCommands.push_back(TYPE_CREATE_SESSION);
    possibleCommands.push_back(TYPE_JOIN_SESSION);
    possibleCommands.push_back(TYPE_LEAVE_SERVER);
    possibleCommands.push_back(TYPE_LEAVE_SESSION);
    possibleCommands.push_back(TYPE_CHAT);
    possibleCommands.push_back(TYPE_WHISPER);
    possibleCommands.push_back(TYPE_REQUEST_GAMES);
    possibleCommands.push_back(TYPE_GAME_INPUT);

    json makeJsonCommand(const std::string& input) {
        std::stringstream commandStream;
        commandStream << "{ \"" << KEY_COMMAND << "\": \""; // Start the json object and declare the command field

        size_t endOfCommand = input.find(" ");
        std::string firstWord = input.substr(0,endOfCommand);
        if ( std::find(possibleCommands.begin(), possibleCommands.end(), firstWord) != possibleCommands.end() ) {
            commandStream << firstWord;
        } else {
            commandStream << commandList[0];
            endOfCommand = 0;
        }

        commandStream << "\", \"" << KEY_DATA << "\": \""; // Declare the data field
        commandStream << input.substr(endOfCommand, string::npos); // Get the data value
        commandStream << "\" }"; // End of the json object

        json message = json::parse( commandStream.str() );

        return message;
    }

}
