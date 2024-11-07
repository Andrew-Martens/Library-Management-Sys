/**
* This file contains class representations of library items
* the three items in this file include Book, Magazine, and Film
* each item has a corresponding ID book = 1, film = 2, & magazine = 3
*  
* @author Andrew Martens
*/

#ifndef ITEMS_HH__
#define ITEMS_HH__

#include<string>
#include<iostream>
#include<filesystem>
#include<fstream>
#include<cstdlib>
#include<unordered_map> 

namespace LibSys {

    /**
     * @author Andrew Martens
     * @class Item
     * 
     * @brief parent class for library items. Bundles basic info, has getters, and overrides some operators
     */
    class Item {
        // member variables
        private:
        const int m_id;
        int m_copies;
        const std::string m_name;

        public:
        /** 
         * Constructors - idealy params have a name and type
         * type corresponds to the type of library item:
         * 1 (book), 2 (film), 3 (magazine) 
         */
        Item() : m_id(0), m_copies(1), m_name("Unknown Item") {}
        Item(std::string name) : m_id(0), m_copies(1), m_name(name) {}
        Item(std::string name, int type) : m_id(type), m_copies(1), m_name(name) {}
        Item(std::string name, int type, int copies) : m_id(type), m_copies(copies), m_name(name) {}

        // getter methods for id, copies, and name
        int get_id() const { return m_id; }
        int get_copies() const {return m_copies; }
        std::string get_name() const { return m_name; }

        //methods to increment/decrement item copies
        void add_copy() {m_copies++;}
        void take_copy() {m_copies--;}
        

        virtual void print() = 0;  // every subclass should have a print & write method
        virtual bool write(std::string) = 0; 

        // override the comparative operators
        bool operator==(Item &item) {
            return m_id == item.get_id();
        }
        bool operator!=(Item &item) {
            return m_id != item.get_id();
        }
        bool operator>=(Item &item) {
            return m_id >= item.get_id();
        }
        bool operator<=(Item &item) {
            return m_id <= item.get_id();
        }

        virtual ~Item(){}  // destructor
    };


    /**
     * @author Andrew Martens
     * @class Book
     * 
     * @brief Child class derived from @class: Item. It has a string for author and overrides Item's print()
     */
    class Book : public Item {
        private:
        const std::string m_author;
        
        public:
        /*
         * Constructors for Book class
         *
         * 
         */
        Book(std::string name) : Item(name, 1), m_author("N/A") {}
        Book(std::string name, std::string author) : Item(name, 1), m_author(author) {}
        Book(std::string name, std::string author, int copies) : Item(name, 1, copies), m_author(author) {}

        std::string get_author() const { return m_author; }  // getter method for m_author

        //prints class representation
        void print() {
            std::cout << "Book: " + get_name() +"\nAuthor: " + m_author + "\nCopies: " << get_copies() << "\n";
        }

        //writes class variables to given file path
        bool write(std::string path) {
            //ensure file exists
            if(!std::filesystem::exists(path)) {
                return false;
            }
            
            std::ofstream ofs(path, std::ios_base::app);
            ofs << get_id() << ";" << get_name() << ";" << m_author << ";" << get_copies() << "\n";
            ofs.close();
            return true;
        }

        ~Book(){}  // destructor for Book
        
    };

    /**
     * @author Andrew Martens
     * @class Film
     * 
     * @brief Child class derived from @class: Item. It has a string for director and overrides Item's print()
     */
    class Film : public Item {
        private:
        const std::string m_director;

        public:
        Film(std::string name) : Item(name, 2), m_director("N/A") {}
        Film(std::string name, std::string director) : Item(name, 2), m_director(director) {}
        Film(std::string name, std::string author, int copies) : Item(name, 2, copies), m_director(author) {}
        
        std::string get_director() const {return m_director;} // getter method for director string

        // prints representation of this class
        void print() {
            std::cout << "Film: " + get_name() + "\nDirector: " + m_director + "\ncopies: " << get_copies() << "\n";
        }

        //writes class variables to given file path
        bool write(std::string path) {
            //ensure file exists
            if(!std::filesystem::exists(path)) {
                return false;
            }
            
            std::ofstream ofs(path, std::ios_base::app);
            ofs << get_id() << ";" << get_name() << ";" << m_director << ";" << get_copies() << "\n";
            ofs.close();
            return true;
        }

        ~Film(){}  // destructor for Film
    };


    /**
     * @author Andrew Martens
     * @class Magazine
     * 
     * @brief Child class derived from @class: Item. It has a string for publisher and overrides Item's print()
     */
    class Magazine : public Item {
        private:
        std::string m_publisher;

        public:
        Magazine(std::string name) : Item(name, 3), m_publisher("N/A") {}
        Magazine(std::string name, std::string publisher) : Item(name, 3), m_publisher(publisher) {}
        Magazine(std::string name, std::string author, int copies) : Item(name, 3, copies), m_publisher(author) {}
        
        std::string get_publisher() const {return m_publisher;}; //getter method for publisher string

        //prints representation of this class
        void print() {
            std::cout << "Magazine: " + get_name() + "\nPublisher: " + m_publisher +"\nCopies: " << get_copies() << "\n";
        }

        //writes class variables to given file path
        bool write(std::string path) {
            //ensure file exists
            if(!std::filesystem::exists(path)) {
                return false;
            }
            
            std::ofstream ofs(path, std::ios_base::app);
            ofs << get_id() << ";" << get_name() << ";" << m_publisher << ";" << get_copies() << "\n";
            ofs.close();
            return true;
        }

        ~Magazine(){}  //destructor for Magazine
    };
}

#endif