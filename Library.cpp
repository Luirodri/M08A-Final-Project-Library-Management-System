#include "Library.h"
#include "FileIO.h"
#include "LibraryExceptions.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
using namespace std;

static string lower(string s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return tolower(c); });
    return s;
}

static vector<string> splitCSV(const string& line, char delim = ','){
    vector<string> parts;
    string field;
    istringstream iss(line);
    while (getline(iss, field, delim)) parts.push_back(field);
    return parts;
}

static void printBookHeader(){
    cout << left << setw(32) << "Title" << setw(24) << "Author" << setw(12) << "Genre" << setw(12) << "Status" << "Format / Details" << endl;
    cout << string(90, '-') << endl;
}

Library::Library(string bf, string pf, string tf)
    : booksFile(move(bf)), patronsFile(move(pf)), transactionsFile(move(tf)){}

Library::~Library(){
    for (Book* b : books) delete b;
}

Book* Library::findBook(const string& title) const {
    string key = lower(title);
    for (Book* b : books)
        if (lower(b->getTitle()) == key) return b;
    throw BookNotFoundException(title);
}

Patron& Library::findPatron(int id) {
    auto it = patrons.find(id);
    if (it == patrons.end()) throw PatronNotFoundException(id);
    return it->second;
}
const Patron& Library::findPatron(int id) const{
    auto it = patrons.find(id);
    if (it == patrons.end()) throw PatronNotFoundException(id);
    return it->second;
}

static Book* parseBook(const string& line){
    vector<string> f = splitCSV(line);
    if (f.size()< 5) throw invalid_argument("Malformed book record: " + line);
    Genre g = stringToGenre(f[0]);
    Book* book = nullptr;
    if (f[3] == "Printed") book = new PrintedBook(f[1], f[2], g, stoi(f[4]));
    else if (f[3] == "EBook") book = new EBook(f[1], f[2], g, stod(f[4]));
    else throw invalid_argument("Unknown book format '" + f[3] + "'");
    if (f.size() >= 6 && f[5] == "CheckedOut") book->setStatus(BookStatus::CheckedOut);
    return book;
}

struct PatronRecord{
    int id;
    string name;
    vector<string> borrowed;
};

static PatronRecord parsePatron(const string& line){
    vector<string> f = splitCSV(line);
    if (f.size() < 2) throw invalid_argument("Malformed patron record: " + line);
    PatronRecord r;
    r.id = stoi(f[0]);
    r.name = f[1];
    if (f.size() >= 3) r.borrowed = splitCSV(f[2], '|');
    return r;
} 
void Library::loadData(){
    vector<Book*> loadedBooks= loadFromFile<Book*>(booksFile, parseBook);
    vector<PatronRecord> records;
    try{
        records = loadFromFile<PatronRecord>(patronsFile, parsePatron);
    } catch (...){
        for (Book* b : loadedBooks) delete b;
        throw;
    }
    for (Book* b : books) delete b;
    books = loadedBooks;
    patrons.clear();

    for (const PatronRecord& r : records){
        if (patrons.count(r.id)) throw DuplicateException("Duplicate patron ID in file: " + to_string(r.id));
        Patron p(r.name, r.id);
        for (const string& t : r.borrowed){
            if (t.empty()) continue;
            p.borrowBook(findBook(t));
        }
        patrons.emplace(r.id, p);
    }
    transactions = loadFromFile<Transaction>(transactionsFile, Transaction::fromCSV, false);
}

void Library::saveData(){
    saveToFile(booksFile, books, [](const Book* b){return b->getCSV(); });

    vector<Patron> list;
    for (const auto& entry : patrons) list.push_back(entry.second);
    saveToFile(patronsFile, list, [](const Patron& p){
        string line = to_string(p.getID()) + "," + p.getName();
        const auto& borrowed = p.getBorrowedBooks();
        if (!borrowed.empty()){
            line += ",";
            for (size_t i = 0; i < borrowed.size(); ++i){
                if (i) line += "|";
                line += borrowed[i]->getTitle();
            }
        }
        return line;
    });
    saveToFile(transactionsFile, transactions, [](const Transaction& t){ return t.getCSV(); });
}

void Library::addBook(Book* b){
    if (!b) throw invalid_argument("Cannot add a null book.");
    for (const Book* existing : books){
        if (*existing == *b){
            string title = b->getTitle();
            delete b;
            throw DuplicateException("\"" + title + "\" is already in the library.");
        }
    }
    books.push_back(b);
}

void Library::addPatron(const Patron& p){
    if (patrons.count(p.getID()))
    throw DuplicateException("A patron with ID " + to_string(p.getID())+ " already exists.");
    patrons.emplace(p.getID(), p);
}

void Library::checkoutBook(int patronID, string title){
    Patron& patron = findPatron(patronID);
    Book* book = findBook(title);
    if (book->getStatus() == BookStatus::CheckedOut) throw BookUnavailableException(book->getTitle());

    patron.borrowBook(book);
    book->setStatus(BookStatus::CheckedOut);
    transactions.emplace_back(patronID, book->getTitle(), "CHECKOUT");

    cout << patron.getName() << " checked out \"" << book->getTitle() << "\". Due: " << transactions.back().getDueDate() << endl;
}

void Library::returnBook(int patronID, string title){
    Patron& patron = findPatron(patronID);
    Book* book = findBook(title);

    patron.returnBook(book);
    book->setStatus(BookStatus::Available);
    transactions.emplace_back(patronID, book->getTitle(), "RETURN");

    cout << patron.getName() << " returned \"" << book->getTitle() << "\"." << endl;
}

void Library::displayBooks() const {
    cout << "\n=== Books (" << books.size() << ") ===" << endl;
    printBookHeader();
    for(const Book* b : books) b->displayInfo();
}

void Library::displayPatrons() const{
    cout << "\n=== Patrons (" << patrons.size() << ") ===" << endl;
    for (const auto& entry : patrons) entry.second.displayPatron();
}

void Library::displayTransactions() const{
    cout << "\n=== Transaction Log (" << transactions.size() << ") ===" << endl;
    if (transactions.empty()) cout << "(no transactions yet)" << endl;
    for (const Transaction& t : transactions) t.displayTransaction();
}

void Library::searchByAuthor(const string& author) const{
    string key = lower(author);
    int found = 0;
    cout << "\n=== Search by Author: \"" << author << "\" ===" << endl;
    for (auto it = books.begin(); it != books.end(); ++it){
        if (lower((*it)->getAuthor()).find(key) != string::npos){
            if (found == 0) printBookHeader();
            (*it)->displayInfo();
            ++found;
        }
    }
    if (found == 0) cout << "No books found for that author." << endl;
}

void Library::searchByGenre(const string & genre) const {
    string key = lower(genre);
    int found = 0;
    cout << "\n=== Search by genre: \"" << genre << "\" ===" << endl;
    for (auto it = books.begin(); it != books.end(); ++ it){
        if (lower(genreToString((*it)->getGenre())).find(key) != string::npos){
            if (found == 0) printBookHeader();
            (*it)->displayInfo();
            ++found;
        }
    }
    if (found == 0) cout << "No books found in that genre." << endl;
}

size_t Library::bookCount() const {return books.size();}
size_t Library::patronCount() const {return patrons.size();}