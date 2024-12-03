/**
* @author Andrew Martens
*
* This file contains the high level functionallity for the library system
* to use create Library object and call run()
*
* TODO: ptentially add a remove-user function
* CURRENT: DOCUMENT AND TEST!!!
*  
* 
*/

#ifndef LIBRARY_HH__
#define LIBRARY_HH__

#include<string>
#include<iostream>
#include<vector>
#include<filesystem>
#include<fstream>
#include<unordered_map>
#include "Users.hh"
#include "Items.hh"


namespace LibSys {

    /**
     * @author Andrew Martens
     * @class Library
     *
     * @brief higher level class that manages interactions between users and library inventory
     * @note outside of this class Library objects should only call @see run()
     */
    class Library {
        //private variables
        std::unordered_map<std::string, Item*> m_library;
        const std::string m_library_name;
        const std::string u_filename; //user filename
        const std::string i_filename; //inventory filename
        User* m_user;
        std::string m_username;

    public:
        /**
         * Main constructor for high level Library class
         * 
         * The constructor builds a directory structure to store library data and 
         * loads any items that have been prviously saved.
         *
         * Directory structure as follows:
         *      Current
         *          libraries
         *              <library_name>
         *                  items.txt
         *                  inventory.txt
         *
         */
        Library(std::string name) : m_library_name(name), m_user(nullptr), m_username("Guest"),
                                    u_filename("libraries/"+name+"/users.txt"), i_filename("libraries/"+name+"/inventory.txt") {

            // Create directory structure to store library information
            std::filesystem::create_directory("libraries");
            std::filesystem::path lib_path("libraries/" + name);
            if(std::filesystem::create_directory(lib_path)) {
                // Create the files that store user, and inventory information
                std::ofstream user_file(u_filename);
                user_file.close();
                std::ofstream inventory_file(i_filename);
                inventory_file.close();
            }
            load_items(); //load items into unordered_map
        }

        
        /**
         * @author Andrew Martens
         * 
         * @brief this method runs the actual library system and offers relevant functionallities
         * @see close(), help(), login(), search(), print_items(), 
         *      checkout(), add(), & remove_item()
         */
        void run() {
            std::string command;
            help();
            while(true) {
                std::cout << m_username << " @ " << m_library_name<<" > ";
                std::cin >> command;
                if(command=="exit") {
                    //ensure user is logged out
                    if(m_user) {
                        write_users();
                        delete m_user;
                        m_user = nullptr;
                    }
                    //delete items from heap
                    close();
                    std::cout << "System terminating...\n";
                    return;
                } else if (command=="help") {
                    help(); 
                } else if (command=="login") {
                    if(m_user) {
                        std::cout << "Already logged in\n";
                        continue;
                    }
                    login();
                } else if (command=="search") {
                    //get keyword from user
                    std::string keyword;
                    std::getline(std::cin, keyword);
                    keyword.erase(0,1); //removes the first char since it picks up a space

                    if(!search(keyword)) {
                        std::cout << "'" << keyword << "' not found\n";
                    }
                } else if (command=="all-items") {
                    print_items();
                } else if (command=="logout") {
                    //updates file then deletes m_user from heap
                    if(m_user) {
                        write_users();
                        delete m_user;
                        m_user = nullptr;
                        m_username = "Guest";
                    }
                } else if(command=="profile"){
                    if(m_user) {
                        m_user->print();
                    }
                } else if (command=="checkout") {
                    //ensure user is logged in
                    if(!m_user){
                        std::cout << "Must login to checkout items\n";
                        continue;
                    }
                    checkout();
                } else if (command=="return-item"){
                    //ensure user is logged in
                    if(!m_user){
                        std::cout << "Must login to checkout items\n";
                        continue;
                    }
                    return_item();
                } else if(command=="add-item") {
                    //ensure user is logged in to an admin account
                    if(!m_user || m_user->m_id < 200000){
                        std::cout << "Only librarians can access this command\n";
                        continue;
                    }
                    add_item();
                } else if(command=="remove-item") {
                    //ensure user is logged in to an admin account
                    if(!m_user || m_user->m_id < 200000){
                        std::cout << "Only librarians can access this command\n";
                        continue;
                    }
                    remove_item();
                } else {
                    std::cout << "Unknown Command\n";
                }
            }
        }

        // simple deconstructor
        ~Library() {}
    private:

