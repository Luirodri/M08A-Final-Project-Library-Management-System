#include "Book.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>
using namespace std;

string genreToString(Genre g) {
    switch (g) {
        case Genre::Fiction: return "Fiction";
        case Genre::NonFiction: return "Non-Fiction";
        case Genre::Mystery: return "Mystery";
        case Genre::Science: return "Science";
        case Genre::Biography: return "Biography";
    }
    return "Unknown";
}

Genre stringToGenre(const string& s) {
    if (s == "Fiction") return Genre::Fiction;
    if (s == "NonFiction") return Genre::NonFiction;
    if (s == "Mystery") return Genre::Mystery;
    if (s == "Science") return Genre::Science;
    if (s == "Biography") return Genre::Biography;
    throw invalid_argument("Unknown genre: " + s);
}

string statusToString(BookStatus s){
    return (s == BookStatus::Available) ? "Available" : "CheckedOut";
}

Book::Book(string t, string a, Genre g)
    : title(move(t)), author(move(a)), genre(g), status(BookStatus::Available) {};

void Book::displayInfo() const{
    cout << left << setw(32) << title << setw(24) << author << setw(12) << genreToString(genre) << setw(12) << statusToString(status) << endl;
}

string Book::getType() const{
    return "Book";
}
string Book::getExtra() const{
    return "";
}

string Book::getCSV() const {
    return genreToString(genre) + "," + title + "," + author + "," + getType() + "," + getExtra()+ "," + statusToString(status);
}

void Book::setStatus(BookStatus s){
    status = s;
}
BookStatus Book::getStatus() const{
    return status;
}
string Book::getTitle() const{
    return title;
}
string Book::getAuthor() const{
    return author;
}
Genre Book::getGenre() const{
    return genre;
}

bool Book::operator==(const Book& other) const{
    return title == other.title && author == other.author;
}

ostream& operator<<(ostream& os, const Book& b){
    os << left << setw(32) << b.title << setw(24) << b.author << setw(12) << genreToString(b.genre) << setw(12) << statusToString(b.status);
    return os;
}

EBook::EBook(string t, string a, Genre g, double size)
    : Book(move(t), move(a), g), fileSizeMB(size) {
        if (size <= 0) {
            throw invalid_argument("File size must be positive");
        }
    }

void EBook::displayInfo() const{
    Book::displayInfo();
    cout << "EBook Size: " << fixed << setprecision(1) << fileSizeMB << " MB" << endl;
}

string EBook::getType() const{
    return "EBook";
}

string EBook::getExtra() const{
    ostringstream oss;
    oss << fixed << setprecision(1) << fileSizeMB;
    return oss.str();
}

PrintedBook::PrintedBook(string t, string a, Genre g, int pages)
    : Book(move(t), move(a), g), numPages(pages) {
        if (pages <= 0) {
            throw invalid_argument("Number of pages must be positive");
        }
    }
void PrintedBook::displayInfo() const{
    Book::displayInfo();
    cout << "Printed Book Pages: " << numPages << endl;
}

string PrintedBook::getType() const{
    return "Printed";
}
string PrintedBook::getExtra() const{
    return to_string(numPages);
}
