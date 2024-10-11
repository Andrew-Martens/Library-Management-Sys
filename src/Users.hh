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
    std::unordered_map<std::string, const Item*> library;

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
        std::string get_name() { return name; }
        int get_id() {return id;}
        std::string to_string() { return name; }

        // method to search for a library item
        void search(const std::string& title) {
            if(library.count(title) == 0) {
                std::cout << title << " not found\n";
            }
        }  // AND THIS

        ~User() {}
    };

    // Members are logged in users with an id = 2
    // they can search checkout and return library items
    class Member : public User {
        private:
        std::vector<const Item*> checked_out;
        public:
        Member(std::string name) : User(name, 2) {}

        // method to get checked out items
        std::vector<const Item*> get_items() const { return checked_out; }

        bool checkout(std::string title) {
            if(library.count(title) == 0 || checked_out.size() > 3) {
                return false;
            }
            checked_out.push_back(library[title]);
            return true;
        }
        void return_item(const std::string title) {
            for(int i = 0; i < checked_out.size(); i++) {
                if()
            }
        }  // AND THIS
        

        ~Member() {}  // deconstructor for the Member class
    };

    // Librarians also must login
    // they can search, add, and remove library items
    class Librarian : public User {
        
        public:
        Librarian(std::string name) : User(name, 2) {}
        
        void add(const Item* item) {
            library[item->get_name()] = item;
        }
        void remove(std::string title) {
            library.erase(title);
        }

        ~Librarian();
    };
}

#endif