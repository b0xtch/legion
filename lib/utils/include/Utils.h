#ifndef UTIL_H
#define UTIL_H

#include <string>

namespace Utils {
    /** Generates a random string. */
    std::string generateSessionId(std::size_t length);

    /** Loads the contents of a file into a string_view */
    std::string loadFile(const std::string& filename);
}

#endif
