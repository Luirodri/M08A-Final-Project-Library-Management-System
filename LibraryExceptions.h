#ifndef LIBRARYEXCEPTIONS_H
#define LIBRARYEXCEPTIONS_H

#include <stdexcept>
#include <string>
using namespace std;

class BookNotFoundException : public runtime_error {
    public:
        explicit BookNotFoundException(const string& title)
            : runtime_error("Book not found: " + title) {}
};

class PatronNotFoundException : public runtime_error {
    public:
        explicit PatronNotFoundException(int id)
            : runtime_error("Patron not found: " + to_string(id)) {}
};

class BookUnavailableException : public runtime_error {
    public:
        explicit BookUnavailableException(const string& title)
            : runtime_error("Book unavailable: " + title) {}
};

class DuplicateException : public runtime_error {
    public:
        explicit DuplicateException(const string& what)
            : runtime_error("Duplicate entry: " + what) {}
};

#endif