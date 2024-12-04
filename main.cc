/**
 * @author Andrew Martens & Rachel Roach
 *
 * This program contains the main method for running LibSys.
 * it prompts user for library name, validates the string, then runs LibSys::Library.run()
 * 
 */

#include <string>
#include <stdexcept>
#include <iostream>
#include "src/Items.hh"
#include "src/Users.hh"
#include "src/Library.hh"

int main() {

    // Header for opening showing the different libraries user can access
    std::cout << "\n-=Welcome to the Library Management System=-";
    std::cout << "\n--------------------------------------------\n";

    std::string lib_name;

    // prompt user for library name
    std::cout << "\nEnter the Library Name You're Using. Note: can not use whitespace > ";
    std::cin >> lib_name;

    //ensure no whitespace in library's name
    if(lib_name.find(' ') != std::string::npos) {
        throw std::invalid_argument("Error: Library name can't contain a space");
    }

    //create object and run
    LibSys::Library library(lib_name);
    library.run();
}