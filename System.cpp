#include "System.hpp"

#include <iostream>
#include <vector>
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>       
#include <set>
#include <unistd.h>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <utility>

#include "Owner.hpp"
#include "Manager.hpp"
#include "Member.hpp"
#include "Issues.hpp"
#include "Tasks.hpp"

using namespace std;

/////////////////////////////////System Creation/Deletion/////////////////////////////////////////////
//default constructor
//this is how we init our program state
System::System() {
  project_name = "";
  deadline = 0;
  nextID = 1;
  current_user = nullptr;
  users = new vector<User*>();
  usedIDs = new set<long>();
  todo = new Tasks();
  sprints = new vector<Tasks*>();
  done = new Tasks();

  rollover = new queue<Issues*>();
  active_sprints = new queue<Tasks*>();
}

//destructor for our System
System::~System() {
  //delete all existing users
  for(int i = 0; i < users->size(); i++) {
    delete (*users)[i];
  }
  delete users;

  //delete set of usedIDs
  delete usedIDs;

  //delete todo and any issues in it
  todo->delete_issues();
  delete todo;

  
  //delete each sprint in the system and any of its issues
  for(int i = 0; i < sprints->size(); i++) {
    (*sprints)[i]->delete_issues();
    delete (*sprints)[i];
  }
  delete sprints;
  

  //delete done and any issues in it
  done->delete_issues();
  delete done;

  //delete anything in rollover
  while(!rollover->empty()) {
    Issues *temp = rollover->front();
    rollover->pop();
    delete temp;
  } 
  delete rollover;

  //delete anything left in active sprints
  while(!active_sprints->empty()) {
    Tasks *temp = active_sprints->front();
    active_sprints->pop();
    temp->delete_issues();
    delete temp;
  } 
  delete active_sprints;

}

//////////////////////Run Operations//////////////////////////////
//init system with user prompt
void System::init_system() {
  string choice;
  //load previous session or make new project
  cout << "Hello! Choose (1) for loading previous project or (2) to start a new one" << endl;

  cin >> choice;
  while(choice != "1" && choice != "2") {
    cin.clear();
    cin.ignore(10000,'\n');
    cout << "Please choose 1 or 2!" << endl;
    cin >> choice;
  }
  //load previous
  if(choice == "1") {
    read_data();
    write_data();
  } 
  else {
    //make new
    string projectName;
    cout << "Choose a Project Name!" << endl;
    cin.ignore(10000,'\n');
    getline(cin,projectName);
    project_name = projectName;
    //user sets a deadline
    long dl;
    cout <<"Set a Deadline(days)!" << endl;
    cin >> dl;
    while(cin.fail()) {
      cin.clear();
      cin.ignore(10000,'\n');
      cout << "Please put an integer number!" << endl;
      cin >> dl;
    }
    deadline = dl;
    //get the name of the person creating the project
    string name;
    cout << "Who is the project owner?" << endl;
    cin.ignore(10000,'\n');
    getline(cin,name);

    User *owner = new Owner(name);
    current_user = owner;
    users->push_back(owner);
    cout << "Project Created!" << endl;
    //write this data to save file
    write_data();
  }
}

//run the program
void System::go() {
  //init system first
  init_system();
  string buf;
  //loop endlessly
  while(1) {
    cout << endl;
    if(current_user != nullptr) { //there may be no current user if we remove the user who is current
      cout << "Current User: " << current_user->get_name() << endl;
    } else {
      cout << "Current User: None" << endl;
    }
    
    
    cout << endl;
    //main menu
    cout << "Menu:" <<endl;
    cout << "(1)Change Project Details" << endl;
    cout << "(2)Add/Remove/Update users" << endl;
    cout << "(3)Display" << endl;
    cout << "(4)Todo/Sprint/Work Done" << endl;
    cout << "(5)Begin Sprint" << endl;
    cout << "(6)Exit Program" << endl;

    string choice;
    cin >> choice; //make sure choice is within range
    while(choice != "1" && choice != "2" 
    && choice != "3" && choice != "4" 
    && choice != "5" &&  choice != "6") {
      cin.clear();
      cin.ignore(10000,'\n');
      cout << "Please choose a valid option!" << endl;
      cin >> choice;
    }

    //delegate to helpers for menu operations
    if(choice == "1" ) {
      project_detail_menu();
    } else if(choice == "2") {
      user_detail_menu();
    } else if (choice == "3") {
      display_menu();
    } else if (choice == "4") {
      issue_menu();
    } else if(choice == "5") {
      run_sprint();
    } else if(choice == "6") {
      break;
    }

    //save data after each loop
    write_data();
  }
  cout << "Program Ended!" << endl;
}

