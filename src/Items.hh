/**
* This file contains class representations of library items
* the three items in this file include Book, Magazine, and Film
*  
* @author Andrew Martens
*/
#ifndef ITEMS_HH__
#define ITEMS_HH__

#include<string>
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
        // 10000(book), 20000(film), 30000(magazine)
        Item() : id(0), copies(1), name("Unknown Item") {}
        Item(std::string name) : id(0), copies(1), name(name) {}
        Item(std::string name, int type) : id(type + get_id()), copies(1), name(name) {}

        // getter methods for id and name
        int get_id() const { return id; }
        int get_copies() const {return copies; }
        std::string get_name() const { return name; }

        virtual std::string to_string() = 0;  // every subclass should have a to_string

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
        const std::string genre;
        
        public:
        Book(std::string name) : Item(name, 10000), author("N/A"), genre("N/A") {}
        Book(std::string name, std::string author) : Item(name, 10000), author(author), genre("N/A") {}
        Book(std::string name, std::string author, std::string genre) : Item(name, 10000), author(author), genre(genre) {}

        std::string get_author() const { return author; }  // getter method for author string
        std::string get_genre() const { return genre; }  // getter method for author string
        std::string to_string() {
            return "Book: " + get_name() +"\n" + get_author() + "\n" + get_genre();
        }

        ~Book(){}  // destructor for Book
        
    };

    class Film : public Item {
        private:
        const std::string director;

        public:
        Film(std::string name) : Item(name), director("N/A") {}
        Film(std::string name, std::string director) : Item(name), director(director) {}
        
        std::string get_director() const {return director;} // getter method for director string
        // string representation of this class
        std::string to_string() {
            return "Film: " + get_name() + "\n" + get_director();
        }

        ~Film(){}  // destructor for Film
    };

    class Magazine : public Item {
        private:
        std::string publisher;

        public:
        Magazine(std::string name) : Item(name), publisher("N/A") {}
        Magazine(std::string name, std::string publisher) : Item(name), publisher(publisher) {}
        
        std::string get_publisher() const {return publisher;}; // getter method for publisher string
        // string representation of this class
        std::string to_string() {
            return "Magazine: " + get_name() + "\n" + get_publisher();
        }

        ~Magazine(){}  // destructor for Magazine
    };

    // get a random number from 9999-0
    int get_id() {
        return std::rand()%(10000);
    }
}

#endif