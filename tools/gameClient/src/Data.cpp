#include "Data.h"

Data::Data() {
    
}

Data::Data(DataCategory category, std::string_view data)
    : category{category}, data{data}
{
    
}

// Parse the received message and set fields.
Data::Data(std::string message) {
    
}

Data::~Data() {
    
}

// Serializes the data to send over the network. May change to JSON later.
std::string Data::dataToString() {
    std::stringstream stream;
    stream << static_cast<int>(category);
    stream << " " << data;
    return stream.str();
}

DataCategory Data::getCategory() const {
    return category;
}

std::string Data::getData() const {
    return data;
}