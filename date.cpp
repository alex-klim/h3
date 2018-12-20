#include "date.hpp"

#include <regex>
#include <iostream>
#include <limits>

Date::Date()
    : day_(1), month_(Month::January), year_(1),
    s_(Separator::dot), format_(Format::ddmmyyyy),
    valid_(false), total_days_(0) {}

void Date::applyDelta(int days) {
    if (!valid_) {
        std::cerr << "applyDelta canceled. Trying to operate on malformed date.\n";
        return;
    }
    int temp = total_days_ + days;
    if (temp < 0) {
        std::cerr << "Operation is not permitted."
            << "This class operates only dates after 01.01.0001\n";
        return;
    }
    total_days_ = temp;
    countDate();
}

int Date::operator- (const Date& dt) const {
    if (!valid_ or !dt.valid_) {
        std::cerr << "Cant calculate date difference. Either one or both dates are invalid.\n";
        return 0;
    }
    std::cout << "first: " << total_days_ << "\nsecond: " << dt.total_days_ << '\n';
    return total_days_ - dt.total_days_;
}

size_t Date::countDays() {
    size_t total_days = day_;
    total_days += daysSinceYearStarted[static_cast<int>(month_)];
    if (isYearLeap(year_) && static_cast<int>(month_) > 2)
        total_days++;
    total_days += year_ * 365 + (year_ / 4) - (year_ / 100) + (year_ / 400);
    return total_days - 366; // subtracting first year since counting starts from 01.01.0001
}

void Date::countDate() {
    year_ = total_days_ / 365;
    int temp = total_days_ - year_ * 365 - (year_ / 4 - year_ / 100 + year_ / 400);

    while (temp < 0) {
        temp += 365;
        if (isYearLeap(year_))
            temp++;
        year_--;
    }
    year_++;

    int imonth = 1;
    while (temp > daysInMonth[imonth]) {
        temp -= daysInMonth[imonth];
        if (imonth == 2 && isYearLeap(year_))
            temp=-1;
        imonth++;
    }
    if (imonth == 3 && temp == 1 && isYearLeap(year_)){
        month_ = static_cast<Month>(imonth-1);
        day_ = 1;
    } else {
        month_ = static_cast<Month>(imonth);
        day_ = temp+1;
    }
    valid_ = isValid();
}

bool Date::isYearLeap(int year) const {
    if (year % 4 == 0) {
        if (year % 100 == 0) {
            if (year % 400 == 0) {
                return true; // leap
            } else {
                return false; // not leap
            }
        } else {
            return true; //leap
        }
    } else {
        return false; // not leap
    }
}

bool Date::isValid() const {
    if (day_ < 1 || year_ < 1 || static_cast<int>(month_) < 1 || static_cast<int>(month_) > 12) {
        return 0;
    }

    if (month_ != Month::February && day_ > daysInMonth[static_cast<int>(month_)]) {
        return 0;
    }
    if (month_ == Month::February
            && ((isYearLeap(year_) && day_ > 29) || (!isYearLeap(year_) && day_ > 28))) {
        return 0;
    }
    return 1;
}

int Date::setDate(int day,
                Month month,
                int year,
                Separator s,
                Format fmt)
{
    day_ = day;
    month_ = month;
    year_ = year;
    s_ = s;
    format_ = fmt;

    valid_ = isValid();
    if (!valid_)
        return 0;
    total_days_ = countDays();

    return 1;
}

