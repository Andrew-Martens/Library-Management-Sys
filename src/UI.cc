/**
* This application runs the user interface for 
* the library system
*  
* @author Andrew Martens
*/

#include<string>
#include<iostream>
#include<vector>
#include "Users.hh"
#include "Items.hh"

// forward declarations
std::vector<std::string> get_commands(const User &user);



// this method prints a list of commands
void help(const User &user) {
    // print user specific commands
    std::vector<std::string> commands = get_commands(user);
    for(std::string command : commands) {
        std::cout << command << "\n";
    }
    // prints commands all users have access to
    std::cout << "search\n" << "all items\n" << "exit\n\n";
}

// method to return user specific commands
std::vector<std::string> get_commands(const User &user) {
    switch(user.get_id()) {
        case 1:
            return {"logout", "checkout", "return"}; // commands for library members
        case 2:
            return {"logout", "add", "remove"}; // commands for librarians
        default:
            return {"login", "sign up"}; // commands for guest user
    }
}

int main() {
    bool run = true;
    std::string command;
    User user = User();
    std::cout << "Signed in as: " << user.to_string() << "\n";
    help();
    while(run) {
        std::cout << "> ";
        std::cin >> command;

        switch(command) {
            case
        }
    }
}