/////////////////////////////Project Details////////////////////////
//change project level details
void System::project_detail_menu() {
  //make sure if no user is selected or they are not the owner they cant change project level details
  if(current_user == nullptr ||!current_user->isOwner()) {
    cout << "User does not have proper permission!" << endl;
    return;
  }

  string choice;
  cout << "(1)Change Project Name" << endl;
  cout << "(2)Change Project Deadline" << endl;
  cout << "(3)Return to main menu" << endl;
  cin >> choice;
  while(choice != "1" && choice != "2" && choice != "3") {
    cin.clear();
    cin.ignore(10000,'\n');
    cout << "Please choose a valid option!" << endl;
    cin >> choice;
  }

  //delegate to helpers
  if(choice == "1") {
    change_project_name(); //change name
  } else if (choice == "2"){
    set_deadline(); //change deadline
  } else {
    return;
  }
}

//set deadline to user input
void System::set_deadline() {
  long newdl;
  cout << "Choose a new deadline for the project(days)" << endl;
  cin >> newdl;
  while(cin.fail()) {
      cin.clear();
      cin.ignore(10000,'\n');
      cout << "Please put an integer number!" << endl;
      cin >> newdl;
  }
  deadline = newdl;
}

//set project name to user input
void System::change_project_name() {
  string newname;
  cout << "Choose a new name for the project" << endl;
  cin.clear();
  cin.ignore(10000,'\n');
  getline(cin,newname);
  project_name = newname;
}


///////////////////////User Manipulation//////////////////////////
void System::user_detail_menu() {
  string choice;
  cout << "(1)Switch Current User" << endl;
  cout << "(2)Add User" << endl;
  cout << "(3)Remove User" << endl;
  cout << "(4)Return to main menu" << endl;
  cin >> choice;
  while(choice != "1" && choice != "2" && choice != "3" && choice !="4") {
    cin.clear();
    cin.ignore(10000,'\n');
    cout << "Please choose a valid option!" << endl;
    cin >> choice;
  }

  if(choice == "1") {
    switch_user();
  } else if (choice == "2"){  
    add_user();
  } else if (choice == "3") {
    remove_user();
  } else {
    return;
  }
}

//helper to find a user in the system
User *System::find_user(std::string name) {
  for(int i = 0; i < users->size(); i++) {
    if((*users)[i]->get_name() == name) {
      //current_user = (*users)[i];
      return (*users)[i];
    }
  }
  //cout << "User does not exist in database" << endl;
  return nullptr;
}


//switch to specified user
void System::switch_user() {
  //anyone can switch even if current user is null
  //or else we get locked out of the project
  string name;
  cout << "Switch to who?" << endl;
  cin.clear();
  cin.ignore(10000,'\n');
  getline(cin,name);

  //ensure user exists in the system
  if(find_user(name) != nullptr) {
    cout <<"User found in database!" << endl;
    current_user = find_user(name);
  } else {
    cout << "User does not exist in database" << endl;
  }
  
}

//add user
//only manager+ users can add others to the project
//make sure that the new user has a unique username
void System::add_user() {
  if(current_user == nullptr || !current_user->isManager()) {
    cout << "User does not have proper permission!" << endl;
    return;
  }
  string name;
  cout << "Name of new user: " << endl;
  cin.clear();
  cin.ignore(10000,'\n');
  getline(cin,name);
  if(find_user(name) != nullptr) {
    cout <<"User already exists!" << endl;
  } else {
    cout << "Choose Employee Type!" << endl;
    string choice;
    cout << "(1)Member" << endl;
    cout << "(2)Manager" << endl;
    //only an owner can see this option
    //as managers can't assign owners
    if(current_user->isOwner()) {
      cout << "(3)Owner" << endl;
    }
    
    cin >> choice;
    while(choice != "1" && choice != "2" && choice != "3") {
      cin.clear();
      cin.ignore(10000,'\n');
      cout << "Please choose a valid option!" << endl;
      cin >> choice;
    }

    //create different user objects depending on choice
    User *newuser;
    if(choice == "1") {
      newuser = new Member(name);
    } else if(choice =="2") {
      newuser = new Manager(name);
    } else if(current_user->isOwner() && choice == "3"){
      newuser = new Owner(name);
    } else {
      //if user chooses 3 and they are not an owner they cannot make another owner
      cout << "Manager can't make user an owner!" << endl;
      return;
    }
    //add new user to our list of users
    users->push_back(newuser);
  }
}


