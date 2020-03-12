#ifndef USER_H
#define USER_H

#include <string>
#include "Server.h"

class User {
public:
    User(const networking::Connection& connection);

    networking::Connection getConnection() const;

    std::string getName() const;

    void setName(std::string& name);

    bool operator< (const User &user) const;

private:
    networking::Connection connection;
    
    std::string name;
};

#endif
