#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

namespace Utils {
    /** Generates a random string. */
    std::string generateSessionId(std::size_t length);

    /** Loads the contents of a file into a string_view */
    std::string loadFile(const std::string& filename);

    /** Lists the files inside of a directory non-recursively. An empty vector is returned if no such directory exists. */
    std::vector<std::string> listFiles(const std::string& directory);

    /** Retrieves the name of a game from a file that defines one. */
    std::string getGameName(const std::string& filename);

    /**
    Takes a string input and converts it into a json object consisting of 2 fields.
    The first field is the command, the second field is any extra arguments for the command.
    If there is no valid command, the input is assumed to be a chat message.
    **/
    json makeJsonCommand(const std::string& input);
    
    std::string removeTrailingWhitespace(const std::string& text);
    
    /**
    Takes a string and a second string to split the first string on.
    Splits the first string and returns a list of all the new substrings.
    The substrings will include the next split string if includeSplit is true.
    **/
    std::vector<std::string> splitString(const std::string& text, const std::string& splitOn, bool includeSplit);
}

#endif