//remove user with specified user name if they exist in our system
void System::remove_user() {
  //only manager+ users can remove others
  if(current_user == nullptr || !current_user->isManager()) {
    cout << "User does not have proper permission!" << endl;
    return;
  }

  string name;
  cout<< "Name of user: " << endl;
  cin >> name;

  //if find_user returns nullptr, means user did not exist
  if(find_user(name) == nullptr) {
    cout << "User does not exist in database" << endl;
    return;
  }

  for(int i = 0; i <users->size(); i++) {
    if((*users)[i]->get_name() == name) {
      //manager cant remove owners, only other owners can
      if(!current_user->isOwner() && (*users)[i]->isOwner()) {
        cout <<"Managers Cannot Remove Owners!" << endl;
        return;
      }

      //if current was that person, set the current to null
      if(current_user->get_name() == name) {
        current_user = nullptr;
      }
      //delete the pointer
      delete (*users)[i];
      //remove pointer from user list
      users->erase(users->begin() + i);
      cout << name << " was removed" << endl;
    }
  }
}

/////////////////////////Issue Manipulation////////////////////////

//displays menu for issue manipulation
void System::issue_menu() {
  string choice;
  cout << "(1)Add new issue to Todo" << endl;
  cout << "(2)Remove Issue" << endl;
  cout << "(3)Move issue" << endl; //only manager+ allowed to do this
  cout << "(4)Update issue property" << endl;
  cout << "(5)Assign issue to self" << endl;
  cout << "(6)Move highest priority issue to a sprint" << endl;
  cout << "(7)Return to main menu" << endl;

  cin >> choice;
  while(choice != "1" && choice != "2" && choice != "3" 
  && choice !="4" && choice !="5" && choice !="6" && choice != "7") {
    cin.clear();
    cin.ignore(10000,'\n');
    cout << "Please choose a valid option!" << endl;
    cin >> choice;
  }

  //delegate to helpers
  if(choice == "1") {
    add_issue();
  } else if(choice == "2" ) {
    remove_issue();
  } else if(choice == "3") {
    move_issue();
  } else if(choice == "4") {
    update_issue();
  }else if(choice == "5"){
    assign_to_self();
  } else if(choice == "6"){
    move_to_sprint();
  }else{
    return;
  }
}

//move highest priority item to sprint
void System::move_to_sprint() {
  Issues *iss = todo->find_highest_priority();
  if(iss == nullptr) {
    cout << "No Issues in ToDo!" << endl;
    return;
  }

  //query which sprint to move to
  iss->set_inprogress();
  sprint_move_help(iss);
}

//add issue to todo pile
void System::add_issue() {
  string n,t,a,dat,desc;
  long d;
  int p;
  User *assignee;

  cin.clear();
  cin.ignore(10000,'\n');
  //query for information necessary to make an issue
  cout <<"What is the name of the issue?" << endl;
  getline(cin,n);

  cout << "Issue Deadline(days)?" << endl;
  cin >> d;
  while(cin.fail()) {
      cin.clear();
      cin.ignore(10000,'\n');
      cout << "Please put an integer number!" << endl;
      cin >> d;
  }
  
  cout << "Priority?(lower integer indicates higher priority)" << endl;
  cin >> p;
  while(cin.fail()) {
      cin.clear();
      cin.ignore(10000,'\n');
      cout << "Please put an integer number!" << endl;
      cin >> p;
  }

  //assumes reporter is current user
  //and there is no assignee yet

  cin.clear();
  cin.ignore(10000,'\n');

  cout << "Issue Type?" << endl;
  getline(cin,t);

  cout <<"Today's Date?" << endl;
  getline(cin,dat);

  cout << "Description of issue?" << endl;
  getline(cin,desc);

  long randID = nextID; //issue number, make sure its not already used 
  while(usedIDs->find(randID) != usedIDs->end()) {
    nextID++;
    randID = nextID;
  }
  nextID++; // increment by 1 to get the next id number
  usedIDs->insert(randID);
  
  //create the new issue
  Issues *new_issue = new Issues(n,randID,d,p,nullptr,current_user,t,dat,desc);
  cout << "Issue created with ID" << randID << endl;

  //add it to todo
  todo->add_issue(new_issue);
}


