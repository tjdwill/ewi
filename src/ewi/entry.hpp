// entry.hpp
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
#ifndef INCLUDED_EWI_ENTRY
#define INCLUDED_EWI_ENTRY

#ifndef INCLUDED_STD_CHRONO
#include <chrono>
#define INCLUDED_STD_CHRONO
#endif

#ifndef INCLUDED_STD_OSTREAM
#include <ostream>
#define INCLUDED_STD_OSTREAM
#endif

#ifndef INCLUDED_STD_STRING
#include <string>
#define INCLUDED_STD_STRING
#endif

#ifndef INCLUDED_STD_VECTOR
#include <vector>
#define INCLUDED_STD_VECTOR
#endif

namespace ewi
{
    /// A given data entry. Can be for either job-related surveys or personal surveys.
    class Entry 
    {
        public:
            // CONSTRUCTORS
            Entry() = delete;
            Entry(
                    std::chrono::year_month_day date,
                    std::string const& notes,
                    std::vector<double> const& metrics
            ) noexcept
                : d_date(date), d_notes(std::move(notes)), d_metrics(std::move(metrics)) {}

            inline auto date() const noexcept -> std::chrono::year_month_day const& { return d_date; }
            inline auto notes() const noexcept -> std::string const& { return d_notes; }
            inline auto metrics() const noexcept -> std::vector<double> const& { return d_metrics; }

        private:
            std::chrono::year_month_day d_date;
            std::string d_notes;
            std::vector<double> d_metrics;      
    };
    inline auto operator<=> (Entry const& a, Entry const& b) noexcept { return a.date() <=> b.date(); }  // The dates are equal.
    auto operator==(Entry const& a, Entry const& b) noexcept -> bool;  // all data members are equal.
    /// Output the Entry on a single line. To do so, the notes string is flattened,
    /// replacing newline characters with a symbol that doesn't break the line.
    auto operator<<(std::ostream& os, Entry const& e) noexcept -> std::ostream&;
} // namespace ewi
#endif // INCLUDED_EWI_ENTRY
