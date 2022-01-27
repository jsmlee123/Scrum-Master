#ifndef USER_H
#define USER_H

#include <string>

//class to represent a user in our program
class User {
  private:
    std::string name;
    //password doesn't seem to be necessary
  public:
    virtual ~User() {};
    //only field is a name
    User(std::string n) {name = n;}
    //only getter, gets the users name
    std::string get_name() {return name;}
    virtual bool isOwner() = 0; //for owner permissions
    virtual bool isManager() = 0; //for manager permissions

    //no setters! 
    //user should not be able to change their name, they can just make a new user
};

#endif