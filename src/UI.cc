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
std::vector<std::string> get_commands(const Library::User &user);
void login();







/**
bool checkout(std::string title) {
    if(library.count(title) == 0 || checked_out.size() > 3) {
        return false;
    }
    library[title]->take_copy(); // decrement item's copy count
    checked_out.push_back(library[title]);
    return true;
}
// method to return a checked out item
bool return_item(const std::string title) {
    // check the title with the books that are checked out
    for(int i = 0; i < checked_out.size(); i++) {
        Item* item = checked_out[i];
        if(item->get_name() == title) {  // if they match add the copy back erase it
            item->add_copy();
            checked_out.erase(checked_out.begin()+i); // erase it from checked out
            return true;
        }
    }
    return false;
}


void add() {
    std::cout << "";
}
// methods to add and remove items from the datastructure
void add(Item* item) {
    Library::library[item->get_name()] = item;
}
void remove(std::string title) {
    Library::library.erase(title);
}
*/





int main() {
    // instance variables
    bool run = true;
    std::string command;
    Library::User user;

    // print info for user
    std::cout << "Signed in as: " << user.to_string() << "\n";
    help(user);
    while(run) {
        std::cout << "> ";
        std::cin >> command;

        switch(user.get_id()) {
        case 2:
            if(command == "add") {
                std::string type;
                std::cin >> type;
                static_cast<Library::Librarian>(user).add(type);
            }
            else if(command == "remove") {
                std::string title;
                std::cin >> title;
                static_cast<Library::Librarian>(user).remove(title);
            }
            break;
        case 1:
            if(command == "checkout") {
                std::string title;
                std::cin >> title;
                static_cast<Library::Member>(user).checkout(title);
            }
            else if(command == "return") {
                std::string title;
                std::cin >> title;
                static_cast<Library::Member>(user).return_item(title);
            }
        default:
            if(command == "login") { } // FIX THIS
        }

        

        
        else if(command =="search") {
            std::string title;
            std::cin >> title;
            user.search(title);
        }
        else if(command == "all items") {user.print_items();}
        else if(command == "checkout") {
            std::string title;
            std::cin >> title;
            (Library::Member)user.checkout(title);
        }
        else if(command == "return") {
            std::string title;
            std::cin >> title;
            static_cast<Library::Member>(user).return_item(title);
        }
        else if(command == "add") {
            std::string title;
            std::cin >> title;
            static_cast<Library::Librarian>(user).add(title);
        }
        else if(command == "remove") {
            std::string title;
            std::cin >> title;
            static_cast<Library::Librarian>(user).remove(title);
        }
        else if(command == "exit") {
            run = false;
        }
    }
}