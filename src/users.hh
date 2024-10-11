/**
* This file contains class representations of library users
* the three users in this file include Guest, Member, and Librarian
* 
* @author Andrew Martens
*/

#include<vector>
#include<string>
#include<fstream>
#include<ifstream>
#include<ofstream>
#include "Items.hh"
#include "Tree.hh"

namespace Library {



    class User {
        private:
        std::string m_name;

        public:
        User() : m_name("Guest") {}
        User(std::String name) : m_name(name) {}

        std::string get_name() { return m_name; }

        ~User() {}
    };

    class Member : public User {
        private:
        std::vector<Item> m_checked_out;
        int m_id;
        public:
        Member(std::string name, int id) : User(std::string name), m_id(id) {
            m_items = load_items(id);
        }



        ~Member() {}  // deconstructor for member class
    };

    class Librarian : public User {
        
        public:
        Librarian(std::string name) : User(std::string name) {}
        
        bool add_item(Item item) {

        }

        ~Librarian();
    }
}