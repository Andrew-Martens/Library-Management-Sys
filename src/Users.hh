/**
* This file contains a class representation of a library user.
* It bundles a string name, vector of items, id, and 
* 
* @author Andrew Martens & Rachel Roach
*/

#ifndef USERS_HH__
#define USERS_HH__

#include<vector>
#include<string>
#include<fstream>
#include<iostream>
#include "Items.hh"

namespace LibSys {

    /**
     * @author Andrew Martens & Rachel Roach
     * @class User
     * 
     * @brief basic struct to store library user information and a method to print
     */
    struct User {
        //member variables
        const std::string m_name;
        const std::string m_password;
        std::vector<Item*> m_items;
        const int m_id;

        // constructors
        User() : m_name("Guest"), m_password("0"), m_id(-1) {}
        User(std::string name, std::string password, int id) : m_name(name), m_password(password), m_id(id) {}
        User(std::string name, std::string password, int id, std::vector<Item*> items) : m_name(name), m_password(password), m_id(id), m_items(items) {}

        /**
         * @author Andrew Martens
         *
         * @brief prints user information to console
         */
        void print() {
            std::cout << "Name: " << m_name << "\nPassword: " << m_password << "\nItems: ";

            for(const Item* item : m_items) { //print each item user has checked out
                std::cout << "'" << item->get_name() << "' ";
            }
            std::cout << "\nID: " << m_id << "\n";
        }

        ~User() {}
    };
}

#endif