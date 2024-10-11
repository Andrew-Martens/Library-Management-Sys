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


namespace Library {
    
    // this method prints a list of commands
    void help() {
        // print user specific commands
        std::cout << "LIST OF COMMANDS:\n";
        std::cout << "search, all items, login,\nlogout, checkout, return, add, remove,\nand exit\n";
    }

    // method to log someone into the system
    // currently typing in class counts as logging in
    int login(const std::string& login) {
        if(login == "Librarian") {return 2;}
        if(login == "Member") {return 1;}
        return 0;
    }

    void run() {
        // instance variables
        bool run = true;
        std::string command;
        User user;
        Member member = Member("Member");
        Librarian librarian = Librarian("Librarian");
        int permission = 0;

        // print info for user
        std::cout << "Signed in as: " << user.to_string() << "\n";
        help();

        while(run) {
            std::cout << "> ";
            std::cin >> command;
            
            switch(permission) {
            case 2:
                if(command == "add") {
                    std::string title;
                    std::cin >> title;
                    librarian.add(title);
                }
                else if(command == "remove") {
                    std::string title;
                    std::cin >> title;
                    librarian.remove(title);
                }
            case 1:
                if(command == "checkout") {
                    std::string title;
                    std::cin >> title;
                    member.checkout(title);
                }
                else if(command == "return") {
                    std::string title;
                    std::cin >> title;
                    member.return_item(title);
                }
                else if(command == "logout") {  // command to logout
                    permission = 0;
                }
            default:
                if(command =="search") {  // command to search by string
                    std::string title;
                    std::cin >> title;
                    user.search(title);
                }
                else if(command == "all items") {user.print_items();} // prints all library items
                else if(command == "login") {
                    std::string info;
                    std::cin >> info;
                    permission = login(info);
                }
                else if(command == "exit") { run = false; }  // exits the system
            }
            std::cout << "Don't have permission/Invalid command\n";
        }

    }
}

int main() {
    Library::run();
}