//remove specified issue in specified pile
void System::remove_issue() {
  //only manager+ people have power to remove issues
  if(current_user == nullptr || !current_user->isManager()) {
    cout << "User does not have permission!" << endl;
    return;
  }

  string choice;
  cout << "Remove from where?" << endl;
  cout << "(1)To Do" << endl;
  cout << "(2)Sprints" << endl;
  cout << "(3)Work Done" << endl;
  cout << "(4)Return to menu" << endl;
  cin >> choice;
  while(choice != "1" && choice != "2" && choice != "3" && choice !="4") {
    cin.clear();
    cin.ignore(10000,'\n');
    cout << "Please choose a valid option!" << endl;
    cin >> choice;
  }
  //return to menu if user choese 4
  if(choice == "4") {
    return;
  }


  //query for id identifier because issues can have the same name
  long id;
  cout << "Issue ID?" << endl;
  cin >> id;


  //look through the chosen pile(s)
  Issues *is = nullptr;
  if(choice == "1") {
    is = todo->remove_issue(id);
  } else if (choice == "2" ) {
    for(int i = 0; i < sprints->size(); i++) {
      if(is == nullptr) {
        is =(*sprints)[i]->remove_issue(id);
      }
      
    }
  } else if(choice == "3" ) {
    is = done->remove_issue(id);
  }

  //if is found then it is not null
  if(is == nullptr) {
    cout << "Issue Not Found!" << endl;
  } else {
    //delete the pointer now that we want to get rid of this issue
    delete is;
    cout << "Issue Removed!" << endl;
  }
}


//move issue in specified pile to another
void System::move_issue() {
  //only manager+ users can move issues around freely
  if(current_user == nullptr || !current_user->isManager()) {
    cout << "User does not have permission!" << endl;
    return;
  }

  //query from where does the user want to move the pile
  string choice;
  cout << "Move from where?" << endl;
  cout << "(1)To Do" << endl;
  cout << "(2)Sprints" << endl;
  cout << "(3)Work Done" << endl;
  cout << "(4)Return to menu" << endl;
  cin >> choice;
  while(choice != "1" && choice != "2" && choice != "3" && choice !="4") {
    cin.clear();
    cin.ignore(10000,'\n');
    cout << "Please choose a valid option!" << endl;
    cin >> choice;
  }

  //user must now the issue id to move an issue
  long id ;
  cout << "Issue ID?" << endl;
  cin >> id;


  while(cin.fail()) {
      cin.clear();
      cin.ignore(10000,'\n');
      cout << "Please put an integer number!" << endl;
      cin >> id; 
  }

  //scan through the designated pile and check if issue is there
  Issues *is = nullptr;
  if(choice == "1") {
    is = todo->remove_issue(id);
  } else if (choice == "2" ) {
    for(int i = 0; i < sprints->size(); i++) {
      if(is == nullptr) {
        is = (*sprints)[i]->remove_issue(id);
      }
    }
  } else if(choice == "3" ) {
    is = done->remove_issue(id);
  } else {
    return;
  }
  //issue not in pile, do nothing
  if(is == nullptr) {
    cout << "Issue Not Found!" << endl;
    return;
  }

  //issue was found move to where
  string choice2;
  cout << "Move to where?" << endl;
  cout << "(1)To Do" << endl;
  cout << "(2)Sprints" << endl;
  cout << "(3)Work Done" << endl;
  
  cin >> choice2;
  while(choice2 != "1" && choice2 != "2" && choice2 != "3") {
    cin.clear();
    cin.ignore(10000,'\n');
    cout << "Please choose a valid option!" << endl;
    cin >> choice2;
  }

  if(choice2 == "1") {
    is->set_notstarted(); //set to not started. Anything else would not make sense
    todo->add_issue(is);
  } else if (choice2 == "2" ) {
    is->set_inprogress();
    sprint_move_help(is); //sprint is not just 1 pile so designate to helper on which sprint to move to
  } else if(choice2 == "3" ) {
    is->set_done();
    done->add_issue(is);
  }
  cout << "Issue Added!" << endl;
}

//help move stuff to sprints since there are several
void System::sprint_move_help(Issues *is) {
  //ask which sprint to move to
  long choice;
  cout << "Which sprint would you like to move to?" << endl;
  cin >> choice;

  while(cin.fail()) {
      cin.clear();
      cin.ignore(10000,'\n');
      cout << "Please put an integer number!" << endl;
      cin >> choice; 
  }

  //grow the sprints vector to size if not large enough
  while(sprints->size() < choice) {
    sprints->push_back(new Tasks());
  } 

  (*sprints)[choice - 1]->add_issue(is);

}

