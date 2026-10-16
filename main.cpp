#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include "Library.h"
#include "LibraryExceptions.h"
using namespace std;

static string readLine(const string& prompt){
    cout << prompt;
    string line;
    if (!getline(cin, line)) throw runtime_error("End of input reached.");
    return line;
}

static int readInt(const string& prompt){
    string line = readLine(prompt);
    size_t pos = 0;
    int value;
    try {
        value = stoi(line, &pos);
    } catch (const exception&) {
        throw invalid_argument("'" + line + "' is not a valid number.");
    }
    if (pos != line.size()) throw invalid_argument("'" + line + "' is not a valid number.");
    return value;
}

static string readText(const string& prompt) {
    string s = readLine(prompt);
    if (s.empty()) throw invalid_argument("Input cannot be empty.");
    if (s.find(',') != string::npos) throw invalid_argument("Commas are not allowed (files are comma-separated).");
    return s;
}

static void showMenu(){
    cout << "\n=======     LIBRARY MANAGEMENT SYSTEM     =======\n"
         << " 1 View books      2 View patrons      3 Checkout\n"
         << " 4 Return          5 Add book          6 Add patron\n"
         << " 7 Search Author   8 Search Genre      9 Transaction log\n"
         << " 0 Save and exit\n";
}

static void addBookFlow(Library& library) {
    string title    = readText("Title: ");
    string author   = readText("Author: ");
    Genre genre     = stringToGenre(readText("Genre (Fiction/NonFiction/Mystery/Science/Biography): "));
    string format   = readText("Format (Printed/EBook): ");

    Book* book = nullptr;
    if (format == "Printed")    book = new PrintedBook(title, author, genre, readInt("Page count: "));
    else if (format == "EBook") book = new EBook(title, author, genre, stod(readText("file size (MB): ")));
    else throw invalid_argument ("Format must be 'Printed' or 'EBook'.");

    library.addBook(book);
    cout << "Added: " << *book << endl;
}

static void addPatronFlow(Library& library){
    int id = readInt("Patron ID: ");
    string name = readText("Patron name: ");
    Patron p(name, id);
    library.addPatron(p);
    cout << "Added: " << p << endl;
}

int main (){
    Library library;

    try {
        library.loadData();
        cout << "Loaded " << library.bookCount() << " books and " << library.patronCount() << " patrons." << endl;
    } catch ( const exception& e){
        cerr << "Startup Error: " << e.what() << endl;
        return 1; 
    }

    bool running = true;
    while (running) {
        showMenu();
        try {
            int choice = readInt("Choose an option: ");
            switch (choice){
                case 1: library.displayBooks(); break;
                case 2: library.displayPatrons(); break;
                case 3: {
                    int id = readInt("Patron ID: ");
                    library.checkoutBook(id, readLine("Book title: "));
                    break;
                } 
                case 4: {
                    int id = readInt("Patron ID: ");
                    library.returnBook(id, readLine("Book title: "));
                    break;
                }
                case 5: addBookFlow(library); break;
                case 6: addPatronFlow(library); break;
                case 7: library.searchByAuthor(readText("Author(or part of name): ")); break;
                case 8: library.searchByGenre(readText("Genre: ")); break;
                case 9: library.displayTransactions(); break;
                case 0: library.saveData();
                        cout << "Data Saved. Goodbye!" << endl;
                        running = false;
                        break;
                default: throw invalid_argument("Menu option must be between 0 and 9.");
            }
        } catch (const runtime_error& e){
            if (string(e.what()) == "End of input reached."){
                cerr << "Input ended; saving and exiting." << endl;
                try {library.saveData(); } catch (...){} ; 
                return 0;
            }
            cerr << "Error: " << e.what() << endl;
        } catch (const exception& e){
                cerr << "Error: " << e.what() << endl;
        }
    }  
    return 0;  
}

