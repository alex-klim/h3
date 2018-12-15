#include <iostream>

enum class Month: std::uint8_t { January = 1, February, March, April, May, June, \
                July, August, September, October, November, December};

enum class Format: int { ddmmyyyy = 1, mmddyyyy, yyyymmdd };

enum class Separator: char { stroke = '/', dot = '.', hyphen = '-', space = ' '};

static constexpr std::uint8_t daysInMonth[13] = {
    0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static constexpr size_t daysSinceYearStarted[13] = {
    0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
};

class Date
{
public:
    Date() = default;
    Date(std::string&);
    Date(const char*);
    int grepAndSetDate(std::string, Format);
    int setDate(int day,
            Month month,
            int year,
            Separator s,
            Format fmt);
    bool isYearLeap(int) const;
    bool isValid(int day,
            Month month,
            int year,
            Separator s,
            Format fmt) const;
    std::string format() const;
    void applyDelta(int);
    void setFormat(const Format&);
    int operator- (const Date&);
    friend std::istream& operator>> (std::istream&, Date&);
    friend std::ostream& operator<< (std::ostream&, const Date&);
private:
    size_t countDays(); // count total_days_ for current date
    void countDate(); // cout date from total_days_

private:
    int day_;
    Month month_;
    int year_;
    Separator s_;
    Format format_;
    bool valid_;
    size_t total_days_;
};
