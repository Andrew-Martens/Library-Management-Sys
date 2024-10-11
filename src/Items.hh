/**
* This file contains class representations of library items
* the three items in this file include Book, Magazine, and Film
*  
* @author Andrew Martens
*/
#ifndef ITEMS_HH__
#define ITEMS_HH__

#include<string>
#include<iostream>
#include<cstdlib>
#include<unordered_map> 

namespace Library {

    // basic superclass for library items
    class Item {
        // member variables
        private:
        const int id;
        int copies;
        const std::string name;

        public:
        // Constructors - idealy params have a name and type
        // type corresponds to the type of library item:
        // 1 (book), 2 (film), 3 (magazine)
        Item() : id(0), copies(1), name("Unknown Item") {}
        Item(std::string name) : id(0), copies(1), name(name) {}
        Item(std::string name, int type) : id(type), copies(1), name(name) {}

        // getter methods for id and name
        int get_id() const { return id; }
        int get_copies() const {return copies; }
        void add_copy() {copies++;}
        void take_copy() {copies--;}
        std::string get_name() const { return name; }

        virtual void print() = 0;  // every subclass should have a print

        // override the comparative operators
        bool operator==(Item &item) {
            return id == item.get_id();
        }
        bool operator!=(Item &item) {
            return id != item.get_id();
        }
        bool operator>=(Item &item) {
            return id >= item.get_id();
        }
        bool operator<=(Item &item) {
            return id <= item.get_id();
        }

        virtual ~Item(){}  // destructor
    };

    class Book : public Item {
        private:
        const std::string author;
        
        public:
        Book(std::string name) : Item(name, 1), author("N/A") {}
        Book(std::string name, std::string author) : Item(name, 1), author(author) {}

        std::string get_author() const { return author; }  // getter method for author string
        void print() {
            std::cout << "Book: " + get_name() +"\nAuthor: " + get_author() + "\nCopies: " << get_copies();
        }

        ~Book(){}  // destructor for Book
        
    };

    class Film : public Item {
        private:
        const std::string director;

        public:
        Film(std::string name) : Item(name, 2), director("N/A") {}
        Film(std::string name, std::string director) : Item(name, 2), director(director) {}
        
        std::string get_director() const {return director;} // getter method for director string
        // string representation of this class
        void print() {
            std::cout << "Film: " + get_name() + "\nDirector: " + get_director() + "\ncopies: " << get_copies();
        }

        ~Film(){}  // destructor for Film
    };

    class Magazine : public Item {
        private:
        std::string publisher;

        public:
        Magazine(std::string name) : Item(name, 3), publisher("N/A") {}
        Magazine(std::string name, std::string publisher) : Item(name, 3), publisher(publisher) {}
        
        std::string get_publisher() const {return publisher;}; // getter method for publisher string
        // string representation of this class
        void print() {
            std::cout << "Magazine: " + get_name() + "\nPublisher: " + get_publisher() +"\nCopies: " << get_copies();
        }

        ~Magazine(){}  // destructor for Magazine
    };

    // get a random number from 9999-0
    int get_id() {
        return std::rand()%(10000);
    }
}

#endif