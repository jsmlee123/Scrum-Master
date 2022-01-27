#ifndef TASKS_H
#define TASKS_H

#include <vector>
#include <string>
#include "Issues.hpp"
#include "User.hpp"

//class used to represent a pile, ex todo, sprint, done
class Tasks {
  private:
    //the issues stored in this pile
    std::vector<Issues*> *issues;
  public:
    //destructor needed to get rid of the vector
    ~Tasks() {delete issues;}
    //empty constructor
    Tasks() {issues = new std::vector<Issues*>();}
    
    //display all the issues in the task in a neat format
    void display(User *curr);

    //get a string representation of all task/issues in the task
    std::string toString();

    //set all the issues in this task to done
    void set_done();

    //decrement all the deadlines in the issues by 1
    void decrement_issues(long sum);

    //get a list of the issues in this task
    std::vector<Issues*> *get_issues() {return issues;}
    
    
    void add_issue(Issues *new_issue){issues->push_back(new_issue);} //add issue to the task
    Issues *remove_issue(long id); //remove issue given its id from task
    Issues *find_issue(long id); //find issue given its id in the task
    Issues *find_highest_priority(); //find the highest priority issue in the stak

    void assign_sprints(std::vector<User*> *users); //given user input, assign an issue to a user

    //this is needed since we only want to do this at program end
    void delete_issues(); //delete the the actual issues in the task
     
    
};

#endif