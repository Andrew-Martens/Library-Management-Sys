
#include "Items.hh"
#include "Users.hh"
#include<iostream>
#include<string>



int main() {

    Library::Book book = Library::Book("Red Mars", "Kim Stanley Robinson");
    Library::Magazine mag = Library::Magazine("Vogue", "Publisher");
    Library::Film movie = Library::Film("Toy Story", "John Lasseter");

    book.print();
    mag.print();
    movie.print();

    Library::Librarian betsy = Library::Librarian("betsy");
    Library::Book* book_ptr = &book;
    Library::Magazine* mag_ptr = &mag;
    Library::Film* movie_ptr = &movie;
    betsy.add(book_ptr);
    betsy.add(mag_ptr);
    betsy.add(movie_ptr);

    betsy.print_items();

    betsy.remove("Toy Story");

    betsy.print_items();

    Library::User user;
    user.search(" ");
    user.search("Red Mars");


}

