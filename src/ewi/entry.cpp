// entry.cpp
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
#include "entry.hpp"
//- STL
#include <format>
#include <iostream>
#include <vector>
//- In-house
#include <utils/string_flattener/string_flattener.hpp>

namespace ewi
{
    
    auto operator== (Entry const& a, Entry const& b) noexcept -> bool
    {
        if (a.date() != b.date())
            return false;
        if (a.notes() != b.notes())
            return false;
        // vector comp
        auto a_vec = a.metrics();
        auto b_vec = b.metrics();
        if (a_vec.size() != b_vec.size())
            return false;
        return a_vec == b_vec;
    }

    auto operator<< (std::ostream& os, Entry const& e) noexcept -> std::ostream&
    {
        os << std::format("{:%F}", e.date()) << ' '
           << utils::StringFlattener::flatten(e.notes()) << ' ';
    
        for (double d: e.metrics())
            os << d << " "; 

        return os;
    }
}
