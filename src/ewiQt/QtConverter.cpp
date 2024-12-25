// QtConverter.cpp
#include "QtConverter.hpp"
//- STL
#include <chrono>
#include <sstream>
#include <vector>
//- Third-party
#include <QtCore>


namespace ewiQt
{
    // https://doc.qt.io/qt-5/qdate.html#toString-2
    // 
    // This format was chosen to conform with the default format used in `std::chrono`,
    // specifically, the `operator<<` overload.
    QString QT_DATE_FORMAT { "yyyy-MM-dd" }; 
    // The STL format is the same as `%F` but written explicitly for conformity to the Qt
    // version.                                                     
    // https://en.cppreference.com/w/cpp/chrono/year_month_day/formatter
    std::string STL_DATE_FORMAT { "%Y-%m-%d" };   

    auto QtConverter::to_ymd(QDate const& date) -> std::chrono::year_month_day
    {
        std::string date_string { date.toString(QT_DATE_FORMAT).toStdString() };
        std::istringstream iss { date_string };
        std::chrono::year_month_day stl_date;
        std::chrono::from_stream(iss, STL_DATE_FORMAT.c_str(), stl_date);

        return stl_date;
    }

    auto QtConverter::to_ymd(QVector<QDate> const& dates) -> std::vector<std::chrono::year_month_day>
    {
        std::vector<std::chrono::year_month_day> stl_dates {};
        for (auto const& date : dates)
            stl_dates.push_back(QtConverter::to_ymd(date));

        return stl_dates;
    }

    auto QtConverter::from_ymd(std::chrono::year_month_day const& date) -> QDate
    {
        std::ostringstream oss {};
        oss << date;
        QString qtDateString = QString::fromStdString(oss.str());

        return QDate::fromString(qtDateString, QT_DATE_FORMAT);
    }

    auto QtConverter::from_ymd(std::vector<std::chrono::year_month_day> const& dates) -> QVector<QDate>
    {
        QVector<QDate> qtDates {};
        for (auto const& date : dates)
            qtDates.push_back(QtConverter::from_ymd(date));

        return qtDates;
    }
} // namespace ewiQt
