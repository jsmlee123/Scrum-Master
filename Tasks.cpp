#include "Tasks.hpp"
#include "Issues.hpp"
#include <string>
#include <iostream>
#include <limits.h>

using namespace std;

//remove an issue
Issues *Tasks::remove_issue(long id) {
  for(int i = 0; i < issues->size(); i++) {
      if((*issues)[i]->get_id() == id) {
        Issues *ret = (*issues)[i];
        //removes the issue from the vector
        //and returns its pointer
        issues->erase(issues->begin() + i);
        return ret;
      }
  }
  return nullptr;
}

//find the issue with the given id in the task
Issues *Tasks::find_issue(long id) {
  for(int i = 0; i < issues->size(); i++) {
      if((*issues)[i]->get_id() == id) {
        Issues *ret = (*issues)[i];
        return ret;
      }
  }
  return nullptr;
}


//search for the highest priority issue
//lower priority is better
Issues *Tasks::find_highest_priority() {
  //set min priority to int max
  int priority = INT_MAX;
  int index;
  Issues *temp = nullptr;
  for(int i = 0 ; i < issues->size(); i++) {
    //if priority of issue is lower than lowest one we've seen so far 
    //use it
    if((*issues)[i]->get_priority() < priority) {
      priority = (*issues)[i]->get_priority();
      temp = (*issues)[i]; 
      index = i;
    }
  }
  //erase it from the list
  if(temp != nullptr) {
    issues->erase(issues->begin() + index);
  }

  return temp;
}

//set all the issues in this task to done
void Tasks::set_done() {
  for(int i = 0; i < issues->size(); i++) {
    (*issues)[i]->set_done();
  }
}

//decrement the deadlines of each issue by the specified number
void Tasks::decrement_issues(long sum) {
  for (int i = 0; i < issues->size() ; i ++) {
    (*issues)[i]->set_deadline(sum);
  }
}

//display the issues in the task to cout
void Tasks::display(User *curr) {
  for(int i = 0; i < issues->size(); i++) {
    if((*issues)[i]->get_assignee() != nullptr && (*issues)[i]->get_assignee()->get_name() == curr->get_name()) {
      (*issues)[i]->display();
    } else {
      if(curr->isManager()) {
        (*issues)[i]->display();
      }
    }
    cout << endl;
  }
}

//return the string format of this task
string Tasks::toString() {
  string out = "";
  for(int i = 0; i < issues->size(); i++) {
    out += (*issues)[i]->toString();
  }
  return out;
}

//delete all the issues in this task
void Tasks::delete_issues() {
  for(int i = 0; i < issues->size(); i++) {
    delete (*issues)[i];
  }
}

//check if user exists
User *user_exists(std::string name,vector<User*> *users) {
  for(int i = 0; i < users->size(); i++) {
    if((*users)[i]->get_name() == name) {
      return (*users)[i];
    }
  }
  //cout << "User does not exist in database" << endl;
  return nullptr;
}

//make sure each issue has an assigned person to it
void Tasks::assign_sprints(std::vector<User*> *users) {
  for(int i = 0; i < issues->size(); i++) {
    if((*issues)[i]->get_assignee() == nullptr) {
      string a;
      cout << "Assign which user to Issue:" << (*issues)[i]->get_name() << endl;
      cout << "ID: " << (*issues)[i]->get_id() << endl;

      cin.clear();
      cin.ignore(10000,'\n');

      getline(cin,a);
      //make sure a person is actually assigned to the issue
      while(user_exists(a,users) == nullptr) {
        cout << "Valid User Please!"  << endl;
        getline(cin,a);
      }

      //issue is properly assigned
      cout << "Issue " << (*issues)[i]->get_id() << " assigned" << endl; 
      (*issues)[i]->set_assignee(user_exists(a,users));
    }
  }

}