//update the given issue
void System::update_issue() {
  //query for the issue id
  long id;
  cout << "Give ID number of issue" << endl;
  cin >> id;
  while(cin.fail()) {
      cin.clear();
      cin.ignore(10000,'\n');
      cout << "Please put an integer number!" << endl;
      cin >> id; 
  }
  Issues *is = nullptr;
  
  is = todo->find_issue(id);
  if (is == nullptr ) {
    for(int i = 0; i < sprints->size(); i++) {
      if(is == nullptr) {
        is =(*sprints)[i]->find_issue(id);
      }
    }
  } 
  //only manager can access issues in work done
  if(is == nullptr && current_user->isManager()) {
    is = done->find_issue(id);
  }

  if(is == nullptr) {
    cout << "Issue does not exist or is in the work done pile" << endl;
    return;
  }
  
  //make sure user is either the assignee or a manager+
  if(current_user == nullptr || is->get_assignee() == nullptr || is->get_assignee()->get_name() != current_user->get_name()) {
    if(!current_user->isManager()) {
      cout << "User does not have permission to edit this issue" << endl;
      //cout << "User is not " << is->get_assignee()->get_name() << endl;
      return;
    }
  }

  //modify the specified issue
  //pass in current_user since different users can modify different things
  is->modify_issue(current_user,users,usedIDs); 
}

//assign issue of specified id to the current user
//a user can assign an issue to themselves if they know its id
//and location. This makes sure the user is consciously choosing
//to assign the issue to themselves
void System::assign_to_self() {
  string choice;
  //query location of issue
  cout << "Issue location?" << endl;
  cout << "(1)To Do" << endl;
  cout << "(2)Sprints" << endl;
  cin >> choice;
  while(choice != "1" && choice != "2") {
    cin.clear();
    cin.ignore(10000,'\n');
    cout << "Please choose a valid option!" << endl;
    cin >> choice;
  }

  //query id of issue
  long id;
  cout << "ID to assign to self" << endl;
  cin >> id;

  while(cin.fail()) {
      cin.clear();
      cin.ignore(10000,'\n');
      cout << "Please put an integer number!" << endl;
      cin >> id;
  }

  //find the issue in the system
  Issues *is = nullptr;
  if(choice == "1") {
    is = todo->find_issue(id);
  } else if (choice == "2" ) {
    for(int i = 0; i < sprints->size(); i++) {
      if(is == nullptr) {
        is = (*sprints)[i]->find_issue(id);
      }
    }
  } else {
    return;
  }

  //if is is null then issue wasn't found 
  if(is == nullptr) {
    cout << "Issue Not Found!" << endl;
    return;
  }

  //set user to current
  is->set_assignee(current_user);
  cout << "Issue succesfully assigned to self" << endl;
}


////////////////////////Display operations///////////////////////////////////////
void System::display_menu() {
  string choice;
  cout << "(1)Display project information" << endl;
  cout << "(2)Display Users on the project" << endl;
  cout << "(3)Display my todo issues" << endl;
  cout << "(4)Display my sprint issues" << endl;
  cout << "(5)Display my done issues" << endl;
  cout << "(6)Display specific issue details " << endl;
  cout << "(7)Return to main menu" << endl;

  cin >> choice;
  while(choice != "1" && choice != "2" && choice != "3" 
  && choice !="4" && choice != "5" && choice !="6" && choice != "7") {
    cin.clear();
    cin.ignore(10000,'\n');
    cout << "Please choose a valid option!" << endl;
    cin >> choice;
  }

  if(choice == "1") {
    display_project_info();
  } else if(choice == "2") {
    display_users();
  } else if(choice == "3") {
    display_todo();
  } else if(choice == "4") {
    display_sprints();
  } else if(choice =="5") {
    display_done();
  } else if(choice == "6") {
    display_issue_detailed();
  } else {
    return;
  }
}

//display simple project information
void System::display_project_info() {
  cout << "Project Name:" << project_name << endl;
  cout << "Deadline:" << deadline << " Days" << endl;
  return;
}

//display users on the project
void System::display_users() {
  for(int i = 0; i < users->size(); i++) {
    string membertype;
    if((*users)[i]->isOwner()) {
      membertype = "Owner/Lead";
    } else if((*users)[i]->isManager()) {
      membertype = "Manager";
    } else {
      membertype = "Member";
    }

    cout << (*users)[i]->get_name() << ", " << membertype << endl;
  }
}

//display the todo issues assigned to user
void System::display_todo() {
  todo->display(current_user);
}

//display sprint issues assigned to user
void System::display_sprints() {
  for(int i = 0; i < sprints->size(); i++) {
    cout << "Sprint #" << (i+1) << endl;
    (*sprints)[i]->display(current_user);
  }
}

