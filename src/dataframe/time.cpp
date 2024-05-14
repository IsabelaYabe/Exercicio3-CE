#include "dataframe/time.hpp"

// Constructor
Time::Time(int day, int month, int year, int hour, int minute, int second)
    : day(day), month(month), year(year), hour(hour), minute(minute), second(second) {}

// Factory method to create Time object from string
std::optional<Time> Time::fromString(const std::string& dateTimeString) {
    std::istringstream iss(dateTimeString);
    char delim;
    int y, m, d, hr = 0, min = 0, sec = 0;

    if (dateTimeString.size() == 10) {
        iss >> y >> delim >> m >> delim >> d;
    } else if (dateTimeString.size() == 19) {
        iss >> y >> delim >> m >> delim >> d >> hr >> delim >> min >> delim >> sec;
    } else {
        return std::nullopt;
    }

    if (iss.fail()) {
        return std::nullopt;
    }
    return Time(d, m, y, hr, min, sec);
}

// Getters
int Time::getDay() const { return day; }
int Time::getMonth() const { return month; }
int Time::getYear() const { return year; }
int Time::getHour() const { return hour; }
int Time::getMinute() const { return minute; }
int Time::getSecond() const { return second; }

// Setters
void Time::setDay(int newDay) { day = newDay; }
void Time::setMonth(int newMonth) { month = newMonth; }
void Time::setYear(int newYear) { year = newYear; }
void Time::setHour(int newHour) { hour = newHour; }
void Time::setMinute(int newMinute) { minute = newMinute; }
void Time::setSecond(int newSecond) { second = newSecond; }

// Printing method
void Time::print() const {
    std::cout << std::setfill('0') << std::setw(2) << day << "-"
              << std::setfill('0') << std::setw(2) << month << "-"
              << year << " "
              << std::setfill('0') << std::setw(2) << hour << ":"
              << std::setfill('0') << std::setw(2) << minute << ":"
              << std::setfill('0') << std::setw(2) << second << std::endl;
}

bool operator!=(const Time& lhs, const Time& rhs) { return !(lhs == rhs); }
bool operator==(const Time& lhs, const Time& rhs) {
    return lhs.day == rhs.day && lhs.month == rhs.month && lhs.year == rhs.year &&
           lhs.hour == rhs.hour && lhs.minute == rhs.minute && lhs.second == rhs.second;
}
bool operator<(const Time& lhs, const Time& rhs) {
    if (lhs.year != rhs.year) return lhs.year < rhs.year;
    if (lhs.month != rhs.month) return lhs.month < rhs.month;
    if (lhs.day != rhs.day) return lhs.day < rhs.day;
    if (lhs.hour != rhs.hour) return lhs.hour < rhs.hour;
    if (lhs.minute != rhs.minute) return lhs.minute < rhs.minute;
    return lhs.second < rhs.second;
}
bool operator>(const Time& lhs, const Time& rhs) { return rhs < lhs; }
bool operator<=(const Time& lhs, const Time& rhs) { return !(rhs < lhs); }
bool operator>=(const Time& lhs, const Time& rhs) { return !(lhs < rhs); }