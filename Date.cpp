#include "Date.h"
#include <iomanip>
#include <sstream>
#include <stdexcept>
using namespace std;

static bool validDate(int y, int m, int d){
    if (y < 1900 || m < 1 || m > 12 || d < 1) {
        return false;
    }
        static const int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        int maxDays = days[m - 1];
        bool leapYear = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
        if (m == 2 && leapYear) {
            maxDays = 29;
        }
        return d >= 1 && d <= maxDays;
    }

    Date::Date(){
        time_t now = time(nullptr);
        tm* localTime = localtime(&now);
        day = localTime->tm_mday;
        month = localTime->tm_mon + 1;
        year = localTime->tm_year + 1900;
    }

    Date::Date(int y, int m, int d) : year(y), month(m), day(d) {
        if (!validDate(y, m, d)) {
            throw invalid_argument("Invalid date");
        }
    }

    Date Date::today() {
        return Date();
    }

    Date Date::fromString(const string& s) {
        istringstream iss(s);
        int y, m, d;
        char sep1, sep2;
        if (!(iss >> y >> sep1 >> m >> sep2 >> d) || sep1 != '-' || sep2 != '-' || !validDate(y, m, d)) {
            throw invalid_argument("Invalid date format");
        }
        return Date(y, m, d);
    }

    time_t Date::toTimeT() const {
        tm timeStruct = {};
        timeStruct.tm_year = year - 1900;
        timeStruct.tm_mon = month - 1;
        timeStruct.tm_mday = day;
        return mktime(&timeStruct);
    }

    string Date::toString() const {
        ostringstream oss;
        oss << setfill('0') << setw(4) << year << "-" << setw(2) << month << "-" << setw(2) << day;
        return oss.str();
    }

    Date Date::addDays(int n) const{
        static const int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
        Date r = *this;
        while (n-->0){
            int maxDays = days[r.month - 1];
            bool leap = (r.year % 4 == 0 && r.year % 100 != 0) || (r.year % 400 == 0);
            if (r.month == 2 && leap) maxDays = 29;
            if (++r.day > maxDays){
                r.day = 1;
                if (++r.month > 12){ r.month = 1; ++r.year;}
            }
        }
        return r;
    }

    bool Date::operator==(const Date& other) const {
        return year == other.year && month == other.month && day == other.day;
    }

    bool Date::operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }

    ostream& operator<<(ostream& os, const Date& d) {
        os << d.toString();
        return os;
    }