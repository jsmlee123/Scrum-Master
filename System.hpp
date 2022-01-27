#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>

//containers
#include <vector>
#include <queue>
#include <set>

#include <string>

#include "User.hpp"
#include "Issues.hpp"
#include "Tasks.hpp"

//system that runs the program
class System {
  private:
    std::string project_name; //name of project
    long deadline; //deadline in days

    long nextID; //next ID to be used for the next issue

    std::vector<User*> *users; // keep track of users in this project
    User *current_user; //user currently using software

    Tasks *todo; // to do list
    std::vector<Tasks*> *sprints; //sprints we need to do
    Tasks *done;// work done list

    std::set<long> *usedIDs;

    std::queue<Issues*> *rollover; // what we need to rollover to the next sprint
    std::queue<Tasks*> *active_sprints; // all sprints that need to be done given how many user wants

    

    //project detail manipulation
    void project_detail_menu(); //show menu
    void set_deadline(); //change the deadline to whatever
    void change_project_name(); //change the name of the project ot whatever

    //user manipulation
    void user_detail_menu(); //show user menu
    void switch_user(); //switch user to stated user
    void add_user(); //add user, system prompts for information
    void remove_user(); //remove user, system prompts for information
    User *find_user(std::string name); //find the specified user given their name and return them

    //issue manipulation
    void issue_menu(); //show the issue related stuff menu
    void remove_issue(); //remove the issue from the program
    void add_issue(); //add the issue to the program
    void update_issue(); //this will delegate to user class probably
    void move_issue(); //ask what the user wishes to move an issue
    void assign_to_self(); //assign an issue to current user
    void move_to_sprint(); //move highest priority item to sprint
    void sprint_move_help(Issues *is); //helper to help move an issue to sprint

    //sprint specific operations
    void run_sprint(); // run sprint on sprint at the front of the queue
    void roll_over(); //rollover anything leftover from a sprint
    void decrement_issues(long sum); //decrement issues inprogress or not done by specified num

    //display information 
    void display_menu(); //display menu
    void display_users(); //display users and their member type
    void display_project_info(); //display information about the project
    void display_todo(); //display relevant issue information in todo
    void display_sprints(); //display relevant issue information in the sprints
    void display_done(); //display relevant issue information in done
    void display_issue_detailed(); //display issue information for specific issue

    //state perseverence information
    void write_data(); //write the the current state to a data.txt file
    void read_data(); //read data from a data.txt file

  public:
    System(); //only empty constructor atm
    
    
    //init the system. Make new project or load past one
    void init_system();
    
    //init the program
    void go();

    //rule of 5 members
    //only in this class since this is the only class that really manages resources
    //and is responsible for deleting them
    ~System(); //destructor
    System(const System& other); // copy constructor
    System(System&& other); //move constructor
    System& operator=(const System& other); //copy assignment
    System& operator=(System&& other); // move assignment

};

#endif