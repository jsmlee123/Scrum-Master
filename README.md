# Issue Tracking App

An app used for scrum related things

Compile with "g++ -std=c++11 -o main main.cpp Issues.cpp System.cpp Tasks.cpp"

Run with ./main

# Opening/Creating a project
- When opening a project, a user is prompted by 2 options, loading or creating a project
- Loading project loads previously stored project 
- Creating project creates a new project and prompts user for name
  of project , project deadline, and project
- Project information can be changed via option 1 in the main menu

# Adding/removing/switching users
- User menu is opened by option 2 in the main menu
- User can switch current user to specified user by choosing option 2 in main menu
- Adding or removing user is limited to project owners and project managers and
      can choose the employee type of the new user (owner, manager, member) depending on authority

# Issue operations and sprints
- Issue and sprints menu is opened by option 4 in the main menu
- Adding/moving/removing issues are limited to managers and owners.
- Adding an issue prompts the user to fill out all the information regarding
  a story/issue and prompts the user to assign it to a user. Each issue is assigned a unique ID
- A owner/ manager can also update information about an existing issue and can re-assign issue
  common members are only allowed to assign issues to themselves
- Owners and managers have the option to move the highest priority issue into the next sprint
- An owner/manager can move issues anywhere between to-do pile, different sprints, and the finished pile

# Simulating sprints
- An owner/manager can initiate sprints by choosing option 5 in the main menu
- An owner/manager can choose how many sprints to run consecutively and is asked how long
  the sprints will .The sprints will run for a simulated time of 10 seconds per day
- At the end of the sprint, the user is asked if the issues were completed and if so they 
  are moved to the finished pile. If not, the issue is reassigned a deadline and moved to the next sprint

# Displaying information
- Display menu is opened by option 4 in the main menu
- A user can display project information and users without specified permissions
- A user can only see issues they are assigned to unless they are a owner/manager
  in which case they see all
- A user can see specific information about issues/stories given they know its id

# Saving project
- The project is saved on each command done so if the program crashes or is exited, the project information 
  is saved.