#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <iostream>
#include <string>
#include "Book.h"
#include "Patron.h"
#include "Date.h"
using namespace std;

class Transaction {
    private:
        int patronID;
        string bookTitle;
        string type;
        Date date;
        Date dueDate;

    public:
        Transaction(int pID, string bt, string t = "CHECKOUT");
        Transaction(int pID, string bt, string t, Date d, Date dueDate); 

        void displayTransaction() const;
        string getCSV() const;
        static Transaction fromCSV(const string& csvLine);

        int getPatronID() const;
        string getBookTitle() const;
        string getType() const;
        Date getDate() const;
        Date getDueDate() const;
};

#endif