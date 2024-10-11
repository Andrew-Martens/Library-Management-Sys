
#include "Items.hh"
#include "Users.hh"
#include<iostream>



int main() {
    Library::Book book = Library::Book("Red Mars", "Kim Stanley Robinson");
    Library::Magazine mag = Library::Magazine("Vogue", "Publisher");
    Library::Film movie = Library::Film("Toy Story", "John Lasseter");

    std::cout << book.to_string() << "\n" << "\n";
    std::cout << mag.to_string() << "\n" << "\n";
    std::cout << movie.to_string() << "\n" << "\n";


}