//display done issues assigned to user
void System::display_done() {
  done->display(current_user);
}

//display information about specific issue user wants to see
void System::display_issue_detailed() {
  long id;
  cout << "ID of issue you wish to view" << endl;
  cin >> id;

  Issues *is = nullptr;
  //look for issue in every pile
  is = todo->find_issue(id);

  if (is == nullptr ) {
    for(int i = 0; i < sprints->size(); i++) {
      if(is == nullptr) {
        is =(*sprints)[i]->find_issue(id);
      }
    }
  } 

  if(is == nullptr ) {
    is = done->find_issue(id);
  }
  //this means the issue hasnt been created yet
  if(is == nullptr) {
    cout << "This issue does not exist" << endl;
    return;
  } 

  //make sure user is assigned to issue or is manager+
  if(is->get_assignee() != nullptr && is->get_assignee()->get_name() == current_user->get_name()) {
    is->display();
  } else {
    if(current_user->isManager()) {
      is->display();
    } else {
      cout << "User does not have permission to view issue" << endl;
    }
  }
}

///////////////////////////////////////////Sprint Operations/////////////////////////////////////////

//run sprints
void System::run_sprint() {
  if(!current_user->isManager()) { //to run a sprint user must be manager+
    cout << "User does not have permission to run sprints" << endl;
    return;
  }
  //query number sprints we can run
  long choice;
  cout << "How many sprints would you like to run?" << endl;
  cin >> choice;

  //max number of sprints we have in our sprints pile
  int maxSprints = sprints->size();

  //query length of each sprint
  int i = 0;
  vector<long> time; //keep track of how long each sprint is
  //ask for length of each sprint in 
  for(;i < maxSprints && i < choice; i++) {
    active_sprints->push((*sprints)[i]);
    sprints->erase(sprints->begin() + 0);
    long tiempo;
    cout << "How many days for sprint " << (i+1) << endl;
    cin >> tiempo;
    long sum = deadline;
    //make sure sum of sprints is less than deadline time
    while(cin.fail() || tiempo > sum) { 
      cin.clear();
      cin.ignore(10000,'\n');
      cout << "Please put an a valid time !" << endl;
      cin >> tiempo;
    }
    sum -= tiempo;
    time.push_back(tiempo);
  }

  int j = 0;
  while(!active_sprints->empty()) {
    Tasks* current_sprint = active_sprints->front();
    active_sprints->pop();
    current_sprint->assign_sprints(users); //make sure that each issue has an assignee before starting
    cout << "Sprint " << (j+1) << " starting!" << endl; 

    //simulate time[j] days, each day is 10 seconds
    sleep(10 * time[j]);

    //sprint finished
    cout << "Sprint done!" << endl;
    current_sprint->set_done(); //set all the issues to done;

    //decrement deadlines of everything since time has passed
    for(int z = 0; z <= j && !active_sprints->empty() ;z++) {
      active_sprints->front()->decrement_issues(time[z]);
    }
    decrement_issues(time[j]);
    deadline -= time[j];
    

    //add all the issues in the recently done sprint to the done pile
    vector<Issues*> * issues = current_sprint->get_issues();
    for(int k = 0; k < issues->size(); k++) {
      done->add_issue((*issues)[k]);
    }

    //After the sprint check if every issue was completed during the sprint
    //if not ask for a new deadline 
    for(int k = 0; k < issues->size(); k++) {
      string choice;
      Issues *curr = (*issues)[k];
      curr->set_deadline(curr->get_deadline() - time[j]); 
      cout << "Was issue:" << curr->get_name() << endl;
      cout << "Deadline: " << curr->get_deadline() << " days" << endl;
      cout << "ID: " << curr->get_id() << " completed?" << endl;
      cout << "(1) Yes (2) No" << endl;
      cin >> choice;
      while(choice != "1" && choice != "2") {
        cin.clear();
        cin.ignore(10000,'\n');
        cout << "Please choose a valid option!" << endl;
        cin >> choice;
      }

      //if not completed ask for new deadline and assign new id and assign
      //the new issue to next sprint
      if(choice == "2") {
        long newdeadline;
        cout << "New Deadline(days)?" << endl;
        cin >> newdeadline;

        long newid = nextID;
        if(usedIDs->find(nextID) != usedIDs->end()) {
          nextID++;
          newid = nextID;
        }
        Issues *new_iss = new Issues(curr->get_name(),newid,newdeadline,curr->get_priority()
        ,curr->get_assignee(),curr->get_reporter(),curr->get_type(),curr->get_date(),curr->get_description());
        
        new_iss->set_inprogress();
        rollover->push(new_iss);
      } else {
        continue;
      }
    }
    //increment j for next sprint
    j++;
    roll_over();
    //this sprint is over so we dont need it anymore
    delete current_sprint;
  }

}

