#ifndef PATRON_H
#define PATRON_H

#include <iostream>
#include <string>
#include <vector>
#include "Book.h"
using namespace std;

class Patron {
    private:
        string name;
        int id;
        vector<Book*> borrowedBooks;

    public:
        static const int MAX_BORROWED_BOOKS = 5;
        Patron(string n, int id);

        void borrowBook(Book* book);
        void returnBook(Book* book);
        void displayPatron() const;
        bool hasBook(const Book* book) const;

        int getID() const;
        string getName() const;
        const vector<Book*>& getBorrowedBooks() const;

        bool operator==(const Patron& other) const;
        friend ostream& operator<<(ostream& os, const Patron& p);
};

#endif