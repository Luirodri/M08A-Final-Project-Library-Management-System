#include "Patron.h"
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <sstream>
using namespace std;

Patron::Patron(string n, int i) : name(move(n)), id(i) {
    if (id <= 0) throw invalid_argument("Invalid patron ID: must be a positive number");
    if (name.empty()) throw invalid_argument("Patron name cannot be empty");
}

void Patron::borrowBook(Book* book){
    if (borrowedBooks.size() >= MAX_BORROWED_BOOKS)
        throw runtime_error("Cannot borrow more than " + to_string(MAX_BORROWED_BOOKS) + " books");
    borrowedBooks.push_back(book);
}

void Patron::returnBook(Book* book){
    auto it = find(borrowedBooks.begin(), borrowedBooks.end(), book);
    if (it == borrowedBooks.end())
        throw runtime_error(name + " has not borrowed this book");
    borrowedBooks.erase(it);
}

bool Patron::hasBook(const Book* book) const{
    return find(borrowedBooks.begin(), borrowedBooks.end(), book) != borrowedBooks.end();
}

void Patron::displayPatron() const{
    cout << left << setw(6) << id << setw(20) << name << "Books out: " << borrowedBooks.size();
    if (!borrowedBooks.empty()){
        cout << " (";
        for (size_t i = 0; i < borrowedBooks.size(); ++i){
            if (i) cout << ", ";
            cout << borrowedBooks[i]->getTitle();
        }
        cout << ")";
    }
    cout << endl;
}

int Patron::getID() const{
    return id;
}
string Patron::getName() const{
    return name;
}
const vector<Book*>& Patron::getBorrowedBooks() const{
    return borrowedBooks;
}

bool Patron::operator==(const Patron& other) const{
    return id == other.id;
}

ostream& operator<<(ostream& os, const Patron& p){
    os << "Patron #" << p.id << ": " << p.name << " (" << p.borrowedBooks.size() << "book(s) borrowed)";
    return os;
}