// entry.t.cpp
/// Entry test
/// Tests:
///     - Date comparison
///     - Entry equality comparison
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
#include <cassert>
#include <chrono>
#include <string>
#include <vector>

using Date = std::chrono::year_month_day;
using ewi::Entry;

void date_comp_test() 
{    
    using namespace std::chrono;
    // different date
    constexpr auto date = Date(2024y, std::chrono::October, 23d);
    Entry a(
            date,
            "",
            std::vector<double> {0.}
    );
    Entry b(
            date + std::chrono::months(2),
            "",
            std::vector<double> {0.}
    );

    assert(a < b);
    assert(a <= b);
    assert(!(a < a));
}

void equal_comp_test() 
{    
    using namespace std::chrono;
    // different date
    constexpr auto date = Date(2024y, std::chrono::October, 23d);
    Entry a(
            date,
            "",
            std::vector<double> {0., 0.1}
    );
    Entry b(
            date,
            "",
            std::vector<double> {0.}
    );
    Entry c(
            date,
            "",
            std::vector<double> {}
    );


    assert(a == a);
    assert(a != b);
    assert((b != c) && (a != c));
}

int main() 
{
    date_comp_test();
    equal_comp_test();
}
