#include "User.h"

User::User(const networking::Connection& connection) :
    connection{connection}, name{""}
{
    
}

networking::Connection User::getConnection() const {
    return connection;
}

std::string User::getName() const {
    return name;
}

void User::setName(std::string& name) {
    this->name = name;
}
