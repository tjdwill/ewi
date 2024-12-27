// QtConverter.hpp
// A convenience struct defining static methods for converting between Qt and STL data
// types.
#ifndef INCLUDED_EWIQT_QTCONVERTER
#define INCLUDED_EWIQT_QTCONVERTER

#ifndef INCLUDED_STD_CHRONO
#include <chrono>
#define INCLUDED_STD_CHRONO
#endif

#ifndef INCLUDED_STD_STRING
#include <string>
#define INCLUDED_STD_STRING
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
    static auto to_stl(QDate const& date) -> std::chrono::year_month_day;
    static auto to_stl(QVector<QDate> const& dates) -> std::vector<std::chrono::year_month_day>;
    static auto from_stl(std::chrono::year_month_day const& date) -> QDate;
    static auto from_stl(std::vector<std::chrono::year_month_day> const& dates) -> QVector<QDate>;

    // Convert btw. std::string and QString
    static auto to_stl(QString const& s) -> std::string;
    static auto to_stl(QStringList const& strings) -> std::vector<std::string>;
    static auto from_stl(std::string const& s) -> QString;
    static auto from_stl(std::vector<std::string> const& strings) -> QStringList;

};
} // namespace ewiQt
#endif // INCLUDED_EWIQT_QTCONVERTER
