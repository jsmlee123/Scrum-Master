#ifndef MEMBER_H
#define MEMBER_H

#include "User.hpp"

//represents a team member
//is also a user object
class Member :public User{
  public:
    ~Member() {}
    //constructor delegates t super
    Member(std::string n):User(n) {};
    //is an owner no
    bool isOwner() override {return false;}
    //is a manager no
    bool isManager() override{return false;}
};

#endif