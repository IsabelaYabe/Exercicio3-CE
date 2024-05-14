
#ifndef TIME_HPP
#define TIME_HPP

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <optional>

class Time {
private:
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;

public:
    // Enum classes for comparison utilities
    enum class TimeMeasure { Year, Month, Day, Hour, Minute, Second };
    enum class CompareType { Less, LessOrEqual, Equal, Greater, GreaterOrEqual };

    // Constructor
    Time(int day, int month, int year, int hour = 0, int minute = 0, int second = 0);

    // Factory method from string
    static std::optional<Time> fromString(const std::string& dateTimeString);

    // Getters
    int getDay() const;
    int getMonth() const;
    int getYear() const;
    int getHour() const;
    int getMinute() const;
    int getSecond() const;

    // Setters
    void setDay(int newDay);
    void setMonth(int newMonth);
    void setYear(int newYear);
    void setHour(int newHour);
    void setMinute(int newMinute);
    void setSecond(int newSecond);

    // Utility methods
    void print() const;
    static bool compareTime(const Time& a, const Time& b, TimeMeasure measure, CompareType cmpType);

    // Overloaded comparison operators
    friend bool operator!=(const Time& lhs, const Time& rhs);
    friend bool operator==(const Time& lhs, const Time& rhs);
    friend bool operator<(const Time& lhs, const Time& rhs);
    friend bool operator>(const Time& lhs, const Time& rhs);
    friend bool operator<=(const Time& lhs, const Time& rhs);
    friend bool operator>=(const Time& lhs, const Time& rhs);
};

#endif // TIME_HPP