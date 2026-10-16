#ifndef LIBRARY_H
#define LIBRARY_H

#include <map>
#include <string>
#include <vector>
#include "Book.h"
#include "Patron.h"
#include "Transaction.h"
using namespace std;

class Library{
private:
    vector<Book*> books;
    map<int, Patron> patrons;
    vector<Transaction> transactions;

    string booksFile;
    string patronsFile;
    string transactionsFile;

    Book* findBook(const string& title) const;
    Patron& findPatron(int id);
    const Patron& findPatron(int id) const;

public:
    Library(string bf = "books.txt", string pf = "patrons.txt", string tf = "transactions.txt");
    ~Library();
    Library(const Library&) = delete;
    Library& operator=(const Library&) = delete;

    void loadData();
    void saveData();

    void addBook(Book* b);
    void addPatron(const Patron& p);
    void checkoutBook(int patronID, string title);
    void returnBook(int patronID, string title);

    void displayBooks() const;
    void displayPatrons() const;
    void displayTransactions() const;

    void searchByAuthor(const string& author) const;
    void searchByGenre(const string& genre) const;

    size_t bookCount() const;
    size_t patronCount() const;
};

#endif