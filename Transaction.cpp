#include "Transaction.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <vector>
using namespace std;

static const int LOAN_DAYS = 14;

Transaction::Transaction(int pID, string bt, string t)
    : patronID(pID), bookTitle(move(bt)), type(move(t)), date(Date::today()), dueDate(date.addDays(LOAN_DAYS)) {}

Transaction::Transaction(int pID, string bt, string t, Date d, Date due)
    : patronID(pID), bookTitle(move(bt)), type(move(t)), date(d), dueDate(due) {}

void Transaction::displayTransaction() const{
    string title = bookTitle;
    if (title.length() > 32) title = title.substr(0,29) + "...";
    
    cout << left << setw(10) << type << "Patron: " << setw(9) << patronID << setw(34) << bookTitle << "Date: " << date;
    if (type == "CHECKOUT") {
        cout << " Due: " << dueDate;
    }
    cout << endl;
}

string Transaction::getCSV() const{
    return to_string(patronID) + "," + bookTitle + "," + type + "," + date.toString() + "," + dueDate.toString();
}

Transaction Transaction::fromCSV(const string& line){
    vector<string> f;
    string field;
    istringstream iss(line);
    while (getline(iss, field, ',')) {
        f.push_back(field);
    }
    if (f.size() != 5) {
        throw invalid_argument("Invalid CSV line for Transaction: " + line);
    }
    return Transaction(stoi(f[0]), f[1], f[2], Date::fromString(f[3]), Date::fromString(f[4]));
}

int Transaction::getPatronID() const{ return patronID;}
string Transaction::getBookTitle() const{ return bookTitle;}
string Transaction::getType() const{ return type;}
Date Transaction::getDate() const{ return date;}
Date Transaction::getDueDate() const{return dueDate;}
