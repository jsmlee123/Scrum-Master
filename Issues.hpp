#ifndef ISSUES_H
#define ISSUES_H

#include <string>
#include <vector>
#include <set>
#include <string>
#include "User.hpp"

//class used to represent an issue
class Issues {
  private:
    std::string name;
    long id;
    long deadline;
    int priority;
    User *assignee;
    User *reporter;
    std::string type;
    std::string status;
    std::string date;
    std::string description;
    std::string comments;
  public:
    //no need for a destructor since we don't want to be deleting users, we'll delete them at program end
    //deleting the users here would lead to a massive headache

    //constructor for initial creation
    Issues(std::string n, long i, long d, int p, User * a,User* r,std::string t, 
      std::string dat,std::string desc);

    //constructor to build entire issue from scratch
    Issues(std::string n, long i, long d, int p, User * a,User* r,std::string t, 
      std::string dat,std::string desc,std::string comm,std::string status);

    //display
    //cout information in a neat format 
    void display();

    //get a string representation of Issues
    std::string toString();


    //getters
    std::string get_name() {return name;}
    long get_id() {return id;}
    long get_deadline() {return deadline;}
    int get_priority() {return priority;}
    User* get_assignee() { return assignee;}
    User* get_reporter() { return reporter;}
    std::string get_type() {return name;}
    std::string  get_status() {return status;}
    std::string get_date() {return date;}
    std::string get_description() {return description;}
    std::string get_comments(){return comments;}

    //some setters
    void set_notstarted() {status = "Not Started";}
    void set_done() {status = "Finished";}
    void set_inprogress() {status = "In progress";}
    void set_deadline(long sum) {deadline = sum;}
    void set_assignee(User* newuser) {assignee = newuser;}

    //setter that allows user input to modify the object
    void modify_issue(User *curr, std::vector<User*> *users, std::set<long> *ids);
 

  
};

#endif