#ifndef DATA_H
#define DATA

#include <sstream>
#include <string>

enum DataCategory {
    UNKNOWN, // Default state is error
    GAME_END, GAME_BEGIN, GAME_DATA, OTHER
};

class Data {
private:
    DataCategory category;
    std::string data;

public:
    Data();
    Data(DataCategory category, std::string_view data);
    Data(std::string message);
    ~Data();
    std::string dataToString();
    
    DataCategory getCategory() const;
    std::string getData() const;
};

#endif