int Date::grepAndSetDate(std::string date_string, Format fmt) {
    // ddmmyyy or mmddyyy
    std::regex year_last("(\\d?\\d)([\\/\\.\\-\\s])(\\d?\\d)[\\/\\.\\-\\s](\\d?\\d?\\d?\\d)");
    // yyyymmdd
    std::regex year_first("\\d?\\d?\\d?\\d([\\/\\.\\-\\s])\\d?\\d[\\/\\.\\-\\s]\\d?\\d");

    std::smatch match;

    if (fmt == Format::ddmmyyyy) {
        if (std::regex_search(date_string, match, year_last)) {
            if (match.ready()) {
                // extraxt day
                std::string day = match.str(1);
                int iday = std::stoi(day);

                // extract separator
                char sep = match.str(2)[0];

                // extract month
                std::string month = match.str(3);
                int imonth = std::stoi(month);

                // extract year
                std::string year = match.str(4);
                int iyear = std::stoi(year);

                int rv = setDate(iday,
                        static_cast<Month>(imonth),
                        iyear,
                        static_cast<Separator>(sep),
                        fmt);
                return rv;
            }
        }
    } else if (fmt == Format::mmddyyyy) {
        if (std::regex_search(date_string, match, year_last)) {
            if (match.ready()) {
                // extraxt day
                std::string day = match.str(3);
                int iday = std::stoi(day);

                // extract separator
                char sep = match.str(2)[0];

                // extract month
                std::string month = match.str(1);
                int imonth = std::stoi(month);

                // extract year
                std::string year = match.str(4);
                int iyear = std::stoi(year);

                int rv = setDate(iday,
                        static_cast<Month>(imonth),
                        iyear,
                        static_cast<Separator>(sep),
                        fmt);
                return rv;
            }
        }
    } else if (fmt == Format::yyyymmdd) {
        if (std::regex_search(date_string, match, year_first)) {
            if (match.ready()) {
                // extraxt day
                std::string day = match.str(4);
                int iday = std::stoi(day);

                // extract separator
                char sep = match.str(2)[0];

                // extract month
                std::string month = match.str(3);
                int imonth = std::stoi(month);

                // extract year
                std::string year = match.str(1);
                int iyear = std::stoi(year);

                int rv = setDate(iday,
                        static_cast<Month>(imonth),
                        iyear,
                        static_cast<Separator>(sep),
                        fmt);
                return rv;
            }
        }
    } else {
        std::cerr << "Bad date";
        return -1;
    }
    return 1;
}

std::istream& operator>> (std::istream& is, Date& dt) {
    std::cout << "Choose one of the three forms:\n"
        << "\tddmmyyyy (type 1)\n"
        << "\tmmddyyyy (type 2)\n"
        << "\tyyyymmdd (type 3)\n";
    int choice = 1;
    std::cin >> choice;
    std::cin.ignore();

    std::cout << "Enter date in the format that you picked.\n"
                << "Use one of the following separators: [./-] or space:\n";

    char  date_string[128];
    std::cin.ignore();
    std::cin.getline(date_string, std::numeric_limits<unsigned>::max());

    switch(choice) {
        case 1: dt.grepAndSetDate(date_string, Format::ddmmyyyy); break;
        case 2: dt.grepAndSetDate(date_string, Format::mmddyyyy); break;
        case 3: dt.grepAndSetDate(date_string, Format::yyyymmdd); break;
    }
    return is;
}

std::ostream& operator<< (std::ostream& os, const Date& dt) {
    if (!dt.valid_) {
        os << "Bad date\n";
        return os;
    }
    if (dt.format_ == Format::ddmmyyyy) {
        os << "date: " << dt.day_ << static_cast<char>(dt.s_)
        << static_cast<int>(dt.month_) << static_cast<char>(dt.s_)
        << dt.year_;
    } else if (dt.format_ == Format::mmddyyyy) {
        os << "date: "<< static_cast<int>(dt.month_) << static_cast<char>(dt.s_)
           << dt.day_ << static_cast<char>(dt.s_)
           << dt.year_;
    } else if (dt.format_ == Format::yyyymmdd) {
        os << "date: "<< dt.year_ << static_cast<char>(dt.s_)
           << static_cast<int>(dt.month_) << static_cast<char>(dt.s_)
           << dt.day_;
    }
    return os;
}

void Date::setFormat(const Format& fmt) {
    this->format_ = fmt;
}

std::string Date::format() const {
    switch (this->format_) {
        case Format::ddmmyyyy: return "ddmmyyyy"; break;
        case Format::mmddyyyy: return "mmddyyyy"; break;
        case Format::yyyymmdd: return "yyyymmdd";
    }
    return "Never have I ever been in this statement";
}