//decrement todo and sprints deadlines by specified number
void System::decrement_issues(long sum) {
  todo->decrement_issues(sum);
  for(int i = 0;  i < sprints->size(); i++) {
    (*sprints)[i]->decrement_issues(sum);
  }
}

//rollover any issues that werent completed
void System::roll_over() {
  //if there's another sprint to be run put it there
  if(active_sprints->size() >= 1) {
    while(!rollover->empty()) {
      Issues *isstemp = rollover->front();
      rollover->pop();
      active_sprints->front()->add_issue(isstemp);
    } 
  }

  //rollover to first sprint
  while(!rollover->empty()) {
    //if sprint doesnt have any sprints make a new one
    if(sprints->size() == 0) {
      sprints->push_back(new Tasks());
    }
    Issues *isstemp = rollover->front();
    rollover->pop();
    (*sprints)[0]->add_issue(isstemp);
  } 
  
}



///////////////////////////////////File IO Operations///////////////////////////////////////

//write file information to a file data.txt
void System::write_data() {
  ofstream outfile("data.txt");
  //basic project information
  outfile << project_name << endl;
  outfile << deadline << endl;
  outfile << nextID << endl; //the next id 

  //exporting users
  outfile << "USERS" << endl;
  for(int i = 0; i < users->size(); i++) {
    string membertype;
    if((*users)[i]->isOwner()) {
      membertype = "Owner";
    } else if((*users)[i]->isManager()) {
      membertype = "Manager";
    } else {
      membertype = "Member";
    }
    outfile << (*users)[i]->get_name() << endl;
    outfile << membertype << endl;
  }

  //output a current user if its not null
  outfile << "CURRENT_USER" << endl;
  if(current_user != nullptr) {
    outfile << current_user->get_name() << endl;
  } else {
    outfile << "None" << endl;
  }

  //output todo
  outfile << "TODO" << endl;
  outfile << todo->toString();

  //output the sprints
  outfile << "SPRINT" << endl;
  for(int i = 0; i < sprints->size(); i++) {
    outfile << "SPRINTS" << endl;
    outfile << (*sprints)[i]->toString();
  }

  //output done
  outfile << "DONE" << endl;
  outfile << done->toString();

  //output all the used ids
  outfile << "USEDIDS" << endl;
  set<long>::iterator it;
  for(it = usedIDs->begin(); it != usedIDs->end(); it++) {
    outfile << *it << endl;
  }

  //nothing to do with active sprints/rollover
  //since user can't end the program if theres an active sprint

  outfile.close();
}