        /**
         * @author Andrew Martens
         *
         * @brief loads library items from @file: <library_name>/inventory.txt into unordered map
         * @see tokenize_line(std::ifstream, char delimiter, vector<string> tokens). Used to split file entries into tokens
         * @note only called in constructor
         */
        void load_items() {
            //variables
            std::ifstream ifs(i_filename);
            std::string buffer;
            std::vector<std::string> tokens;
            int id;
            int copies;
            char delimiter = ';';

            // The file entries follow the following format
            //<id>;<title>;<creator>;<copies>\n
            //and therefore is stored into the tokens sequentially
            while(!ifs.eof()) {
                //tokenizes next line in the file
                tokenize_line(ifs, delimiter, tokens);

                //ensure id can be casted to int
                try{
                    id = std::stoi(tokens[0]);
                    copies = std::stoi(tokens[3]);
                } catch(std::invalid_argument) {
                    return;
                }

                //switch off id to cast correctly
                switch(id) {
                    case 1: 
                    {
                        //construct new book item & add to unordered_map
                        Item* item = new Book(tokens[1], tokens[2], copies);
                        m_library[tokens[1]] = item;
                        continue;
                    }
                    case 2:
                    {
                        //construct new film item
                        Item* item = new Film(tokens[1], tokens[2], copies);
                        m_library[tokens[1]] = item;
                        continue;
                    }
                    case 3:
                    {
                        //construct new magazine item
                        Item* item = new Magazine(tokens[1], tokens[2], copies);
                        m_library[tokens[1]] = item;
                        continue;
                    }
                }

            }
            std::cout << "Library Items Loaded Successfully\n";
            ifs.close();
        }

        /**
         * @author Andrew Martens
         *
         * @brief writes user information to file
         *
         * To avoid file fragmentation, every user except for the logged in user
         * is coppied from users.txt to a temp file that replaces users.txt.
         */
        void write_users() {
            //input file from users.txt and create a temp file to write to
            std::string t_filename = "Libraries/"+ m_library_name +"/temp.txt";
            std::ifstream ifs(u_filename);
            std::ofstream temp(t_filename);

            //write logged in user to file to save any changes
            temp << m_user->m_name << ";" << m_user->m_id;
            for(Item* item : m_user->m_items) {
                temp << ";" << item->get_name();
            }
            temp << "\n";

            //variables
            std::string buffer;
            std::vector<std::string> tokens;
            char delimiter = ';';
            bool found = false;

            while(std::getline(ifs, buffer)) {
                //if user hasn't been found check for match
                if(!found) {
                    tokens.clear();
                    split(buffer, delimiter, tokens);

                    if(m_user->m_name == tokens[0]) {
                        found = true;
                        continue; //skips iteration where buffer has user's previous info
                    } 
                }
                
                //writes entry to temp.txt
                temp << buffer << "\n";
            }
            temp.close();
            ifs.close();

            //remove old file and rename new temp file
            remove(u_filename.c_str());
            rename(t_filename.c_str(), u_filename.c_str());
        }

        

        /**
         * @author Andrew Martens
         * @brief deletes all library items from heap
         * 
         * @note this method should only be called before deconstructing the @class Library
         */
        void close() {
            std::ofstream ofs(i_filename, std::ios::trunc);
            ofs.close();
            //delete library items from heap
            for(const auto& pair : m_library) {
                Item* item = pair.second;
                item->write(i_filename);
                delete item;
            }
            // clear library to avoid the use of dangling pointers
            m_library.clear();
        }

        /**
         * @author Rachel Roach & Andrew Martens
         * 
         * @brief prints a list of commands to the console
         */
        void help() {
            std::cout << "\nLIST OF COMMANDS:";
            std::cout << "\n-------------------------------------------------";
            std::cout << "\n1.  help\t\t2. search <name>\n3.  all-items\t\t4. login <username>\n5.  logout\t\t6. profile\n";
            std::cout << "7.  checkout <title>\t8. return-item <title>\n9.  add-item\t\t10. remove-item <title>\n11. exit";
            std::cout << "\n-------------------------------------------------\n\n";
        }

