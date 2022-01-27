#ifndef MANAGER_H
#define MANAGER_H

#include "User.hpp"

//class the represents a manager
//is also a user object
class Manager: public User {
  public:
    ~Manager() {}
    //just delegates to User constructor since it doesnt have its own fields
    Manager(std::string n):User(n) {};
    //has owner permissions no
    bool isOwner() override { return false;}
    //has manager permissions yes
    bool isManager() override { return true;}

};

#endif