//read from previously saved state in data.txt file
void System::read_data() {
  ifstream infile("data.txt");
  
  //get project name
  getline(infile,project_name);

  infile >> deadline; //get the deadline
  infile >> nextID; // get the next ID

  //go onto users
  string checkUser;
  infile >> checkUser;
  infile.ignore(1,'\n');
  if(checkUser == "USERS") {
    string name;
    //build users until we hit current_user tab
    while(getline(infile,name) && name != "CURRENT_USER") {
      string membertype;
      infile >> membertype;
      User* temp;
      if(membertype == "Owner") {
        temp = new Owner(name);
      } else if(membertype == "Manager") {
        temp = new Manager(name);
      } else if(membertype == "Member"){
        temp = new Member(name);
      } else if(membertype == "CURRENT_USER") {
        //this case isn't needed.
        break;
      }
      if(membertype != "") {
        users->push_back(temp);
      }
      infile.ignore(1,'\n');
      
    }
  }
  //get the current_user stored in data file
  string curr;
  if(getline(infile,curr) && curr != "None") {
    current_user = find_user(curr);
  }

  //get the todo stuff
  string istodo;
  todo = new Tasks();

  //read the todo section issue by issue
  infile >> istodo;
  infile.ignore(1,'\n');
  if(istodo== "TODO") {
    string n;
    while(getline(infile,n) && n != "SPRINT") {
      
      string t,dat,desc,comm,stat;
      long id,dl;
      int pr;
      User *a,*r;

      infile >> id;
      infile >> dl;
      infile >> pr;
      infile.ignore(1,'\n');

      string temp_reporter;
      getline(infile,temp_reporter);
      r = find_user(temp_reporter);

      string temp_assignee;
      getline(infile,temp_assignee);
      a = find_user(temp_assignee);

      getline(infile,t);
      getline(infile,dat);
      getline(infile,desc);
      getline(infile,comm);
      getline(infile,stat);

      Issues *newiss = new Issues(n,id,dl,pr,a,r,t,dat,desc,comm,stat);
      newiss->set_notstarted();
      todo->add_issue(newiss);
      
    } 
  }
  
  //read from the sprints section sprint by sprint
  int index = -1;
  string next;
  getline(infile,next);
  while(next != "DONE") {
    
    sprints->push_back(new Tasks());
    index++;
    while(getline(infile,next) && next != "SPRINTS" && next != "DONE") {
      string t,dat,desc,comm,stat;
      long id,dl;
      int pr;
      User *a,*r;

      infile >> id;
      infile >> dl;
      infile >> pr;
      infile.ignore(1,'\n');

      string temp_reporter;
      getline(infile,temp_reporter);
      r = find_user(temp_reporter);

      string temp_assignee;
      getline(infile,temp_assignee);
      a = find_user(temp_assignee);

      getline(infile,t);
      getline(infile,dat);
      getline(infile,desc);
      getline(infile,comm);
      getline(infile,stat);

      Issues *newiss = new Issues(next,id,dl,pr,a,r,t,dat,desc,comm,stat);
      newiss->set_inprogress();
      //newiss->display();
      (*sprints)[index]->add_issue(newiss);
    }
  }

  //read from the done section
  string ndone;
  while(getline(infile,ndone) && ndone != "USEDIDS") {
      string t,dat,desc,comm,stat;
      long id,dl;
      int pr;
      User *a,*r;

      infile >> id;
      infile >> dl;
      infile >> pr;
      infile.ignore(1,'\n');

      string temp_reporter;
      getline(infile,temp_reporter);
      r = find_user(temp_reporter);

      string temp_assignee;
      getline(infile,temp_assignee);
      a = find_user(temp_assignee);

      getline(infile,t);
      getline(infile,dat);
      getline(infile,desc);
      getline(infile,comm);
      getline(infile,stat);

      Issues *newiss = new Issues(ndone,id,dl,pr,a,r,t,dat,desc,comm,stat);
      newiss->set_done();
      done->add_issue(newiss);
  }

  //read from the used id section
  long nextnum;
  while(infile >> nextnum) {
    usedIDs->insert(nextnum);
  }
  
  //close the file after done reading from it
  infile.close();
}


////////////////////////////////////////Rule of 5///////////////////////////////////////////////////

// copy constructor
System::System(const System& other) {
  project_name = other.project_name;
  deadline = other.deadline;
  nextID = other.nextID;

  users = new vector<User*>();
  *users = *other.users;

  current_user = new Owner("");
  *current_user = *other.current_user;

  todo = new Tasks();
  *todo = *other.todo;

  sprints = new vector<Tasks*>();
  *sprints = *other.sprints;

  done = new Tasks();
  *done = *other.done;

  usedIDs = new set<long>();
  *usedIDs = *other.usedIDs;

  rollover = new queue<Issues*>();
  *rollover = *other.rollover;

  active_sprints = new queue<Tasks*>();
  *active_sprints = *other.active_sprints;
} 

//move constructor
System::System(System&& other) : deadline{other.deadline}, project_name{other.project_name}, 
  nextID{other.nextID},users{other.users}, todo{other.todo}, sprints{other.sprints}, current_user{other.current_user},
  done{other.done},usedIDs{other.usedIDs}, rollover{other.rollover},active_sprints{other.active_sprints} {
  
  other.users = nullptr;
  other.current_user = nullptr;
  other.todo = nullptr;
  other.sprints = nullptr;
  other.done = nullptr;
  other.usedIDs = nullptr;
  other.rollover = nullptr;
  other.active_sprints = nullptr;

}; 

//copy assignment
System& System::operator=(const System& other) {
  return *this = System(other);
}

//move assignment
System& System::operator=(System&& other) {
  swap(deadline,other.deadline);
  swap(project_name,other.project_name);
  swap(nextID,other.nextID);
  swap(users,other.users);
  swap(todo,other.todo);
  swap(sprints,other.sprints);
  swap(done,other.done);
  swap(current_user,other.current_user);
  swap(usedIDs,other.usedIDs);
  swap(rollover,other.rollover);
  swap(active_sprints,other.active_sprints);

  return *this;
}



