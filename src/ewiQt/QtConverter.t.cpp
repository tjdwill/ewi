// QtConverter.t.cpp
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
#include <cassert>
#include <chrono>
#include <string>
#include <vector>
//- Third-party
#include <QtCore>

void test_date();
void test_dates();
void test_string();
void test_strings();

int main()
{
    test_date();
    test_dates();
    test_string();
    test_strings();
}

// -------------------------------------------------------------------------------------------------
namespace
{
    using namespace std::chrono;
    using QtC = ewiQt::QtConverter;

    void fill_date_vecs(std::vector<year_month_day>& stl, QVector<QDate>& qt)
    {
        for (int i {0}; i < 7; ++i)
        {
            stl.push_back(year_month_day(2024y, April, 22d + days(i)));
            qt.push_back(QDate(2024, 04, 22+i));
        }
    }

    QStringList Qt_STRINGS { "foo", "bar", "Baz", };
    std::vector<std::string> STL_STRINGS { "foo", "bar", "Baz", };
}

void test_date()
{
    year_month_day STL_DATE { 2024y, December, 23d };
    QDate QT_DATE { 2024, 12, 23 };

    assert(STL_DATE == QtC::to_stl(QT_DATE));
    assert(QT_DATE == QtC::toQt(STL_DATE));
}

void test_dates()
{
    std::vector<year_month_day> STL_DATES {};
    QVector<QDate> QT_DATES {};
    fill_date_vecs(STL_DATES, QT_DATES);
    
    assert(QtC::to_stl(QT_DATES) == STL_DATES);
    assert(QtC::toQt(STL_DATES) == QT_DATES);
}

void test_string()
{
    assert(QtC::to_stl(Qt_STRINGS[0]) == STL_STRINGS[0]);
    assert(QtC::toQt(STL_STRINGS[0]) == Qt_STRINGS[0]);
}

void test_strings()
{
    assert(QtC::to_stl(Qt_STRINGS) == STL_STRINGS);
    assert(QtC::toQt(STL_STRINGS) == Qt_STRINGS);
}
