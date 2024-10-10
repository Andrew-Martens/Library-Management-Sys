/**
* This file contains class representations of library items
* the three items in this file include Book, Magazine, and Film
* 
* @author Andrew Martens
*/

#include<string>

namespace Library {
    // basic superclass for library items
    class Item {
        private:
        int m_id;
        std::String m_name;
        public:
        // 
        Item() : m_id(generate_id()), m_name("Unknown Item") {}
        Item(std::string name) : m_id(generate_id()), m_name(); {}

        // getter methods for id and name
        int get_id() { return m_id; }
        std::string get_name() {return m_name; }

        virtual std::string to_string() = 0;

        virtual ~Item(){}
    };

    class Book : public Item {
        private:
        std::string m_author;
        
        public:
        Book(std::string name) : Item(name), m_author("N/A") {};
        Book(std::string name, std::string author) : Item(name), m_author(author) {};

        std::string get_author();  // getter method for author string
        std::string to_string() {
            return get_name() + "\n" + get_author();
        }

        ~Book(){}  // destructor for Book
        
    };

    class Film : public Item {
        private:
        std::string m_director;

        public:
        Film(std::string name) : Item(name), m_director("N/A") {};
        Film(std::string name, std::string director) : Item(name), m_director(director) {};
        
        std::string get_director() {return m_director;}; // getter method for director string
        // string representation of this class
        std::string to_string() {
            return get_name() + "\n" + get_director();
        }

        ~Film(){}  // destructor for Film
    };

    class Magazine : public Item {
        private:
        std::string m_publisher;

        public:
        Magazine(std::string name) : Item(name), m_publisher("N/A") {};
        Magazine(std::string name, std::string publisher) : Item(name), m_publisher(publisher) {};
        
        std::string get_publisher() {return m_publisher;}; // getter method for publisher string
        // string representation of this class
        std::string to_string() {
            return get_name() + "\n" + get_publisher();
        }

        ~Magazine(){}  // destructor for Magazine
    };
}