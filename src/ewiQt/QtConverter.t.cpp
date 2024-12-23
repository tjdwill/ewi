// QtConverter.t.cpp
#include "QtConverter.hpp"
//-
#include <cassert>
#include <chrono>
#include <vector>
//-
#include <QtCore>

void test_date();
void test_vec_dates();

int main()
{
    test_date();
    test_vec_dates();
}

// -------------------------------------------------------------------------------------------------
namespace
{
    using namespace std::chrono;
    using ewiQt::QtConverter;

    void fill_date_vecs(std::vector<year_month_day>& stl, QVector<QDate>& qt)
    {
        for (int i {0}; i < 7; ++i)
        {
            stl.push_back(year_month_day(2024y, April, 22d + days(i)));
            qt.push_back(QDate(2024, 04, 22+i));
        }
    }
}

void test_date()
{
    year_month_day STL_DATE { 2024y, December, 23d };
    QDate QT_DATE { 2024, 12, 23 };

    assert(STL_DATE == QtConverter::to_ymd(QT_DATE));
    assert(QT_DATE == QtConverter::from_ymd(STL_DATE));
}

void test_vec_dates()
{
    std::vector<year_month_day> STL_DATES {};
    QVector<QDate> QT_DATES {};
    fill_date_vecs(STL_DATES, QT_DATES);
    
    assert(QtConverter::to_ymd(QT_DATES) == STL_DATES);
    assert(QtConverter::from_ymd(STL_DATES) == QT_DATES);
}
