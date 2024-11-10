**Library Management System in C++**

A command-line-based Library Management System built in C++17, designed to manage books, members, and transactions in a library. This project allows users to add, remove, search for books, and handle borrowing/returning operations for library members.


**FEATURES**
 - add-item
 - remove-item
 - search
 - list all items
 - login
 - logout
 - view profile
 - checkout
 - return

**PREREQUISITES**

C++17 or later and GCC or Clang compiler with C++17 support

**INSTALLATION**

git clone https://github.com/your-username/library-management-system.git

cd library-management-system

g++ -std=c++17 -o library_system main.cpp

**RUNNING THE PROGRAM**

Upon running the program, the user should enter a library name, and the program will construct a directory structure to store information. The user can also enter "help" to see a list of commands.

NOTE: All commands and inputs are case-sensitive and searches for library items must match their corresponding title for that item to appear.