        /**
         * @author Andrew Martens
         *
         * @brief checks inputed username against known users & can create a new user if username is unique
         * 
         * This function promps user to enter their username and checks the username against 
         * @file: <library_name>/users.txt. On a match the user is constructed, else the
         * user has the option to create a new account.
         * 
         * @see
         */
        void login() {
            // user variables
            std::string username;
            int id;


            //get username and email from user
            std::getline(std::cin, username);
            username.erase(0,1);

            if(!validate(username)) {
                std::cout << "Error: Username can't contain a semicolon\n";
                return;
            }


            //create input file stream to see all users, 
            std::ifstream ifs(u_filename);
            std::vector<std::string> tokens;
            char delimiter = ';';

            while(!ifs.eof()) {
                //tokenize next line
                tokenize_line(ifs, delimiter, tokens);
                
                
                //if the entered username matches the entry's username construct user
                if(tokens[0] == username) {
                    //convert id to int. Throws error message if it can't be converted
                    try{
                        id = std::stoi(tokens[1]);
                    } catch(std::invalid_argument) {
                        std::cout << "Error Logging in: File format error\n";
                        return;
                    }

                    // construct user
                    m_user = new User(username, id);
                    m_username = username;
                     

                    // any remaining tokens are items the user checked out
                    // therefore every token past token[1] needs to be pushed to user's items vector
                    for(int i = 2; i < tokens.size(); i++) {
                        // get item title and push back the corresponding Item from the library's unordered map
                        std::string item = tokens[i];
                        m_user->m_items.push_back(m_library[item]);
                    }
                    m_user->print(); //print user info

                    ifs.close();
                    return;
                }
            }
            //if no account was found close input file and ask user if they would like to make an account
            ifs.close();
            std::cout << "Username not found.\nWould you like to make an account [y/n]: ";
            char ch;
            std::cin >> ch;

            if(ch == 'y') {
                //make id for user and write new user into <library_name>/users.txt
                std::cout << "To create librarian account enter 'l', else enter 'm' > ";
                std::cin >> ch;

                if(ch == 'l') { //code to create librarian/admin account
                    //construct user with admin permision and print
                    id = create_id(true);
                    m_user = new User(username, id);
                    m_username = m_user->m_name;
                } else { //code to create library member account
                    //construct user without admin permision and print
                    id = create_id(false);
                    m_user = new User(username, id);
                    m_username = m_user->m_name;
                }
                //print conformation
                std::cout << "Created new user:\n";
                m_user->print();
            }
        }

        /**
         * @author Andrew Martens
         *
         * @brief tokenizes the next line in an input file stream
         */
        void tokenize_line(std::ifstream& ifs, char delimiter, std::vector<std::string>& tokens) {
            std::string buffer;
            std::getline(ifs, buffer);
            tokens.clear();
            split(buffer, delimiter, tokens);
        }

        /**
         * @author Andrew Martens
         * 
         * @brief splits a string and stores tokens in a vector
         */
        void split(std::string s, const char& delimiter, std::vector<std::string>& tokens) {
            size_t pos = 0;
            std::string token;

            //get position of the delimiter
            while ((pos = s.find(delimiter)) != std::string::npos) {
                //push back token and erase string up to the position 
                token = s.substr(0, pos);
                tokens.push_back(token);
                s.erase(0, pos + 1);
            }
            tokens.push_back(s);
        }



        /**
         * @author Andrew Martens
         * 
         * @brief searches through library's inventory to see if an item with inputed keyword exists
         * @param keyword the title to search for
         * @returns false if item isn't found
         */
        bool search(std::string keyword) {
            //check if item exists in the library
            if(m_library.count(keyword) == 0) {
                return false;
            } else {
                std::cout << "\n"; //formating

                // gets item and id from the map then casts it based on its id
                Item* item = m_library[keyword];
                int id = item->get_id();
                switch(id) {
                    case 1:
                        dynamic_cast<Book*>(item)->print();
                        break;
                    case 2: 
                        dynamic_cast<Film*>(item)->print();
                        break;
                    case 3: 
                        dynamic_cast<Magazine*>(item)->print();
                        break;
                }
                std::cout << "\n"; //formating
                return true;
            }
        }

        /**
         * @author Andrew Martens
         * 
         * @brief prints all library items
         */
        void print_items() {
            std::cout << "\nListing all items:\n\n";
            // iterates through each key-value pair in the unordered map
            for(const auto& pair : m_library) {
                Item* item = pair.second;
                int id = item->get_id();

                //switch off id for correct casting then print item
                switch(id) {
                    case 1:
                    {
                        dynamic_cast<Book*>(item)->print();
                        std::cout << "\n";
                        break;
                    }
                    case 2: 
                    {
                        dynamic_cast<Film*>(item)->print();
                        std::cout << "\n";
                        break;
                    }
                    case 3: 
                    {
                        dynamic_cast<Magazine*>(item)->print();
                        std::cout << "\n";
                        break;
                    }
                }
            }
        }

        /**
         * @author Andrew Martens
         *
         * @brief returns one of two static IDs that correspond to user permisions
         */
        int create_id(bool is_admin) {
            //IDs for admin accounts start with a '2'
            if(is_admin) {return 200000;}
            return 100000;
        }

