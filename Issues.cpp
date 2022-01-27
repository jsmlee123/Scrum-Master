#include "Issues.hpp"
#include "User.hpp"


#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <sstream>

#include "System.hpp"
#include "Issues.hpp"

using namespace std;

//constructor
Issues::Issues(std::string n, long i, long d, int p, User * a,User* r,std::string t, 
  std::string dat,std::string desc) {
  name = n;
  id = i;
  deadline = d;
  priority = p;
  assignee = a;
  reporter = r;
  type = t;
  date  = dat;
  description = desc;
  comments = "NA";
  status = "Not started";

}

//i coulda delegated to the other constructor but it wouldve made things look ugly
Issues::Issues(std::string n, long i, long d, int p, User * a,User* r,std::string t, 
  std::string dat,std::string desc,std::string comm,std::string status) {
  name = n;
  id = i;
  deadline = d;
  priority = p;
  assignee = a;
  reporter = r;
  type = t;
  date  = dat;
  description = desc;
  comments = comm;
  status = status;

}

void Issues::display() {
  cout << "Issue Name: " << name << endl;
  cout << "ID: " << id << endl;
  cout << "Deadline: " << deadline << endl;
  cout << "Priority: " << priority << endl;
  cout << "Reporter: " << reporter->get_name() << endl;
  if(assignee != nullptr) {
    cout << "Assignee: " << assignee->get_name() << endl;
  } else {
    cout << "Assignee: None" << endl;
  }
  
  cout << "Type: " << type << endl;
  cout << "Date Created: " << date << endl;
  cout << "Description: " << description << endl;
  cout << "Comments: " << comments << endl;
  cout << "Status: " << status << endl;
  return;
}

string Issues::toString() {
  stringstream out;
  out << name << endl;
  out << id << endl;
  out << deadline << endl;
  out << priority << endl;
  out << reporter->get_name() << endl;
  if(assignee != nullptr) {
    out << assignee->get_name() << endl;
  } else {
    out << "NONE" << endl;
  }
  
  out << type << endl;
  out << date << endl;
  out << description << endl;
  out << comments << endl;
  out << status << endl;
  
  return out.str();
}




User *find_user(std::string name,vector<User*> *users) {
  for(int i = 0; i < users->size(); i++) {
    if((*users)[i]->get_name() == name) {
      //current_user = (*users)[i];
      return (*users)[i];
    }
  }
  //cout << "User does not exist in database" << endl;
  return nullptr;
}

void Issues::modify_issue(User *curr, std::vector<User*> *users, std::set<long> *ids) {
  string choice;
  cout << "What would you like to modify" << endl;
  cout << "(1)Issue Name" << endl;
  cout << "(2)Deadline" << endl;
  cout << "(3)Type" << endl;
  cout << "(4)Date created" << endl;
  cout << "(5)Description" << endl;
  cout << "(6)Comments" << endl;
  cout << "(7)Priority" << endl;
  

  if(curr->isManager()) {
    cout << "(8)Status" << endl;
    cout << "(9)Assignee" << endl;
    cout << "(10)Reporter" << endl;
    cout << "(11)Change id" << endl;
  }
  
  cout << "(12) Edit none of these" << endl;

  cin >> choice;
  cin.clear();
  cin.ignore(10000,'\n');

  if(choice == "1") {
    cout << "Name?" << endl;
    getline(cin,name);
  } else if(choice == "2" ) {
    cout << "Deadline?" << endl;
    cin >> deadline;
    while(cin.fail()) {
      cin.clear();
      cin.ignore(10000,'\n');
      cout << "Please put an integer number!" << endl;
      cin >> deadline;
    }
  } else if(choice == "3") {
    cout << "Type?" << endl;
    getline(cin,type);
  } else if(choice == "4") {
    cout << "Date?" << endl;
    getline(cin,date);
  } else if(choice == "5" ) {
    cout << "Description?" << endl;
    getline(cin,description);
  } else if(choice == "6" ) {
    cout <<"Comments?" << endl;
    getline(cin,comments);
  } else if(choice == "7") {
    cout << "Priority?" << endl;
    cin >> priority;
    while(cin.fail()) {
      cin.clear();
      cin.ignore(10000,'\n');
      cout << "Please put an integer number!" << endl;
      cin >> priority;
    }
  }else if(choice == "12") {
    return;
  }

  string userName;
  User *tempuser = nullptr;
  if(curr->isManager()) {
    if(choice == "7") {
      cout << "Status?" << endl;
      
      getline(cin,status);
    } else if(choice == "8" ) {
      
      cout << "Assignee?" << endl;
      getline(cin,userName);
      tempuser = find_user(userName,users);
      if(tempuser != nullptr) {
        assignee = tempuser;
      } else {
        cout << "User does not exist!" << endl;
      }
      
    } else if(choice == "9") {
      cout << "Reporter?" << endl;
      getline(cin,userName);
      tempuser = find_user(userName,users);
      if(tempuser != nullptr) {
        reporter = tempuser;
      } else {
        cout << "User does not exist!" << endl;
      }
    } else if(choice == "10") {
      long newid;
      cout<< "New ID?" << endl;
      cin >> newid;
      if(ids->find(newid) != ids->end()) {
        cout << "ID already has been used!" << endl;
        return;
      } else {
        id = newid;
        ids->insert(newid);
      }

    }
  }
}

