/**
* This file contains class representations of library users
* the three users in this file include Guest, Member, and Librarian
* 
* @author Andrew Martens
*/

#ifndef USERS_HH__
#define USERS_HH__

#include<vector>
#include<string>
#include<fstream>
#include<iostream>
#include "Items.hh"

namespace Library {

    // the datastructure to hold all the items
    std::unordered_map<std::string, Item*> library;
    

    // The most basic user class - 
    // they can only search for items and have an id = 0
    class User {
        private:
        std::string name;
        int id;

        public:
        // constructors
        User() : name("Guest"), id(0){}
        User(std::string name, int id) : name(name), id(id){}

        // getter and to string methods
        std::string get_name() const { return name; }
        int get_id() const {return id;}
        std::string to_string() const { return name; }

        // method to search for a library item
        void search(const std::string& title) {
            if(library.count(title) == 0) {
                std::cout << "'" << title << "' not found\n";
            } else {
                Item* item = library[title];
                int id = item->get_id();
                switch(id) {
                    case 1:
                        dynamic_cast<Book*>(item)->print();
                        std::cout << "\n\n";
                        break;
                    case 2: 
                        dynamic_cast<Film*>(item)->print();
                        std::cout << "\n\n";
                        break;
                    case 3: 
                        dynamic_cast<Magazine*>(item)->print();
                        std::cout << "\n\n";
                        break;
                }
            }
        }

        // method to print all the items in the library
        void print_items() {
            std::cout << "Listing all items:\n";
            for(const auto& pair : library) {
                Item* item = pair.second;
                int id = item->get_id();
                switch(id) {
                    case 1:
                        dynamic_cast<Book*>(item)->print();
                        std::cout << "\n\n";
                        break;
                    case 2: 
                        dynamic_cast<Film*>(item)->print();
                        std::cout << "\n\n";
                        break;
                    case 3: 
                        dynamic_cast<Magazine*>(item)->print();
                        std::cout << "\n\n";
                        break;
                }
            }
        }

        ~User() {}
    };

    // Members are logged in users with an id = 1
    // they can search checkout and return library items
    class Member : public User {
        private:
        std::vector<Item*> checked_out;
        public:
        Member(std::string name) : User(name, 1) {}

        // method to get checked out items
        std::vector<Item*> get_items() const { return checked_out; }

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
        

        ~Member() {}
    };

    // Librarians also must login
    // they can search, add, and remove library items
    class Librarian : public User {
        public:
        Librarian(std::string name) : User(name, 2) {}
        

        // methods to add and remove items from the datastructure  
        void add(std::string type) {
            // variables for the title and extra info
            std::string title;
            std::string extra;
            if(type == "book") {
                std::cout << "Enter title formated: \"The Title\"";
                std::cin >> title;
                std::cout << "Enter the author: ";
                std::cin >> extra;
                Book b = Book(title, extra);
                Book* item = &b;
                add(item);
                return;
            }
            if(type == "magazine") {
                std::cout << "Enter title formated: \"The Title\"";
                std::cin >> title;
                std::cout << "Enter the author: ";
                std::cin >> extra;
                Magazine b = Magazine(title, extra);
                Magazine* item = &b;
                add(item);
                return;
            }
            if(type == "film") {
                std::cout << "Enter title formated: \"The Title\"";
                std::cin >> title;
                std::cout << "Enter the author: ";
                std::cin >> extra;
                Film b = Film(title, extra);
                Film* item = &b;
                add(item);
                return;
            }
        } 
        void add(Item* item) {
            library[item->get_name()] = item;
        }
        void remove(std::string title) {
            library.erase(title);
        }

        ~Librarian() {}
    };
}

#endif