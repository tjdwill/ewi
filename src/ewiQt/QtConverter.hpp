// QtConverter.hpp
// A convenience struct defining static methods for converting between Qt and STL data
// types.
#ifndef INCLUDED_EWIQT_QTCONVERTER
#define INCLUDED_EWIQT_QTCONVERTER

#ifndef INCLUDED_STD_CHRONO
#include <chrono>
#define INCLUDED_STD_CHRONO
#endif

#ifndef INCLUDED_STD_VECTOR
#include <vector>
#define INCLUDED_STD_VECTOR
#endif

#ifndef INCLUDED_QT_QTCORE
#include <QtCore>
#define INCLUDED_QT_QTCORE
#endif

namespace ewiQt
{
/// A struct for converting between Qt and STL data types.
struct QtConverter
{
    /// convert between QDate and std::chrono::year_month_day
    static auto to_ymd(QDate const& date) -> std::chrono::year_month_day;
    static auto to_ymd(QVector<QDate> const& dates) -> std::vector<std::chrono::year_month_day>;
    static auto from_ymd(std::chrono::year_month_day const& date) -> QDate;
    static auto from_ymd(std::vector<std::chrono::year_month_day> const& dates) -> QVector<QDate>;
};
} // namespace ewiQt
#endif // INCLUDED_EWIQT_QTCONVERTER
