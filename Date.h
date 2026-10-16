#ifndef DATE_h
#define DATE_h

#include <iostream>
#include <string>
#include <ctime>
using namespace std;

class Date{
    private:
        int year;
        int month;
        int day;

    public:
        Date();
        Date(int y, int m, int d);
        
        static Date today();
        static Date fromString(const string& s);

        Date addDays(int days) const;
        string toString() const;
        time_t toTimeT() const;

        bool operator==(const Date& other) const;
        bool operator<(const Date& other) const;
        friend ostream& operator<<(ostream& os, const Date& d);
};

#endif