// QtConverter.cpp
/*
* Copyright (C) 2024 Terrance Williams
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "QtConverter.hpp"
//- STL
#include <chrono>
#include <sstream>
#include <string>
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

    auto QtConverter::to_stl(QDate const& date) -> std::chrono::year_month_day
    {
        std::string date_string { date.toString(QT_DATE_FORMAT).toStdString() };
        std::istringstream iss { date_string };
        std::chrono::year_month_day stl_date;
        std::chrono::from_stream(iss, STL_DATE_FORMAT.c_str(), stl_date);

        return stl_date;
    }

    auto QtConverter::to_stl(QVector<QDate> const& dates) -> std::vector<std::chrono::year_month_day>
    {
        std::vector<std::chrono::year_month_day> stl_dates {};
        for (auto const& date : dates)
            stl_dates.push_back(QtConverter::to_stl(date));

        return stl_dates;
    }

    auto QtConverter::from_stl(std::chrono::year_month_day const& date) -> QDate
    {
        std::ostringstream oss {};
        oss << date;
        QString qtDateString = QString::fromStdString(oss.str());

        return QDate::fromString(qtDateString, QT_DATE_FORMAT);
    }

    auto QtConverter::from_stl(std::vector<std::chrono::year_month_day> const& dates) -> QVector<QDate>
    {
        QVector<QDate> qtDates {};
        for (auto const& date : dates)
            qtDates.push_back(QtConverter::from_stl(date));

        return qtDates;
    }

    auto QtConverter::to_stl(QString const& s) -> std::string
    {
        return s.toStdString();
    }
    auto QtConverter::from_stl(std::string const& s) -> QString
    {
        return QString::fromStdString(s);
    }

    auto QtConverter::to_stl(QStringList const& strings) -> std::vector<std::string>
    {
        std::vector<std::string> v {};
        for (auto const& s : strings)
            v.push_back(QtConverter::to_stl(s));
        return v;
    }

    auto QtConverter::from_stl(std::vector<std::string> const& strings) -> QStringList
    {
        QStringList qsl {};
        for (auto const& s : strings)
            qsl.push_back(QtConverter::from_stl(s));
        return qsl;
    }
} // namespace ewiQt
