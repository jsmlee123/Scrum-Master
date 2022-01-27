#ifndef OWNER_H
#define OWNER_H

#include "User.hpp"

//represents an owner 
//is also a user object
class Owner: public User {
  public:
    ~Owner() {}
    //delegate to super constructor
    Owner(std::string n) : User(n) {};
    //is an owner yes
    bool isOwner() override {return true;}
    //is a manager yes
    bool isManager() override {return true;}
};

#endif