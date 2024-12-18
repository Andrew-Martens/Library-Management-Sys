/**
* @author Andrew Martens & Rachel Roach
*
* This file contains the high level functionallity for the library system
* to use create Library object and call run()
*/

#ifndef LIBRARY_HH__
#define LIBRARY_HH__

#include<string>
#include<iostream>
#include<vector>
#include<filesystem>
#include<fstream>
#include<algorithm>
#include<regex>
#include<unordered_map>
#include<algorithm>
#include "Users.hh"
#include "Items.hh"

namespace LibSys {

    /**
     * @author Andrew Martens & Rachel Roach
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
        std::string m_password;

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
         */
        Library(std::string name) : m_library_name(name), m_user(nullptr), m_username("Guest"), m_password("0"),
                                    u_filename(".libraries/"+name+"/users.txt"), i_filename(".libraries/"+name+"/inventory.txt") {

            // Create directory structure to store library information
            std::filesystem::create_directory(".libraries");
            std::filesystem::path lib_path(".libraries/" + name);
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
         * @author Andrew Martens & Rachel Roach
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
                    std::cout << "\nSystem terminating...\n\n";
                    return;
                } else if (command=="help") {
                    help(); 
                } else if (command=="login") {
                    if(m_user) {
                        continue;
                    }
                    login();
                } else if (command == "create-account") {
                    createAccount();
                } else if (command == "delete-account") {
                    deleteAccount();
                } else if (command=="search") {
                    //get keyword from user
                    std::cout << "\n";
                    std::string keyword;
                    std::getline(std::cin, keyword);
                    keyword.erase(0,1); //removes the first char since it picks up a space

                    if(search(keyword) == 0) {
                        std::cout << "\n'" << keyword << "' not found\n";
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
                        m_password = "0";
                    }
                    std::cout << "\nLogging Out...\n\n";
                } else if(command=="profile"){
                    if(m_user) {
                        std::cout << "\n";
                        m_user->print();
                        std::cout << "\n";
                    }
                } else if (command=="checkout") {
                    //ensure user is logged in
                    if(!m_user){
                        std::cout << "\nMust login to checkout item\n\n";
                        continue;
                    }
                    checkout();
                } else if (command=="return-item"){
                    //ensure user is logged in
                    if(!m_user){
                        std::cout << "\nMust login to return item\n\n";
                        continue;
                    }
                    return_item();
                } else if(command=="add-item") {
                    //ensure user is logged in to an admin account
                    if(!m_user || m_user->m_id < 200000){
                        std::cout << "\nOnly librarians can access this command\n\n";
                        continue;
                    } else {add_item();}
                } else if(command=="remove-item") {
                    //ensure user is logged in to an admin account
                    if(!m_user || m_user->m_id < 200000){
                        std::cout << "\nOnly librarians can access this command\n\n";
                        continue;
                    }
                    remove_item();
                } else {
                    if(!m_user) {
                        std::cout << "\nUnknown Command\n\n";
                    } else {
                        std::cout << "\nCommand not available for logged in users\n\n";
                    }
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
         * @author Andrew Martens & Rachel Roach
         *
         * @brief writes user information to file
         *
         * To avoid file fragmentation, every user except for the logged in user
         * is coppied from users.txt to a temp file that replaces users.txt.
         */
        void write_users() {
            //input file from users.txt and create a temp file to write to
            std::string t_filename = ".libraries/"+ m_library_name +"/temp.txt";
            std::ifstream ifs(u_filename);
            std::ofstream temp(t_filename);

            //write logged in user to file to save any changes
            temp << m_user->m_name << ";" << m_user->m_password << ";" << m_user->m_id;
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
         * @author Rachel Roach
         * 
         * @brief prints a list of commands to the console
         */
        void help() {
            std::cout << "\nLIST OF COMMANDS:";
            std::cout << "\n-------------------------------------------------";
            std::cout << "\n1.  help\t\t2.  search <name>\n3.  all-items\t\t4.  login <username>\n5.  logout\t\t6.  profile\n";
            std::cout << "7.  checkout <title>\t8.  return-item <title>\n9.  add-item\t\t10. remove-item <title>\n11. exit\t\t12. create-account\n13. delete-account";
            std::cout << "\n-------------------------------------------------\n\n";
        }

        /**
        * @author Rachel Roach
        *
        * @brief creates a user account and stores it in a file, provides the 
        * user with different account options available to them
        */
        void createAccount() {
            
            // User variables
            std::string username;
            std::string password;
            int id;

            std::cout << "\nWould you like to make an account [y/n]: ";
                char ch;
                std::cin >> ch;

                if(ch == 'y') {
                    std::ofstream ofs(u_filename, std::ios::app);
                    if (!ofs) {
                        std::cout << "Error: Unable to open user file for writing.\n";
                        return;
                    }

                    //make id for user and write new user into <library_name>/users.txt
                    std::cout << "\nTo create librarian account enter 'l', else enter 'm' > ";
                    std::cin >> ch;

                    if(ch == 'l') {
                        //construct user with admin permissions and print
                        std::cout << "\nEnter a username: ";
                        std::cin >> username;
                        std::cout << "Enter a password: ";
                        std::cin >> password;
                        id = create_id(true);
                        m_user = new User(username, password, id);
                        m_username = m_user->m_name;
                    } else if(ch == 'm') {
                        // construct user without admid permissions and print
                        std::cout << "Enter a username: ";
                        std::cin >> username;
                        std::cout << "Enter a password: ";
                        std::cin >> password;
                        id = create_id(false);
                        m_user = new User(username, password, id);
                        m_username = m_user->m_name;
                    } else { 
                        std::cout << "\nCreating empty account w/ no permissions...\n";
                        //construct user without admin permissions and print
                        id = create_id(false);
                        m_user = new User(username, password, id);
                        m_username = m_user->m_name;
                    }
                    //print conformation
                    std::cout << "\nCreated new user:\n";
                    std::cout << "-----------------\n";
                    m_user->print();
                    std::cout << "\n";
                } else if (ch == 'n') {
                    std::cout << "\nReturning to Guest user...\n\n";
                }  else {
                    std::cout << "\nReturning to Guest user...\n";
                }
        }

        /**
        * @author Rachel Roach
        *
        * @brief Deletes the currently logged-in user account after password verification.
        * Prevents account deletion if the user has checked-out items and transitions to a guest user if successful.
        */
        void deleteAccount() {

            // Check if a user is logged in
            if (!m_user) {
                std::cout << "\nYou must be logged in to delete an account.\n\n";
                return;
            }

            // Local variables
            std::string username = m_user->m_name;
            std::string storedPassword;
            std::string enteredPassword;
            std::string checkedOutItem;
            std::string tempFilename = "temp_users.txt";
            std::ifstream ifs(u_filename);
            std::ofstream ofs(tempFilename, std::ios::out);
            std::string line;
            bool userFound = false;
            bool hasCheckedOutItems = false;

            // throws an error if file cannot be opened
            if (!ifs || !ofs) {
                std::cout << "\nError: Unable to open file.\n\n";
                return;
            }

            // Process the user file line by line
            while (std::getline(ifs, line)) {
                std::stringstream ss(line);
                std::string fileUsername, filePassword, fileCheckedOutItem;
                int fileId;

                // Parse the line in the file in the format: 
                // "username;password;id;checkedOutItem"
                if (std::getline(ss, fileUsername, ';') &&
                    std::getline(ss, filePassword, ';') &&
                    ss >> fileId) {
                    // Ensure that we clean up any extra whitespace between the fields
                    std::getline(ss, fileCheckedOutItem);

                    // if the username is found in the file...
                    if (fileUsername == username) {
                        userFound = true;
                        storedPassword = filePassword;
                        checkedOutItem = fileCheckedOutItem;

                        // Check if the user has checked-out items
                        if (!checkedOutItem.empty()) {
                            hasCheckedOutItems = true;
                            // Replace semicolons with spaces in the checked-out item string -- for print statement
                            std::replace(checkedOutItem.begin(), checkedOutItem.end(), ';', ' ');
                            std::cout << "\nError: Cannot delete account due to outstanding items:" << checkedOutItem << "\n\n";
                            ofs << line << "\n";
                            break;
                        }

                        // If no checked-out items, proceed to password verification
                        std::cout << "\nEnter password to confirm deletion: ";
                        std::cin >> enteredPassword;

                        // If the password doesn't match the accounts fail verification
                        if (storedPassword != enteredPassword) {
                            std::cout << "\nPassword verification failed. Account not deleted...\n\n";
                            ofs << line << "\n";
                            break;
                        }

                        // Confirm deletion print statement, return to Guest
                        std::cout << "\n------------------------------------";
                        std::cout << "\nAccount '" << username << "' has been deleted.";
                        std::cout << "\n------------------------------------\n";
                        continue; 
                    }
                    // Write back all other accounts
                    ofs << line << "\n";
                }
            }

            // Error statement for when account isn't found in file
            if (!userFound) {
                std::cout << "\nError: No matching account found in the file.\n\n";
            }

            // Close the files
            ifs.close();
            ofs.close();

            // Replace the original file with the temporary file
            if (userFound && !hasCheckedOutItems && storedPassword == enteredPassword) {
                if (std::remove(u_filename.c_str()) != 0 || std::rename(tempFilename.c_str(), u_filename.c_str()) != 0) {
                    std::cout << "Error: Unable to update user file.\n";
                } else {
                    // Log out the user and switch to guest user mode
                    delete m_user;
                    m_user = nullptr;
                    m_username = "Guest";
                    m_password = "0";
                    std::cout << "\nYou are now logged in as a guest user.\n\n";
                }
            } else {
                // Clean up the temporary file if no user was deleted
                std::remove(tempFilename.c_str());
            }
        }

        /**
         * @author Andrew Martens & Rachel Roach
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
            std::string password;
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
                if (tokens.size() > 1 && tokens[0] == username) {
                    // Prompt for password
                    std::cout << "\nEnter password: ";
                    std::getline(std::cin, password);

                    // Verify password
                    if (tokens[1] == password) {
                        // Convert ID to integer
                        try {
                            id = std::stoi(tokens[2]);
                        } catch (std::invalid_argument) {
                            std::cout << "Error Logging in: File format error\n";
                            return;
                        }

                        // Construct user object
                        m_user = new User(username, password, id);
                        m_username = username;

                        // Populate user's items
                        for (size_t i = 3; i < tokens.size(); i++) {
                            std::string item = tokens[i];
                            m_user->m_items.push_back(m_library[item]);
                        }

                        std::cout << "\nLog In Successful:\n";
                        m_user->print(); // Print user info
                        std::cout << "\n";
                        ifs.close();
                        return;
                    } else {
                        // Password mismatch
                        std::cout << "\nIncorrect Password. Returning as Guest user...\n";
                        ifs.close();
                        return;
                    }
                }
                    }
                    //if no account was found close input file and ask user if they would like to make an account
                    ifs.close();
                    std::cout << "\nAccount not found\n";
                    createAccount();
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
         * @returns number of items printed
         */
        int search(std::string keyword) {
            std::regex pattern(tolower(keyword) + ".*");
            int count;

            for(const auto& entry : m_library) {
                std::string title = entry.first;
                title = tolower(title);
                std::string contributer;

                Item* item = entry.second;
                int id = item->get_id();
                switch(id) {
                    case 1:
                        contributer = tolower(dynamic_cast<Book*>(item)->get_author());
                        if(std::regex_match(contributer, pattern) || std::regex_match(title, pattern)) {
                            dynamic_cast<Book*>(item)->print();
                            count++;
                        }
                        break;
                    case 2: 
                        contributer = tolower(dynamic_cast<Film*>(item)->get_director());
                        if(std::regex_match(contributer, pattern) || std::regex_match(title, pattern)) {
                            dynamic_cast<Film*>(item)->print();
                            count++;
                        }
                        break;
                    case 3: 
                        contributer = tolower(dynamic_cast<Magazine*>(item)->get_publisher());
                        if(std::regex_match(contributer, pattern) || std::regex_match(title, pattern)) {
                            dynamic_cast<Magazine*>(item)->print();
                            count++;
                        }
                        break;
                }
                std::cout << "\n"; //formating

            }
            return count;
        }

        /**
         * @author Andrew Martens
         * @brief converts a string to lowercase
         * @note helper function for @see search(std::string)
         */
        std::string tolower(std::string s) {
            //transforms each char to lowercase
            std::transform(s.begin(), s.end(), s.begin(),
                        [](unsigned char c){ return std::tolower(c); }
                        );
            return s;
        }

        /**
         * @author Andrew Martens & Rachel Roach
         * 
         * @brief prints all library items
         */
        void print_items() {
            std::cout << "\nListing all items:\n";
            std::cout << "-------------------\n\n";
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
            std::cout << "\n-----------------------------";
            std::cout << "\nItem checked out successfully";
            std::cout << "\n-----------------------------\n\n";
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
                    std::cout << "\n------------------------\n";
                    std::cout << "Returned '" << keyword << "'";
                    std::cout << "\n------------------------\n\n";
                    return;
                }
            }
            std::cout << "No user item titled '" << keyword << "'\n";
        }