        /**
         * @author Andrew Martens
         *
         * @brief checks a library item out based of a given keyword
         * @note user must be logged in to access this method
         */
        void checkout() {
            std::string keyword;
            std::getline(std::cin,keyword);
            keyword.erase(0,1);

            // Check if the item is in stock and that user isn't at the limit of items checked out
            if(m_library.count(keyword) == 0 || m_user->m_items.size() >= 3) {
                std::cout << "Error checking out item:\nItem doesn't exist or user has max number of items checked out\n";
                return;
            }
            Item* item = m_library[keyword];  //get item

            //ensure a copy of the item is available
            if(item->get_copies() <= 0) {
                std::cout << "Error: Item out of stock\n";
                return;
            }

            item->take_copy(); // decrement item's copy count
            m_user->m_items.push_back(item);
            std::cout << "Item checked out successfully\n";
        }


        /**
         * @author Andrew Martens
         *
         * @brief returns a library item based off a given keyword
         */
        void return_item() {
            // get keyword from user
            std::string keyword;
            std::getline(std::cin, keyword);
            keyword.erase(0,1); //removes space


            // ensure the user is returning an item they have checked out
            for(int i = 0; i < m_user->m_items.size(); i++) {
                Item* item = m_user->m_items[i];
                if(item->get_name() == keyword) {  // if they match add the copy back erase it
                    item->add_copy();
                    m_user->m_items.erase(m_user->m_items.begin()+i); // erase it from checked out
                    std::cout << "Returned '" << keyword << "'\n";
                    return;
                }
            }
            std::cout << "No user item titled '" << keyword << "'\n";
        }

        /**
         * @author Andrew Martens
         *
         * @brief adds an item to the library. If item is alreay in library option is given to add a copy
         * @note only users with admin access can access this functionality
         */ 
        void add_item() {
            // variables for the title and extra info
            int type;
            std::string title;
            std::string extra;

            //get item type
            std::cout << "To add book enter '1', film '2', and magazine '3' > ";

            //read into string to check type safety
            std::cin >> title; 
            try{
                type = std::stoi(title);
            } catch(std::invalid_argument) {
                std::cout << "Invalid Argument\n";
                return;
            }
            
            //skips to new line
            std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');

            // Get title from user
            std::cout << "Enter title > ";
            std::getline(std::cin, title);

            //validate title field
            if(!validate(title)) {
                std::cout << "Error: Title can't contain ';'\n";
                return;
            }

            //catch if library item already exists
            if(m_library.count(title) > 0) {
                std::cout << "Item already in inventory. Add a copy? [y/n]: "; //prompt user

                char ch;
                std::cin >> ch;
                if(ch != 'y') { //don't add copy and return
                    std::cout << "Copy not added\n";
                    return;
                }
                //add copy and return
                m_library[title]->add_copy();
                std::cout << "Copy added\n";
                return;
            }

    
            if(type == 1) {
                //get extra info from user
                std::cout << "Enter the author: ";
                std::getline(std::cin, extra);

                //validate author name
                if(!validate(extra)) {
                    std::cout << "Error: Author can't contain ';'\n";
                    return;
                }

                //construct Item, add to map, & write to file
                Item* item = new Book(title, extra);
                m_library[title] = item;
            }else if(type == 2) {
                //get extra info from user
                std::cout << "Enter the director: ";
                std::getline(std::cin, extra);
                
                if(!validate(extra)) { //validate director name
                    std::cout << "Error: Director can't contain ';'\n";
                    return;
                }

                //construct Item, add to map, & write to file
                Item* item = new Film(title, extra);;
                m_library[title] = item;
            }else if(type == 3) {
                //get extra info from user
                std::cout <<"Enter the publisher > ";
                std::getline(std::cin, extra);

                if(!validate(extra)) { //validate publisher name
                    std::cout << "Error: Publisher can't contain ';'\n";
                    return;
                }

                //construct Item, add to map, & write to file
                Item* item = new Magazine(title, extra);;
                m_library[title] = item;
            }else {
                std::cout << "Unknown library item type. Item not added\n";
            }
            std::cout << title << " added successfully\n";
        }

        /**
         * @author Andrew Martens
         *
         * @brief prompts for keyword then removes item from library if found
         * 
         */
        void remove_item() {
            //prompt user
            std::string keyword;
            std::getline(std::cin, keyword);
            keyword.erase(0,1); //removes leading space

            if(m_library.erase(keyword) == 1) { //erase from unordered map
                std::cout << "Removed '" << keyword << "'\n";
            } else{
                std::cout << "Item not found\n";
            }
        }



        /**
         * @author Andrew Martens
         *
         * @brief returns false if given string contains a semicolon
         */
        bool validate(std::string str) {
           //ensure no semicolon in library's name
           return !(str.find(';') != std::string::npos);
        }
        
    };
}

#endif