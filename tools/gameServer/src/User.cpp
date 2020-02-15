#include "User.h"

User::User(networking::Connection cnxn) :
    cnxn{cnxn}, name{""}
{
    
}

networking::Connection User::getConn() const {
    return cnxn;
}

std::string_view User::getName() const {
    return name;
}

void User::setName(std::string_view& name) {
    this->name = name;
}