        /**
         * @author Andrew Martens & Rachel Roach
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
            std::cout << "\nOptions:";
            std::cout << "\n-----------------";
            std::cout << "\n1. Book";
            std::cout << "\n2. Movie";
            std::cout << "\n3. Magazine";
            std::cout << "\n-----------------";
            std::cout << "\n\nType of Item You're Adding (Enter Number) > ";

            //read into string to check type safety
            std::cin >> title; 
            try{
                type = std::stoi(title);
            } catch(std::invalid_argument) {
                std::cout << "\nInvalid Argument. No Longer adding item...\n\n";
                return;
            }
            
            //skips to new line
            std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');

            // Get title from user
            std::cout << "\nEnter title: ";
            std::getline(std::cin, title);

            //validate title field
            if(!validate(title)) {
                std::cout << "Error: Title can't contain ';'\n";
                return;
            }

            //catch if library item already exists
            if(m_library.count(title) > 0) {
                std::cout << "\nItem already in inventory. Add a copy? [y/n]: "; //prompt user

                char ch;
                std::cin >> ch;
                if(ch != 'y') { //don't add copy and return
                    std::cout << "\n--------------\nCopy not added\n--------------\n\n";
                    return;
                }
                //add copy and return
                m_library[title]->add_copy();
                std::cout << "\n----------\nCopy added\n----------\n\n";
                return;
            }

            // for BOOK
            if(type == 1) {
                // Gets the name of the author
                std::cout << "Enter the author: ";
                std::getline(std::cin, extra);
                // Validates the name
                if(!validate(extra)) {
                    std::cout << "Error: Author can't contain ';'\n";
                    return;
                }
                // construct Item, add to map, & write to file
                Item* item = new Book(title, extra);
                m_library[title] = item;
            }

            // for MOVIE
            else if(type == 2) {
                //get extra info from user
                std::cout << "Enter the director: ";
                std::getline(std::cin, extra);
                // Validates name
                if(!validate(extra)) {
                    std::cout << "Error: Director can't contain ';'\n";
                    return;
                }
                //construct Item, add to map, & write to file
                Item* item = new Film(title, extra);;
                m_library[title] = item;
            }

            // for MAGAZINE
            else if(type == 3) {
                // Gets publisher name
                std::cout <<"Enter the publisher: ";
                std::getline(std::cin, extra);
                // Validates name
                if(!validate(extra)) { 
                    std::cout << "Error: Publisher can't contain ';'\n";
                    return;
                }
                //construct Item, add to map, & write to file
                Item* item = new Magazine(title, extra);;
                m_library[title] = item;
            }else {
                std::cout << "\nUnknown library item type. Item not added\n\n";
            }
            std::cout << "\n" << title << " added successfully\n\n";
        }

        /**
         * @author Andrew Martens & Rachel Roach
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
                std::cout << "\n----------------------------\n";
                std::cout << "Removed '" << keyword << "'\n";
                std::cout << "----------------------------\n\n";
            } else{
                std::cout << "\nItem not found\n\n";
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