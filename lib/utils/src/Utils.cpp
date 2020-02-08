#include "Utils.h"

#include <random>

std::string Utils::generateSessionId(std::size_t length){
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
