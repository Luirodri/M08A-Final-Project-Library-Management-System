#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <string>
using namespace std;

enum class Genre { Fiction, NonFiction, Mystery, Science, Biography};
enum class BookStatus {Available, CheckedOut};

string genreToString(Genre g);
Genre stringToGenre(const string& s);
string statusToString(BookStatus s);

class Book {
    protected:
        string title;
        string author;
        Genre genre;
        BookStatus status;

    public:
        Book(string t, string a, Genre g);
        virtual ~Book() = default;

        virtual void displayInfo() const;
        virtual string getType() const;
        virtual string getExtra() const;
        virtual string getCSV() const;

        void setStatus(BookStatus s);
        BookStatus getStatus() const;
        string getTitle() const;
        string getAuthor() const;
        Genre getGenre() const;

        bool operator==(const Book& other) const;
        friend ostream& operator<<(ostream& os, const Book& b);
};

class EBook : public Book {
    private:
        double fileSizeMB;

    public:
        EBook(string t, string a, Genre g, double size);
        void displayInfo() const override;
        string getType() const override;
        string getExtra() const override;
};

class PrintedBook : public Book {
    private:
        int numPages;

    public:
        PrintedBook(string t, string a, Genre g, int pages);
        void displayInfo() const override;
        string getType() const override;
        string getExtra() const override;
};

#endif