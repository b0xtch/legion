#ifndef USER_H
#define USER_H

#include <string>

#include "Server.h"

class User {
public:
    User(networking::Connection cnxn);
    
    networking::Connection getConn() const;
    std::string_view getName() const;
    void setName(std::string_view& name);
private:
    networking::Connection cnxn;
    
    std::string_view name;
};

#endif